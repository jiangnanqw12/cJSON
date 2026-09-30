"""Build an embedded UTF-8 Q2 bundle; encryption is an optional dependency.

Framing and encoding detection follow q2/src/plot_q_data.py. All decoded text
uses LF so q2's universal-newline bundle reader cannot invalidate frame sizes.
QR image generation is not needed for the transparent TCP link.
"""
import argparse
import base64
import binascii
from collections import deque
import getpass
import io
import json
import os
from pathlib import Path
import re
import shutil
import stat
import sys
import tempfile
import warnings
import zipfile
import zlib

MAX_FRAME_CHARS = 1500
FALLBACK_ENCODINGS = ("gb18030", "gbk", "gb2312", "cp936")


# Password input, encrypted archives, and safe restoration.
MAGIC = b"TTAENC1\0"
HEADER_SIZE = len(MAGIC) + 16 + 12
ENCRYPTED_NAME = "encrypted.tta.txt"


def crypto_types():
    try:
        from cryptography.exceptions import InvalidTag
        from cryptography.hazmat.primitives.ciphers.aead import AESGCM
        from cryptography.hazmat.primitives.kdf.scrypt import Scrypt
    except ImportError as error:
        raise ValueError("Encryption requires: python3 -m pip install -r src/generator/requirements.txt") from error
    return AESGCM, Scrypt, InvalidTag


def read_password(password_file=None, confirm=False):
    if password_file is not None:
        password = Path(password_file).expanduser().read_bytes().decode("utf-8")
        if password.endswith("\r\n"):
            password = password[:-2]
        elif password.endswith("\n"):
            password = password[:-1]
    else:
        # Never fall back to an echoed password when no controlling terminal exists.
        with warnings.catch_warnings():
            warnings.simplefilter("error", getpass.GetPassWarning)
            try:
                password = getpass.getpass("Password: ")
                if confirm and password != getpass.getpass("Confirm password: "):
                    raise ValueError("Passwords do not match")
            except (getpass.GetPassWarning, EOFError) as error:
                raise ValueError("A hidden password prompt requires a terminal; use --password-file") from error
    if not password or "\r" in password or "\n" in password or "\0" in password:
        raise ValueError("Password must be a non-empty single line without NUL")
    return password.encode("utf-8")


def validate_paths(names):
    """Validate a complete file-only archive before creating any directories."""
    seen, files, directories = {}, set(), set()
    for name in names:
        parts = name.split("/")
        for part in parts:
            if (not part or part in (".", "..") or part.endswith((" ", ".")) or
                    any(ord(c) < 32 or c in '<>:"\\|?*' for c in part)):
                raise ValueError(f"Unsafe archive path: {name!r}")
            stem = part.split(".")[0].rstrip(" ").upper()
            if stem in {"CON", "PRN", "AUX", "NUL"} or re.fullmatch(r"(COM|LPT)[1-9¹²³]", stem):
                raise ValueError(f"Reserved Windows filename: {name!r}")
        for count in range(1, len(parts) + 1):
            path = "/".join(parts[:count])
            key = path.casefold()
            if seen.setdefault(key, path) != path:
                raise ValueError(f"Case collision in archive: {name!r}")
            if count < len(parts):
                directories.add(key)
        key = name.casefold()
        if key in files:
            raise ValueError(f"Duplicate archive path: {name!r}")
        files.add(key)
    if not files or files & directories:
        raise ValueError("Empty archive or file/directory conflict")


def encrypt_archive(archive, password):
    AESGCM, Scrypt, _ = crypto_types()
    salt, nonce = os.urandom(16), os.urandom(12)
    header = MAGIC + salt + nonce
    key = Scrypt(salt=salt, length=32, n=2**17, r=8, p=1).derive(password)
    ciphertext = AESGCM(key).encrypt(nonce, archive, header)
    return base64.b64encode(header + ciphertext).decode("ascii") + "\n"


def encrypt_files(files, password):
    files = list(files)
    validate_paths(name for name, _ in files)
    buffer = io.BytesIO()
    with zipfile.ZipFile(buffer, "w", compression=zipfile.ZIP_DEFLATED) as archive:
        for name, text in files:
            info = zipfile.ZipInfo(name)
            info.compress_type = zipfile.ZIP_DEFLATED
            info.create_system = 3
            info.external_attr = (stat.S_IFREG | 0o600) << 16
            archive.writestr(info, text.encode("utf-8"))
    return encrypt_archive(buffer.getvalue(), password)


def decrypt_archive(text, password):
    AESGCM, Scrypt, InvalidTag = crypto_types()
    try:
        # Our writer emits one Base64 line; permit a terminal LF or CRLF on export.
        encoded = text[:-2] if text.endswith(b"\r\n") else text[:-1] if text.endswith(b"\n") else text
        envelope = base64.b64decode(encoded, validate=True)
    except (ValueError, binascii.Error) as error:
        raise ValueError("Invalid encrypted package encoding") from error
    if len(envelope) < HEADER_SIZE + 16 or not envelope.startswith(MAGIC):
        raise ValueError("Unsupported or truncated encrypted package")
    header = envelope[:HEADER_SIZE]
    salt, nonce = header[8:24], header[24:36]
    key = Scrypt(salt=salt, length=32, n=2**17, r=8, p=1).derive(password)
    try:
        return AESGCM(key).decrypt(nonce, envelope[HEADER_SIZE:], header)
    except InvalidTag as error:
        raise ValueError("Incorrect password or damaged encrypted package") from error


def decrypt_file(source, destination, password):
    source, destination = Path(source).expanduser(), Path(destination).expanduser()
    if destination.exists() or destination.is_symlink():
        raise ValueError(f"Output must not already exist: {destination}")
    plaintext = decrypt_archive(source.read_bytes(), password)
    staging = None
    try:
        with zipfile.ZipFile(io.BytesIO(plaintext)) as archive:
            entries = archive.infolist()
            validate_paths(info.orig_filename for info in entries)
            for info in entries:
                mode = info.external_attr >> 16
                if (info.filename != info.orig_filename or info.is_dir() or
                        stat.S_IFMT(mode) not in (0, stat.S_IFREG) or info.flag_bits & 1 or
                        info.compress_type != zipfile.ZIP_DEFLATED):
                    raise ValueError("Archive must contain only regular DEFLATE text files")
            destination.parent.mkdir(parents=True, exist_ok=True)
            staging = Path(tempfile.mkdtemp(prefix=".tta-decrypt-", dir=destination.parent))
            for info in entries:
                body = archive.read(info).decode("utf-8")
                if "\0" in body or "\r" in body:
                    raise ValueError("Archive contains non-normalized text")
                target = staging.joinpath(*info.filename.split("/"))
                target.parent.mkdir(parents=True, exist_ok=True)
                with target.open("xb") as stream:
                    stream.write(body.encode("utf-8"))
                    stream.flush()
                    os.fsync(stream.fileno())
            if destination.exists() or destination.is_symlink():
                raise ValueError(f"Output must not already exist: {destination}")
            staging.rename(destination)
            staging = None
    except (zipfile.BadZipFile, zlib.error, NotImplementedError, RuntimeError) as error:
        raise ValueError("Invalid encrypted text archive") from error
    finally:
        if staging is not None:
            shutil.rmtree(staging)


# Text discovery, Q2 framing, and C++ resource generation.
def read_text(path):
    try:
        # Match q2's UTF-8 universal-newline read, including BOM preservation.
        with path.open("r", encoding="utf-8") as source:
            return source.read()
    except UnicodeDecodeError:
        raw = path.read_bytes()
    for encoding in FALLBACK_ENCODINGS:
        try:
            return raw.decode(encoding).replace("\r\n", "\n").replace("\r", "\n")
        except UnicodeDecodeError:
            pass
    raise ValueError(f"cannot decode text file: {path}")


def payload(path, index, total, body):
    header = f"Q2_BEGIN path={path} part={index}/{total} size={len(body)}"
    return f"{header}\n{body}\nQ2_END" if body else f"{header}\nQ2_END"


def bodies_for_total(path, data, limit, total):
    segments = deque(data.splitlines(keepends=True))
    bodies = []
    body = ""
    while segments:
        index = len(bodies) + 1
        segment = segments.popleft()
        if len(payload(path, index, total, body + segment)) <= limit:
            body += segment
            continue
        if body:
            bodies.append(body)
            body = ""
            segments.appendleft(segment)
            continue
        low, high, best = 1, len(segment), 0
        while low <= high:
            middle = (low + high) // 2
            if len(payload(path, index, total, segment[:middle])) <= limit:
                best, low = middle, middle + 1
            else:
                high = middle - 1
        if best == 0:
            raise ValueError(f"path too long for a {limit}-character Q2 frame: {path}")
        bodies.append(segment[:best])
        if best < len(segment):
            segments.appendleft(segment[best:])
    if body or not bodies:
        bodies.append(body)
    return bodies


def frames(path, data, limit=MAX_FRAME_CHARS):
    total = 1
    while True:
        bodies = bodies_for_total(path, data, limit, total)
        if len(bodies) == total:
            result = [payload(path, i + 1, total, body) for i, body in enumerate(bodies)]
            if any(len(frame) > limit for frame in result):
                raise ValueError(f"path too long for a {limit}-character Q2 frame: {path}")
            return result
        total = len(bodies)


def discover(directory, extensions, recursive, output=None, exclude=()):
    root = Path(directory).expanduser()
    if root.is_symlink() or not root.is_dir():
        raise ValueError(f"input must be a directory, not a symlink: {root}")
    selected = set()
    for extension in extensions:
        if not isinstance(extension, str):
            raise ValueError("extensions entries must be strings")
        extension = extension.strip().lower()
        if not extension or any(c in extension for c in "/\\\r\n"):
            raise ValueError("extensions must be non-empty filename suffixes")
        selected.add(extension if extension.startswith(".") else "." + extension)
    found = []
    excluded = [Path(path).expanduser() for path in exclude]
    if output is not None:
        excluded.append(output)

    def walk_error(error):
        raise error

    for current, dirs, names in os.walk(root, onerror=walk_error, followlinks=False):
        dirs[:] = sorted(d for d in dirs if not (Path(current) / d).is_symlink()) if recursive else []
        for name in names:
            candidate = Path(current) / name
            if candidate.is_symlink() or not candidate.is_file() or candidate.suffix.lower() not in selected:
                continue
            if any(candidate.resolve() == path.resolve() or
                   (path.exists() and candidate.samefile(path)) for path in excluded):
                continue
            relative = candidate.relative_to(root).as_posix()
            # Q2's header is a single line; backslash is also ambiguous on Windows restore.
            if any(ord(c) < 32 for c in relative) or "\\" in relative:
                raise ValueError(f"unsupported Q2 path: {relative!r}")
            found.append((relative, candidate))
    if not found:
        raise ValueError(f"no files match the selected extensions in {root}")
    return sorted(found)


def cpp_string(value):
    # Fixed-width octal escapes cannot consume the next character like \x can.
    return '"' + "".join(f"\\{byte:03o}" for byte in value.encode("utf-8")) + '"'


def read_files(directory, extensions, recursive, output=None, exclude=()):
    for relative, path in discover(directory, extensions, recursive, output, exclude):
        try:
            text = read_text(path)
            if "\0" in text:
                raise ValueError("NUL is not supported in text input")
        except (UnicodeError, ValueError, OSError) as error:
            raise ValueError(f"{path}: {error}") from error
        yield relative, text


def generate(directory, extensions, recursive, output=None, *, password=None, exclude=()):
    texts = read_files(directory, extensions, recursive, output, exclude)
    if password is not None:
        texts = [(ENCRYPTED_NAME, encrypt_files(texts, password))]
    entries, bundle = [], bytearray()
    for relative, text in texts:
        chunks = frames(relative, text)
        entries.append(f"    {{{cpp_string(relative)}, {len(text.encode('utf-8'))}U, {len(chunks)}U}},")
        for chunk in chunks:
            bundle.extend(chunk.encode("utf-8"))
            bundle.append(10)
    lines = ['#include "embedded_text.hpp"', 'namespace mvs::text {', 'const File files[] = {']
    lines.extend(entries or ['    {nullptr, 0U, 0U},'])
    lines.extend(['};', f'const std::size_t fileCount = {len(entries)}U;', 'const unsigned char bundle[] = {'])
    for offset in range(0, len(bundle), 24):
        lines.append("    " + ",".join(str(b) for b in bundle[offset:offset + 24]) + ",")
    if not bundle:
        lines.append("    0,")
    lines.extend(['};', f'const std::size_t bundleSize = {len(bundle)}U;', '} // namespace mvs::text', ''])
    return "\n".join(lines)


DEFAULT_OUTPUT = "../../pd/src/subsystem/mvs/verification/resources/embedded_text.cpp"


def load_config(config_path, *, input_dir=None, output_cpp=None):
    config_path = Path(config_path).expanduser().resolve()
    config = {}
    layers = [config_path]
    local = config_path.with_name(config_path.stem + "_local" + config_path.suffix)
    if not config_path.stem.endswith("_local") and local.exists():
        layers.append(local)
    for selected in layers:
        with selected.open(encoding="utf-8-sig") as stream:
            values = json.load(stream)
        if not isinstance(values, dict):
            raise ValueError(f"config must be a JSON object: {selected}")
        unknown = set(values) - {"input_dir", "extensions", "recursive", "output_cpp"}
        if unknown:
            raise ValueError(f"unknown config fields in {selected}: " + ", ".join(sorted(unknown)))
        config.update(values)
    cli_paths = {name for name, value in (("input_dir", input_dir), ("output_cpp", output_cpp))
                 if value is not None}
    if input_dir is not None:
        config["input_dir"] = input_dir
    if output_cpp is not None:
        config["output_cpp"] = output_cpp
    input_dir = config.get("input_dir", "")
    output_cpp = config.get("output_cpp", DEFAULT_OUTPUT)
    for name, value in (("input_dir", input_dir), ("output_cpp", output_cpp)):
        if not isinstance(value, str) or not value.strip():
            raise ValueError(f"{name} must be a non-empty path; edit {config_path}")
    extensions = config.get("extensions", [".txt"])
    if not isinstance(extensions, list) or not extensions:
        raise ValueError("extensions must be a non-empty JSON array of suffixes")
    recursive = config.get("recursive", True)
    if not isinstance(recursive, bool):
        raise ValueError("recursive must be true or false")

    def absolute(name, value):
        path = Path(value).expanduser()
        # Keep the leaf unresolved so discover can reject a symlink input root.
        base = Path.cwd() if name in cli_paths else config_path.parent
        return path if path.is_absolute() else base / path

    return absolute("input_dir", input_dir), extensions, recursive, absolute("output_cpp", output_cpp)


def write_output(output, content):
    if output.is_symlink():
        raise ValueError(f"output must not be a symlink: {output}")
    encoded = content.encode("utf-8")
    if output.exists() and output.read_bytes() == encoded:
        return False
    output.parent.mkdir(parents=True, exist_ok=True)
    temporary = None
    try:
        with tempfile.NamedTemporaryFile(dir=output.parent, prefix=".text-to-array-", suffix=".tmp", delete=False) as stream:
            temporary = Path(stream.name)
            stream.write(encoded)
        temporary.replace(output)
    finally:
        if temporary is not None:
            temporary.unlink(missing_ok=True)
    return True


def main(argv=None):
    parser = argparse.ArgumentParser(description="Convert text files in a directory into embedded C++ resources.")
    parser.add_argument("--config", type=Path,
                        default=Path(__file__).resolve().parent / "text_to_array.json",
                        help="configuration file (default: text_to_array.json next to this script)")
    parser.add_argument("-i", "--input-dir", metavar="DIR",
                        help="input directory (overrides config; relative to the working directory)")
    parser.add_argument("-o", "--output", metavar="FILE",
                        help="output C++ file (overrides config; relative to the working directory)")
    parser.add_argument("--encrypt", action="store_true", help="compress and password-encrypt the selected files")
    parser.add_argument("--password-file", type=Path,
                        help="UTF-8 single-line password file; requires --encrypt; otherwise prompt")
    args = parser.parse_args(argv)
    if args.password_file is not None and not args.encrypt:
        parser.error("--password-file requires --encrypt")
    try:
        source, extensions, recursive, output = load_config(args.config, input_dir=args.input_dir,
                                                          output_cpp=args.output)
        password, exclude = None, ()
        if args.encrypt:
            crypto_types()
            if args.password_file is not None:
                password_path = args.password_file.expanduser()
                if output.resolve() == password_path.resolve() or (
                        output.exists() and password_path.exists() and output.samefile(password_path)):
                    raise ValueError("Output must not replace the password file")
                exclude = (password_path,)
            password = read_password(args.password_file, confirm=True)
        content = generate(source, extensions, recursive, output, password=password, exclude=exclude)
        changed = write_output(output, content)
    except (ValueError, OSError, OverflowError) as error:
        print(f"text-to-array: {error}", file=sys.stderr)
        return 1
    except KeyboardInterrupt:
        print("text-to-array: cancelled", file=sys.stderr)
        return 1
    print(f"{'Generated' if changed else 'Unchanged'}: {output}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
