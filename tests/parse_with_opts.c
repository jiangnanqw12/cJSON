# THA1 详细设计

# 1 需求描述

【需求背景】CLS 子系统需要通过以太网接入 THA1 隔振器。THA1 仪器作为自定义 TCP 协议的 Client，GPB 上的 CLS 作为 TCP Server 监听设备连接。业务进程通过 Sensor Framework 的统一传感器接口完成设备打开、控制参数下发、状态读取和采样数据读取；CLS 内部负责配置解析、定长帧编解码、TCP Server 生命周期和最新上传帧缓存。

【Actor】分控业务进程、Sensor Framework、CLS LTSensor、THA1 设备；离线联调时包含 `eth_tha1_sim` 仿真器。

【周边依赖】Sensor Framework、`EthDeviceListCfg_CLS_CP.json`、TCP/IP 协议栈、`cls_ltsensor_tcp_server` 传输层和 THA1 自定义 TCP 协议。

【输入】传感器 ID、配置项 ID、配置数据、输出缓冲区和超时时间；设备侧输入为连接请求及 1176 字节上传帧。

【处理】

1. 从以太网设备配置读取监听地址和端口，初始化并启动 THA1 TCP Server。
2. 将 Sensor Framework 的统一操作路由到 THA1 适配逻辑，管理 CLS 进程内全局 Server 实例。
3. 接受设备连接，定长接收上传帧，校验帧头、长度和功能码后解析状态、增益、采样率及 11 个采样通道。
4. 将最新成功解析的上传帧加锁缓存，供配置项 Get、`LTSensor_ReadData()` 和 `LTSensor_ReadMiscData()` 读取。
5. 校验开关值和增益范围，将 Set 请求编码为 12 字节 Little-endian 下传控制帧，并发送到当前连接设备。
6. 在超时、断链、坏帧和发送失败时关闭当前 client socket，继续等待下一次连接或返回对应错误码。

【输出】监听和连接状态、配置读取结果、控制下发结果、默认采样值、完整上传帧以及错误码。

【使用限制】

1. 当前仅支持 `CLS_SENSOR_THA1`（3450）和一台已连接设备；新的连接会替换已有 client socket。
2. 上传帧必须为 1176 字节，帧头为 `0x7FEF`、长度字段为 `0x0492`、功能码为 `0x1000`。
3. 下传控制帧固定为 12 字节；开关值仅允许 `0` 或 `1`，垂直和水平增益范围为 `0..500`。
4. 配置项 Get（连接状态除外）和数据读取只返回最近一次成功接收的上传帧；未收到上传帧时返回 `SENSOR_CLS_ENODATA`。
5. 连接状态反映当前已接受的 socket，不是心跳；对端异常可能在后续收发发现之前仍显示已连接。

## 开发功能点

1. 支持 THA1 TCP Server 初始化、监听、单设备连接、关闭和断链后的重新接受连接。
2. 支持上传帧的定长接收、Little-endian 解析、格式校验和线程安全的最新帧缓存。
3. 支持减振开关、数据上送开关、垂直增益、水平增益及批量增益控制帧构造和下发。
4. 通过 Sensor Framework 配置项提供设备状态、连接状态、采样率和最近上传的增益状态。
5. 支持 `LTSensor_ReadData()` 返回 X1 通道最后一个采样值，以及 `LTSensor_ReadMiscData()` 返回完整 `EthTha1UploadFrame`。
6. 提供仿真器与真实仪器共用的协议路径，用于离线联调和回归测试。

# 2 软件功能设计

## 2.1 总体功能描述

### 2.1.1 软件分层

```mermaid
flowchart TD
    APP[分控业务进程] --> FW[Sensor Framework]
    FW --> ADAPTER[CLS Sensor 适配层\ncls_sensor.c]
    ADAPTER --> THA1[THA1 业务层\ncls_tha1.c]
    THA1 --> SERVER[TCP Server 传输层\ncls_ltsensor_tcp_server.c]
    SERVER <-->|TCP/IP / 自定义定长帧| DEV[THA1 设备\nTCP Client]
    SIM[eth_tha1_sim] -->|离线联调| SERVER
    CFG[EthDeviceListCfg_CLS_CP.json] --> ADAPTER
    TYPE[cls_sensor_type.h\n配置项和数据结构] --> APP
    TYPE --> ADAPTER
```

各层职责如下：

| 层次 | 主要职责 |
|---|---|
| Sensor Framework | 注册并调用打开、关闭、配置读写和数据读取回调 |
| CLS Sensor 适配层 | 按 `CLS_SENSOR_THA1` 路由请求，管理全局 THA1 Server，检查缓冲区并映射 cfgItem |
| THA1 业务层 | 编解码 THA1 定长帧，维护监听线程、当前 client socket 和最新上传帧 |
| TCP Server 传输层 | 监听、接受连接、定长收发、关闭 socket 和处理超时 |
| THA1 设备/仿真器 | 主动连接 CLS，持续上传采样帧并接收控制帧 |

### 2.1.2 初始化与连接时序

`LTSensor_Open()` 是业务进程的对外入口。Sensor Framework 将请求分发到 `CLS_OpenSensor()`，CLS 适配层再路由至 `CLS_ThaOpenSensor()`。`CLS_EnsureTha1Server()` 解析配置、初始化 `CLS_THA1_Server` 并启动监听线程；此时 Open 成功只表示监听已启动，并不表示 THA1 已连接或已有上传数据。配置读写和数据读取在调用方未显式 Open 时也会触发同一自动初始化路径。

```mermaid
sequenceDiagram
    autonumber
    participant APP as 分控业务进程
    participant FW as Sensor Framework
    participant CLS as CLS Sensor适配层
    participant CFG as ETH配置解析器
    participant THA1 as THA1业务层
    participant TCP as TCP Server
    participant DEV as THA1设备

    APP->>FW: LTSensor_Open(CLS_SENSOR_THA1)
    FW->>CLS: CLS_OpenSensor(CLS_SENSOR_THA1)
    CLS->>CLS: CLS_ThaOpenSensor()
    CLS->>CFG: ParseEthCfg()
    CFG-->>CLS: 第二个BUS_PATH；仅一个时使用首个
    CLS->>THA1: CLS_THA1_ServerInit(host, port, timeout)
    CLS->>THA1: CLS_THA1_ServerOpen()
    THA1->>TCP: Listen(host:port)
    THA1->>THA1: 启动接收线程
    CLS-->>FW: HELF_EOK（监听已启动）
    FW-->>APP: HELF_EOK

    DEV->>TCP: connect()
    TCP-->>THA1: Accept(client_fd)
    THA1->>THA1: 保存当前client_fd

    APP->>FW: LTSensor_GetCfgData/SetCfgData/ReadData
    FW->>CLS: CLS 注册回调
    CLS->>THA1: 读取最新帧或发送控制帧
    THA1-->>CLS: 业务结果
    CLS-->>FW: 业务结果
    FW-->>APP: 业务结果
```

调用方传入的正数 `timeOut` 用作自动初始化的 Server 超时；否则使用默认 `3000 ms`。重复打开已运行的 Server 不会重复创建监听线程。

### 2.1.3 上传与下传帧时序

```mermaid
sequenceDiagram
    autonumber
    participant DEV as THA1设备/仿真器
    participant TCP as TCP Server
    participant THA1 as THA1业务层
    participant CLS as CLS Sensor适配层
    participant APP as 业务进程

    loop 设备数据上送已开启
        DEV->>TCP: 1176字节上传帧
        TCP->>THA1: RecvExact(1176)
        THA1->>THA1: 校验0x7FEF、0x0492、0x1000
        THA1->>THA1: 解析状态、增益、采样率和11×50采样值
        THA1->>THA1: 加锁更新latest_frame和frame_count
    end

    APP->>CLS: LTSensor_GetCfgData / ReadData / ReadMiscData
    CLS->>THA1: ServerReadLatest()
    alt 已接收有效上传帧
        THA1-->>CLS: EthTha1UploadFrame
        CLS-->>APP: cfg值、X1末采样值或完整帧
    else 尚无上传帧
        THA1-->>CLS: HELF_ENODATA
        CLS-->>APP: SENSOR_CLS_ENODATA
    end

    APP->>CLS: LTSensor_SetCfgData(cfgItem, value)
    CLS->>THA1: BuildSwitch/Gain/BatchGainFrame()
    THA1->>THA1: 校验开关或增益范围并写Little-endian帧
    THA1->>TCP: SendAll(12)
    TCP->>DEV: 控制帧
    THA1-->>CLS: HELF_EOK / 发送错误
    CLS-->>APP: 设置结果
```

### 2.1.4 异常处理

| 场景 | 处理方式 |
|---|---|
| 无效传感器 ID、cfgItem、空指针或缓冲区不足 | 不进行网络操作，返回 CLS 参数、空指针或空间错误码 |
| 配置缺失、监听地址无效或端口为 0 | 初始化失败；`LTSensor_Open()` 或自动初始化返回对应错误 |
| 监听或线程启动失败 | 关闭已创建的监听 socket，返回远端 I/O 或 I/O 错误 |
| 上传帧长度、帧头、长度字段或功能码错误 | 记录协议错误，关闭当前 client socket，接收线程继续等待下一次连接 |
| 接收超时 | 不关闭连接，接收线程继续接收 |
| 对端断开或接收失败 | 关闭当前 client socket，接收线程回到 Accept 循环 |
| 下传时无已连接设备 | `CLS_THA1_ServerSendFrame()` 返回无设备，适配层映射为 `SENSOR_CLS_ENODEV` |
| 下传开关值不为 `0/1` 或增益大于 `500` | 不发送网络请求，返回 `SENSOR_CLS_EINVAL` |
| 尚未收到有效上传帧 | 除连接状态外的 Get 和数据读取返回 `SENSOR_CLS_ENODATA` |

## 2.2 接口设计

### 2.2.1 Sensor Framework 对外接口

`CLS_LTSensor_Init()` 向 Sensor Framework 注册 `Sensor_OperateFuncSet`。业务进程通过 `sensor_api.h` 的 `LTSensor_*` 接口访问 THA1，传感器 ID 为 `CLS_SENSOR_THA1`（3450）。下表中的 `CLS_*` 函数是 Framework 注册后调用的 `STATIC` 内部回调，不作为分控直接调用接口。

| 分控公开接口 | CLS 内部回调 | THA1 行为 |
|---|---|---|
| `LTSensor_Open` | `CLS_OpenSensor` → `CLS_ThaOpenSensor` | 解析配置、初始化 Server、开始监听；不等待设备连接 |
| `LTSensor_Close` | `CLS_CloseSensor` → `CLS_ThaCloseSensor` | 停止监听，关闭 client/listen socket，等待线程退出并释放锁 |
| `LTSensor_GetCfgData` | `CLS_GetSensorCfgData` → `CLS_ThaGetSensorCfgData` | 读取连接状态或最近上传帧中的状态、增益、采样率 |
| `LTSensor_SetCfgData` | `CLS_SetSensorCfgData` → `CLS_ThaSetSensorCfgData` | 构造并向当前连接设备发送 12 字节控制帧 |
| `LTSensor_ReadData` | `CLS_ReadSensorData` → `CLS_ThaReadSensorData` | 返回 X1 通道最后一个采样值，转换为 `DOUBLE` |
| `LTSensor_ReadMiscData` | `CLS_ReadMiscSensorData` → `CLS_ThaReadMiscSensorData` | 返回完整 `EthTha1UploadFrame` |
| `LTSensor_RegIntr` / `LTSensor_UnregIntr` | `CLS_RegisterSensorIntr` / `CLS_SensorUnregisterIntr` | 当前 THA1 未实现，返回不支持 |

### 2.2.2 THA1 业务接口

所有接口成功返回 `HELF_EOK`；参数、帧格式、无数据、无连接和 I/O 错误返回相应 CLS 或 HELF 错误码。

| 接口组 | 接口 | 职责 |
|---|---|---|
| 字节序与帧解析 | `CLS_THA1_ReadU16LE()`、`CLS_THA1_ReadI16LE()`、`CLS_THA1_WriteU16LE()`、`CLS_THA1_ParseUploadFrame()` | 读写 Little-endian 字段，校验并解析上传帧 |
| 控制帧构造 | `CLS_THA1_BuildControlFrame()`、`CLS_THA1_BuildSwitchFrame()`、`CLS_THA1_BuildGainFrame()`、`CLS_THA1_BuildBatchGainFrame()` | 校验功能码和值，构造固定 12 字节下传帧 |
| Server 生命周期 | `CLS_THA1_ServerInit()`、`CLS_THA1_ServerOpen()`、`CLS_THA1_ServerClose()` | 初始化状态和互斥锁、监听并创建线程、关闭 socket 并回收线程 |
| 状态与读取 | `CLS_THA1_ServerIsDeviceConnected()`、`CLS_THA1_ServerWaitForDevice()`、`CLS_THA1_ServerReadLatest()`、`CLS_THA1_ServerReadLatestWithCount()` | 查询 socket 状态、等待连接、读取最近帧及帧计数 |
| 下传 | `CLS_THA1_ServerSendFrame()` | 向当前 client socket 完整发送控制帧；未连接时返回无设备 |

### 2.2.3 配置项映射

`RO` 表示仅支持 `LTSensor_GetCfgData()`，`RW` 表示支持 Get 和 Set，`W` 表示仅支持 `LTSensor_SetCfgData()`；访问权限不编码在 C 枚举名称中。除连接状态外，Get 的来源均为最近成功接收的 `EthTha1UploadFrame`。

| 功能 | 权限 | cfgItem | Set 输入 / Get 输出 | 下传功能码或上传字段 | 值与转换规则 |
|---|---|---|---|---|---|
| 减振开关 | RW | `CLS_THA1_CFG_DAMPING_ENABLED` | `UINT16` / `UINT16` | 下传 `0x10F0` data1 / 上传 `status.bit0` | `0`=关闭，`1`=启动 |
| 数据上送开关 | W | `CLS_THA1_CFG_UPLOAD_ENABLED` | `UINT16` / — | 下传 `0x10F1` data1 | `0`=关闭，`1`=开启 |
| 垂直增益 | RW | `CLS_THA1_CFG_VERTICAL_GAIN` | `UINT16` / `UINT16` | 下传 `0x10FC` data1 / 上传 `vertical_gain` | `0..500`；`180` 对应仪器显示 `0.180` |
| 水平增益 | RW | `CLS_THA1_CFG_HORIZONTAL_GAIN` | `UINT16` / `UINT16` | 下传 `0x10FD` data1 / 上传 `horizontal_gain` | `0..500`；`100` 对应仪器显示 `0.100` |
| 批量增益 | RW | `CLS_THA1_CFG_BATCH_GAIN` | `EthTha1GainConfig` / `EthTha1GainConfig` | 下传 `0x1200` data1/data2 / 上传两个增益字段 | 两个增益均为 `0..500` |
| 设备状态 | RO | `CLS_THA1_CFG_STATUS` | — / `UINT16` | 上传 `status` | bit0 为减振状态；其余位保留 |
| 采样率 | RO | `CLS_THA1_CFG_SAMPLE_RATE` | — / `UINT16` | 上传 `sample_rate_hz` | 单位 Hz |
| 连接状态 | RO | `CLS_THA1_CFG_CONNECTION_STATUS` | — / `UINT16` | 当前 client socket | `0`=未连接，`1`=已连接；不依赖上传帧 |

完整的需求追溯、CLI 命令和 11 个采样通道映射见 [需求接口](需求接口.md)。

### 2.2.4 使用场景

| 场景 | 调用序列 | 预期结果 |
|---|---|---|
| 启动并等待设备 | `LTSensor_Open()` → Get `CLS_THA1_CFG_CONNECTION_STATUS` | Open 成功表示已监听；设备连接后状态为 `1` |
| 读取最近上传数据 | 等待设备上传 → `LTSensor_ReadData()` 或 `LTSensor_ReadMiscData()` | 返回 X1 末采样值或完整 11×50 采样帧 |
| 查询设备配置状态 | `LTSensor_GetCfgData()` 读取状态、增益或采样率 | 从最近上传帧提取字段；无帧时返回无数据 |
| 设置减振或增益 | `LTSensor_SetCfgData()` | 校验值并向已连接设备发送控制帧；设备后续上传帧反映实际状态 |
| 设备断开后恢复 | 收发检测到断链 → 设备重新连接 | CLS 关闭旧 socket 并继续 Accept；后续控制和上传恢复 |

## 2.3 数据结构与协议转换

### 2.3.1 结构定义

| 数据结构 | 字段/容量 | 用途 |
|---|---|---|
| `CLS_THA1_ServerConfig` | `host`、`port`、`timeout_ms` | Server 初始化参数 |
| `CLS_THA1_Server` | 监听/客户端 fd、线程、锁、`latest_frame`、`frame_count`、`has_frame` | 全局 THA1 运行状态和最新帧缓存 |
| `EthTha1UploadFrame` | 状态、垂直/水平增益、30 个保留字、采样率、11×50 个 `INT16` 采样、帧尾保留字 | 上传帧的业务表示 |
| `EthTha1GainConfig` | `vertical_gain`、`horizontal_gain` | 批量增益配置项的输入和输出 |

### 2.3.2 帧格式与数据转换

所有多字节字段使用 Little-endian。上传帧和下传帧都以 `0x7FEF` 开始；解析或构造均通过 `CLS_THA1_ReadU16LE()`、`CLS_THA1_ReadI16LE()` 和 `CLS_THA1_WriteU16LE()` 完成。

| 帧 | 固定字段 | 业务负载 |
|---|---|---|
| 上传帧 | 1176 字节；`header=0x7FEF`、`length=0x0492`、`function=0x1000` | `status`、两个增益、30 个保留字、`sample_rate_hz`、11 个通道各 50 个 `INT16` 采样值、帧尾保留字 |
| 下传帧 | 12 字节；`header=0x7FEF`、`length=0x0006` | `function`、`data1`、`data2`、末尾保留字 `0`；功能码为 `0x10F0`、`0x10F1`、`0x10FC`、`0x10FD` 或 `0x1200` |

采样通道按 `EthTha1UploadFrame.samples[channel][sample]` 保存：X1、X2、Y1、Y2、Z1、Z2、Z3、Z4、FX、FY、FZ 分别对应索引 `0..10`。`LTSensor_ReadData()` 读取 `samples[0][49]`，即 X1 的最后一个采样值。

## 2.4 配置文件

### 2.4.1 配置路径与生效规则

THA1 通过 `CLS_LTSENSOR_ETH_ParseEthCfg()` 读取 `EthDeviceListCfg_CLS_CP.json`。当配置中至少有两个 `BUS_PATH` 时，THA1 使用第二个路径（索引 `1`）；仅有一个路径时回退使用首个路径。配置中的 `ServerPort` 映射为 `serverPort`，`ClientIP` 映射为 `clientIP` 并作为监听 IP；端口必须在 `1..65535`。

`LTSensor_Open()`、配置读写和数据读取均可触发 `CLS_EnsureTha1Server()`。已初始化的全局 Server 会被复用；`LTSensor_Close()` 调用后关闭 Server 并清除初始化标志，后续请求会重新解析配置并建立监听。

### 2.4.2 THA1 配置示例

配置文件中应提供 THA1 对应的 `BUS_PATH`，并以设备可连接到的本机监听地址和端口配置 `ClientIP`、`ServerPort`。协议常量的默认监听端口为 `3434`，实际运行以配置文件中的 `ServerPort` 为准。

```json
{
  "DeviceCfgList": [{
    "BUS_PATH": {
      "ETH": 1,
      "ServerIP": "0.0.0.0",
      "ServerPort": 3434,
      "ClientIP": "0.0.0.0"
    }
  }]
}
```

该示例只表达 THA1 使用的地址和端口字段；实际设备配置文件的完整结构及路径顺序以系统配置为准。
