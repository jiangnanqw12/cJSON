---
marp: true
theme: default
paginate: true
size: 16:9
header: C 语言编程规范培训
footer: 从“能运行”到“工程上可信”
style: |
  section {
    font-size: 28px;
    line-height: 1.32;
    padding: 54px 64px;
  }
  section.lead h1 {
    font-size: 54px;
  }
  h1 {
    font-size: 40px;
  }
  h2 {
    font-size: 31px;
  }
  table {
    font-size: 23px;
  }
  pre {
    font-size: 20px;
  }
  code {
    font-size: 0.92em;
  }
  .small {
    font-size: 22px;
  }
---
<!-- _class: lead -->
<!-- _paginate: false -->
<!-- _header: '' -->
<!-- _footer: '' -->

# C 语言编程规范培训

## 从“能运行”到“工程上可信”

Readable · Correct · Secure
---

# 这次培训解决什么问题？

- 不是把所有人训练成同一种“代码审美”。
- 而是让大家能判断：这段 C 代码是否 **可读、正确、安全、可 Review**。
- 看到问题时，不只说“违反规范”，还要能说清：
  - 风险在哪里？
  - 什么时候触发？
  - 会造成什么影响？
  - 应该怎么修？
---

# 看业界：规范不是额外负担

很多成熟项目都会有自己的 C/C++ 编码规范：

| 来源 | 典型关注点 |
|---|---|
| Linux Kernel | 风格统一、可维护、长期演进 |
| Google C++ Style Guide | 接口边界、命名、可读性 |
| 华为 / 企业规范 | 安全、可靠、工程一致性 |

**结论：规范不是为了限制工程师，而是为了让大型代码长期可维护。**
---

# 一个风格问题，为什么会变成安全漏洞？

Apple SSL/TLS 的 `goto fail` 案例：

```c
if ((err = SomeCheck()) != 0)
    goto fail;
    goto fail;  /* Unconditional. */
```

- 缩进不会改变 C 语义，但会影响人对控制流的判断。
- 缺少统一的大括号规则，会增加 Review 漏检概率。
- “只是格式问题”不等于“没有工程风险”。
---

# 不是所有规范都同样重要

| 等级 | 含义 | 例子 |
|---|---|---|
| **MUST** | 违反可能导致缺陷、安全问题或不可控行为 | 越界、未初始化、整数溢出、错误长度 |
| **SHOULD** | 通常应遵守；允许有明确工程理由的例外 | 函数大小、参数数量、嵌套深度 |
| **STYLE** | 统一即可，优先交给工具 | 缩进、空格、大括号、换行 |

**MUST 要理解机制，SHOULD 要理解边界，STYLE 要自动化。**
---

# 谁来发现问题？

| 问题 | Formatter | Compiler | Static Analyzer | Review |
|---|:---:|:---:|:---:|:---:|
| 缩进 / 空格 | ✓ | | | |
| 未初始化变量 | | ✓ | ✓ | ✓ |
| signed / unsigned | | ✓ | ✓ | ✓ |
| 越界 | | 部分 | ✓ | ✓ |
| 魔鬼数字 | | | 部分 | ✓ |
| 错误抽象 / 所有权 | | | 部分 | ✓ |

**机器能稳定判定的，不要浪费人工 Review 注意力。**
---

<!-- _class: lead -->

# 贯穿案例

## 从一个“能跑”的报文处理函数开始
---

# Quiz：第一眼看起来没什么问题？

```c
int RecvMsgProc (char *buf, int len)
{
    char msgType = 0;
    int ret;

    msgType = GetMsgType( buf, len );
    switch (msgType) {
    case AAA:
        SetAaaMsg(buf, len);
    case BBB:
        ret = ProcAaaBbbMsg(buf, len);
        break;
    case CCC:
        ProcCCcMsg(buf,len);
        break;
    }
    return ret;
}
```

**先不要改。60 秒内你能找出多少问题？**
---

# Answer：至少有这些问题

| 等级 | 问题 |
|---|---|
| STYLE | 函数名与 `(`、实参空格不一致 |
| SHOULD | 注释缺失或只重复函数名语义 |
| MUST | `ret` 可能未初始化就返回 |
| MUST | `case AAA` 可能发生非预期 fall-through |
| SHOULD | `switch` 没有明确 `default` 策略 |
| SHOULD | `char msgType` 语义和取值范围不清楚 |
| MUST | `buf` / `len` 的有效性和数据边界没有体现 |

后续所有章节都会回到这个例子。
---

<!-- _class: lead -->

# Part 1 — Readable

## 可读与可维护
---

# Quiz：命名里要不要带类型？

```c
int iPacketLen;
int packetLen;
```

你更推荐哪个？为什么？

**建议判断：**

- 名字应该表达业务语义，不要重复编码编译器已经知道的静态类型信息。
- 匈牙利命名这类做法在现代工程中通常不是首选。
- 统一命名比争论“哪种风格最优”更重要。
---

# G.NAM.01 使用统一的命名风格

| 对象 | 示例 |
|---|---|
| 函数 / 类型 | `GetPacketId`、`DeviceConfig` |
| 局部变量 / 参数 | `packetLen` |
| 全局变量 | `g_packetCount` |
| 宏 / 枚举值 | `MAX_PACKET_LEN` |

- 命名要能体现业务语义和责任。
- 同一层级、同一概念要保持一致。
- 跨模块一致，比“每个模块内部自洽”更重要。
---

# Quiz：大括号只是风格吗？

```c
if (ready)
    Start();
    LogStart();
```

这段代码的真实控制流是什么？

**结论：**

- 大括号规则不是为了审美，而是为了让控制范围清晰。
- 对 `if / for / while`，建议统一使用大括号。
- STYLE 规则应该由 formatter 固化，不要靠人肉争论。
---

# 注释：解释 Why，不要翻译 What

低价值：

```c
// Increase retry count.
retryCount++;
```

高价值：

```c
// The device needs at least 10 ms before retrying after a NACK.
SleepMs(10);
retryCount++;
```

好的注释通常解释：

- 为什么这样实现？
- 有什么非显然约束？
- 为什么不能采用“看起来更简单”的方案？
---

# G.EXP.02 表达式比较：变化值放左侧，稳定值放右侧

推荐：

```c
if (packetLen > MAX_PACKET_LEN) {
    return ERR_PACKET_TOO_LARGE;
}
```

不推荐：

```c
if (MAX_PACKET_LEN < packetLen) {
    return ERR_PACKET_TOO_LARGE;
}
```

**目的：让读者先看到正在被判断的对象，再看到边界。**
---

# Quiz：哪些数字是魔鬼数字？

```c
int totalMonths = year * 12 + month;
if (chipType == 12) {
    EnableSpecialMode();
}
```

哪一个 `12` 需要改？为什么？

**判断问题：读者是否必须离开当前上下文，才能知道这个数字是什么？**
---

# 魔鬼数字：关键是语义是否可见

不是所有字面量都要定义常量：

```c
int totalMonths = year * 12 + month;  /* 12 is obvious here. */
```

业务值应该具名：

```c
typedef enum {
    CHIP_TYPE_UNKNOWN = 0,
    CHIP_TYPE_XYZ = 12,
} ChipType;
```

**常量名不是为了消灭数字，而是为了暴露语义。**
---

# G.INC.05 头文件应当自包含

如果 `file_op.h` 使用 `FileHandle`，它自己就应该包含定义 `FileHandle` 所需的头文件。

```c
/* type.h */
typedef int FileHandle;

/* file_op.h */
#include "type.h"

void CreateFile(FileHandle handle);
```

不要依赖某个 `.c` 文件“碰巧先 include 了另一个头”。
---

# 函数要短小：阈值是信号，不是数学定律

典型工程约束：

- 非注释代码约 ≤ 50 行。
- 单行约 ≤ 120 列。
- 嵌套约 ≤ 4 层。
- 参数约 ≤ 5 个。

这些数字的目的不是“达标”，而是暴露职责过多、抽象混杂、接口不合理。
---

# 参数太多时，先判断是不是同一概念

```c
int ConfigureDevice(int id,
                    int mode,
                    int timeout,
                    int retry,
                    int width,
                    int height,
                    int flags);
```

可能更合理：

```c
typedef struct {
    int mode;
    int timeout;
    int retry;
    int width;
    int height;
    unsigned int flags;
} DeviceConfig;

int ConfigureDevice(int id, const DeviceConfig *config);
```

**Review 问题：这些参数是否真的属于同一个概念？**
---

<!-- _class: lead -->

# Part 2 — Correct

## 控制流、错误处理与类型
---

# switch：每个分支都要明确终止策略

```c
switch (msgType) {
case MSG_AAA:
    HandleA();
    break;
case MSG_BBB:
    HandleB();
    break;
default:
    return ERR_UNSUPPORTED_MSG;
}
```

如果确实需要 fall-through：

- 必须是设计意图。
- 必须能被读者和工具识别。
- 优先使用项目约定的注释或标准属性（如适用）。
---

# 所有返回路径都必须定义结果

危险：

```c
int ret;

if (condition) {
    ret = DoWork();
}

return ret;
```

更好的设计通常不是“先 `ret = 0` 消告警”，而是：

```c
if (!condition) {
    return ERR_INVALID_STATE;
}

return DoWork();
```

**无意义的默认初始化可能隐藏遗漏路径。**
---

# goto：在 C 中可以是正确的资源清理工具

```c
int Process(void)
{
    int ret;
    Resource *a = AcquireA();
    if (a == NULL) {
        return ERR_A;
    }

    Resource *b = AcquireB();
    if (b == NULL) {
        ret = ERR_B;
        goto release_a;
    }

    ret = DoWork(a, b);
    ReleaseB(b);
release_a:
    ReleaseA(a);
    return ret;
}
```

不要无条件禁止，也不要为了“统一出口”强行使用。
---

# 类型转换前，先问“值域是否兼容”

窄化转换：

```c
uint16_t src = GetValue();
uint8_t dst = (uint8_t)src;
```

正确问题不是“有没有 cast”，而是：

1. `src` 的真实可能范围是什么？
2. 业务允许范围是什么？
3. `uint8_t` 能否完整表示？
4. 超出范围时应该报错、饱和还是截断？
---

# 指针转整数：不要猜数据模型

危险：

```c
int addr = (int)ptr;
```

问题：

- 64 位平台上指针通常比 `int` 宽。
- 截断后再转回指针可能已经不是原地址。
- 这类代码高度依赖平台 ABI（Application Binary Interface，应用二进制接口）。

如果必须保存指针整数值，优先使用标准整数类型：

```c
uintptr_t addr = (uintptr_t)ptr;
```
---

# signed / unsigned：危险往往发生在隐式转换

```c
int len = -1;
unsigned int max = 1024;

if (len < max) {
    Copy(buf, len);
}
```

问题不在 `if` 本身，而在比较前的 usual arithmetic conversion（通常算术转换）：

- `len` 可能先被转换为 unsigned。
- `-1` 变成一个很大的无符号值。
- 判断结果和人类直觉不一致。
---

# 整数表达式：溢出和提升都要看

```c
uint16_t width;
uint16_t height;

uint32_t bytes = width * height * 4;
```

Review 时不要只看左侧类型：

- 右侧表达式可能先按 `int` 或其他类型计算。
- 中间结果可能已经溢出。
- cast 放在最后通常救不了已经溢出的中间值。

更安全的思路：先提升，再检查，再计算。
---

# 有符号溢出不是普通回绕

```c
int count = INT_MAX;
count++;
```

在 C 中，signed overflow（有符号溢出）属于 undefined behavior（未定义行为）。

这意味着：

- 编译器可以基于“它不会发生”做优化。
- 运行表现不一定是稳定回绕。
- 安全判断不能建立在这种行为上。

无符号整数可以按模回绕，但也不代表业务上安全。
---

# Case：让 `RecvMsgProc` 先变正确

```c
int RecvMsgProc(const uint8_t *buf, size_t len)
{
    if (buf == NULL || len < MIN_MSG_LEN) {
        return ERR_INVALID_ARG;
    }

    MsgType msgType = GetMsgType(buf, len);

    switch (msgType) {
    case MSG_AAA:
        return ProcAaaMsg(buf, len);
    case MSG_BBB:
        return ProcBbbMsg(buf, len);
    case MSG_CCC:
        return ProcCccMsg(buf, len);
    default:
        return ERR_UNSUPPORTED_MSG;
    }
}
```

**先保证路径、返回值和控制流正确，再谈风格是否优雅。**
---

<!-- _class: lead -->

# Part 3 — Secure

## 外部输入、缓冲区、内存与格式串
---

# 不可信输入：不只来自网络

在 C 程序里，下面这些都应当视为不可信：

- 网络 / 串口 / 文件 / IPC 输入。
- 外部接口传入的长度、索引、数量。
- 配置文件和数据库字段。
- 由不可信字段计算出来的派生值。

**原则：外部输入不能直接变成长度、索引、分配大小或格式串。**
---

# 安全函数：关键不是 `_s`，而是三个长度

使用内存/字符串函数时，至少要区分：

| 长度 | 含义 |
|---|---|
| Destination capacity | 目标对象真实容量 |
| Source available length | 源数据实际可用长度 |
| Requested copy length | 本次请求复制长度 |

`memcpy_s`、`strncpy_s` 不是护身符。  
`destMax` 传错，一样可能留下安全问题。
---

# Case 1：缓冲区溢出（外部长度）

```c
uint16_t payloadLen = ReadU16(buf + 2);

memcpy(dst, buf + 4, payloadLen);
```

问题：

- `payloadLen` 来自外部报文。
- 没有证明 `buf` 中真的有这么多字节。
- 没有证明 `dst` 的真实容量足够。

修复原则：同时验证 **报文实际长度** 和 **目标对象容量**。
---

# Case 2：`destMax` 不能来自协议字段

危险：

```c
memcpy_s(dst, packetLen, src, copyLen);
```

如果 `packetLen` 是报文里声明的长度，它不是 `dst` 的真实容量。

更合理：

```c
memcpy_s(dst, sizeof(dst), src, copyLen);
```

或者把真实容量作为接口参数传入：

```c
int Decode(uint8_t *dst, size_t dstCap,
           const uint8_t *src, size_t srcLen);
```
---

# Case 3：索引校验不仅看最终值，还要看累计过程

```c
index += itemCount;

if (index < MAX_ITEM_COUNT) {
    table[index] = value;
}
```

风险：

- `index += itemCount` 本身可能已经溢出。
- `itemCount` 可能来自外部输入。
- 只校验最终 `index` 不足以证明累计过程安全。

更好的思路：先证明 `itemCount <= MAX - index`，再做加法。
---

# Case 4：长度算术要先证明，再计算

危险模式：

```c
size_t total = count * sizeof(Item);
Item *items = malloc(total);
```

如果 `count` 来自外部输入：

- `count * sizeof(Item)` 可能溢出。
- 分配到的内存比预期小。
- 后续按 `count` 写入时发生越界。

修复原则：

```c
if (count > SIZE_MAX / sizeof(Item)) {
    return ERR_OVERFLOW;
}
```
---

# Case 5：窄化转换的检查必须发生在转换前

危险：

```c
uint32_t input = ReadU32(buf);
uint16_t value = (uint16_t)input;

if (value > MAX_VALUE) {
    return ERR_INVALID;
}
```

问题：

- 转换已经丢失高位。
- 后面的检查检查的是截断后的值。
- 攻击者可以利用截断绕过业务边界。

正确顺序：先检查 `input`，再转换。
---

# Case 6：资源所有权必须清晰

危险：

```c
char *p = malloc(size);
if (DoStep1(p) != 0) {
    return ERR_STEP1;
}
if (DoStep2(p) != 0) {
    return ERR_STEP2;
}
free(p);
return OK;
```

问题：

- 失败路径泄露内存。
- 每个 return 都需要考虑资源释放。
- 所有权和生命周期没有显式设计。
---

# Case 7：内存分配不仅要防溢出，还要防资源耗尽

```c
uint32_t count = ReadU32(buf);
Item *items = calloc(count, sizeof(Item));
```

即使没有整数溢出，也要考虑：

- `count` 是否有业务上限。
- 总分配大小是否有模块上限。
- 失败时是否能安全返回。
- 是否存在反复触发的大量分配。
---

# Case 8：Format String 是控制语义，不是普通数据

危险：

```c
printf(userInput);
```

正确：

```c
printf("%s", userInput);
```

原因：

- 格式串决定了后续参数如何解释。
- 外部输入不能控制 `%x`、`%s`、`%n` 等格式语义。
- 日志函数、错误打印函数同样要注意。
---

# Case：让 `RecvMsgProc` 具备安全边界

```c
int RecvMsgProc(const uint8_t *buf, size_t len)
{
    MsgHeader header;

    if (buf == NULL || len < sizeof(header)) {
        return ERR_INVALID_ARG;
    }

    if (DecodeHeader(buf, len, &header) != OK) {
        return ERR_BAD_HEADER;
    }

    if (header.payloadLen > len - sizeof(header)) {
        return ERR_TRUNCATED_PACKET;
    }

    return DispatchMessage(&header,
                           buf + sizeof(header),
                           header.payloadLen);
}
```

**外部长度必须和实际缓冲区长度交叉验证。**
---

<!-- _class: lead -->

# Part 4 — Code Review Lab

## 从“找违反规范”到“解释工程风险”
---

# Code Review 不要只报“违反第几条”

低价值：

> 这里违反了命名规范。

高价值：

> `len` 是外部报文字段，但后续直接作为 copy length 使用。  
> 如果报文字段大于实际缓冲区剩余长度，会触发越界读；  
> 应该先验证 `payloadLen <= recvLen - headerLen`。

Review 输出建议包含：

**Severity → Mechanism → Trigger → Impact → Fix**
---

# Lab：用同一张表 Review 一段代码

| 字段 | 要求 |
|---|---|
| Severity | MUST / SHOULD / STYLE |
| Mechanism | 出错机制 |
| Trigger | 触发条件 |
| Impact | 后果 |
| Fix | 修复方案 |

练习时不要只圈代码，要写出完整判断链。
---

# Final Case：最终版本应该具备什么特征？

```c
int RecvMsgProc(const uint8_t *buf, size_t len)
{
    MsgHeader header;

    int ret = ValidateAndDecodeHeader(buf, len, &header);
    if (ret != OK) {
        return ret;
    }

    if (header.payloadLen > len - sizeof(header)) {
        return ERR_TRUNCATED_PACKET;
    }

    return DispatchMessage(&header,
                           buf + sizeof(header),
                           header.payloadLen);
}
```

检查点：命名清晰、控制流明确、返回路径定义、长度验证完整、分发职责单一。
---

# AI 对编程规范的影响：规范更重要了

AI 会改变写代码的速度，但不会自动保证代码质量。

- 代码生成更快，Review 压力更大。
- 代码看起来更“规范”，但语义可能仍然错误。
- 风格类规则更应该工具化和配置化。
- 安全与边界类规则更需要明确的检查清单。

**AI 时代，规范要从“人读的条文”升级为“人和机器都能执行的约束”。**
---

# AI 生成 C 代码的典型风险

AI 生成的 C 代码常见风险不是“不像人写的”，而是“看起来太像对的”。

| 风险 | 例子 | Review 重点 |
|---|---|---|
| API 幻觉 | 编造不存在的封装函数 | 查接口来源 |
| 边界遗漏 | 只检查 `NULL`，不检查长度 | 查输入约束 |
| 类型隐患 | signed / unsigned 混用 | 查隐式转换 |
| 错误处理弱 | 只返回 `-1` | 查错误语义 |
| 资源路径漏 | 成功路径释放，失败路径泄露 | 查生命周期 |

**AI 可以帮你写第一稿，但不能替你承担工程责任。**
---

# AI 时代，编程规范怎么落地？

把规范分成三类处理：

| 规则类型 | 落地方式 | AI 的作用 |
|---|---|---|
| STYLE | Formatter / Linter | 根据配置批量修复 |
| MUST | Compiler / Static Analyzer / CI | 辅助发现风险模式 |
| SHOULD | Code Review Checklist | 辅助解释设计取舍 |

对 AI 来说，最有用的不是一大段口号，而是：

- 明确的项目规范文件。
- 可运行的检查命令。
- 典型反例和修复示例。
- Review 输出模板。
---

# 使用 AI 做 Code Review：让它先解释机制

不要只问：

```text
这段代码有没有问题？
```

更好的问法：

```text
请按 MUST / SHOULD / STYLE 检查这段 C 代码。
对每个 MUST 问题说明：Mechanism、Trigger、Impact、Fix。
重点检查长度、索引、整数转换、资源释放和错误路径。
```

**AI Review 的价值不在于替你拍板，而在于帮你扩大检查面。**
---

<!-- _class: lead -->

# Part 5 — 工程落地

## 工具、流程与检查清单
---

# 规范 in 开发流程

推荐质量闭环：

```mermaid
flowchart LR
    A[Developer] --> B[Formatter]
    B --> C[Compiler Warnings]
    C --> D[Static Analysis]
    D --> E[Unit Test]
    E --> F[Code Review]
    F --> G[CI Gate]
```

不要把所有问题都推给 Code Review。  
Review 应该处理机器难以判断的语义、边界、职责和架构问题。
---

# 工具边界：没有告警 ≠ 代码正确

工具擅长：

- 格式、简单路径、常见未初始化。
- 常见越界、空指针、资源泄露模式。
- signed / unsigned、窄化转换等风险提示。

工具不擅长：

- 业务语义是否正确。
- 外部输入是否真的可信。
- 资源所有权是否清晰。
- 抽象边界是否合理。
---

# 最终 Checklist：日常 C 代码 Review

1. 控制流是否和缩进、括号一致？
2. 所有返回路径是否定义了明确结果？
3. 外部输入是否直接变成长度、索引、分配大小或格式串？
4. 整数计算是否可能溢出或隐式改变类型？
5. 指针、数组和缓冲区是否有真实容量依据？
6. 资源所有权和释放路径是否清晰？
7. 人工 Review 是否集中在机器难以判断的语义问题上？
---

# 总结：最后记住 5 句话

1. 规范的本质是降低不必要的自由度。
2. STYLE 交给工具，MUST 理解机制。
3. C 语言风险通常藏在边界、类型和生命周期里。
4. 安全函数不等于安全代码，正确长度才是关键。
5. Code Review 要解释工程风险，而不是背诵条文。
---

<!-- _class: lead -->
<!-- _paginate: false -->
<!-- _header: '' -->

# Q&A

## 问题与讨论
