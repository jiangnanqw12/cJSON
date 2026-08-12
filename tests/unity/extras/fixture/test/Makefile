/**
 * @file mvs_sensor_type.h
 * 对外提供MVS子系统传感器标识、配置项及数据结构
 * @brief MVS sensor 类型定义对外头文件.
 * @version 1.0
 */
#ifndef MVS_SENSOR_TYPE_H
#define MVS_SENSOR_TYPE_H

#include "helf_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/** MVS相机Sensor ID起始值。 */
#define MVS_CAMERA_SENSOR_ID_BASE 3200U
/** MVS光源Sensor ID起始值，与相机Sensor ID间隔50。 */
#define MVS_LIGHT_SENSOR_ID_BASE (MVS_CAMERA_SENSOR_ID_BASE + 50U)

/**
 * @ingroup mvssubsys
 * @brief MVS相机Sensor ID枚举
 * @enum MVS_CameraSensorId
 */
typedef enum {
    MVS_SENSOR_CAMERA = MVS_CAMERA_SENSOR_ID_BASE, /**< \brief BFS-PGE-50S5M相机Sensor ID */
    MVS_CAMERA_SENSOR_ID_MAX                      /**< \brief 相机Sensor ID上界，不作为有效Sensor ID使用 */
} MVS_CameraSensorId;

/**
 * @ingroup mvssubsys
 * @brief MVS光源Sensor ID枚举
 * @enum MVS_LightSensorId
 */
typedef enum {
    MVS_SENSOR_LIGHT = MVS_LIGHT_SENSOR_ID_BASE, /**< \brief 独立以太网光源Sensor ID */
    MVS_LIGHT_SENSOR_ID_MAX                     /**< \brief 光源Sensor ID上界，不作为有效Sensor ID使用 */
} MVS_LightSensorId;

/** MVS子系统Sensor ID上界，不作为有效Sensor ID使用。 */
#define MVS_SENSOR_ID_MAX MVS_LIGHT_SENSOR_ID_MAX

/**
 * @ingroup mvssubsys
 * @brief MVS相机参数配置项枚举
 * @enum MVS_CameraConfigItem
 */
typedef enum {
    MVS_CFG_IMAGE = 0,       /**< \brief 读写MVS_ImageConfig，配置图像尺寸、ROI和像素格式 */
    MVS_CFG_ACQUISITION,     /**< \brief 读写MVS_AcquisitionConfig，配置曝光、增益、binning和帧率 */
    MVS_CFG_CAPTURE_CONTROL, /**< \brief 读写MVS_CaptureControl，开始或停止图像采集 */
    MVS_CFG_STATS,           /**< \brief 只读MVS_CameraStats，获取相机采集及网络统计 */
    MVS_CFG_TRIGGER_MODE,    /**< \brief 读写MVS_TriggerModeConfig，配置软/硬件触发 */
    MVS_CFG_FRAME_RATE,      /**< \brief 读写MVS_FrameRateConfig，配置采集帧率 */
    MVS_CFG_EXPOSURE_TIME,   /**< \brief 读写MVS_ExposureTimeConfig，配置曝光时间 */
    MVS_CFG_ACQUISITION_MODE, /**< \brief 读写MVS_AcquisitionModeConfig，配置采集模式 */
    MVS_CFG_PAYLOAD_SIZE,    /**< \brief 只读MVS_PayloadSizeConfig，获取当前载荷大小 */
    MVS_CFG_ROI,             /**< \brief 读写MVS_RoiConfig，配置ROI */
    MVS_CFG_PACKET_SIZE,     /**< \brief 读写MVS_PacketSizeConfig，配置GVSP包大小 */
    MVS_CFG_PACKET_DELAY,    /**< \brief 读写MVS_PacketDelayConfig，配置GVSP包间延时 */
    MVS_CFG_GAIN,            /**< \brief 读写MVS_GainConfig，配置模拟增益 */
    MVS_CFG_BINNING,         /**< \brief 读写MVS_BinningConfig，配置像素合并模式 */
    MVS_CFG_PIXEL_FORMAT,    /**< \brief 读写MVS_PixelFormatConfig，配置像素格式 */
    MVS_CFG_HEARTBEAT_TIMEOUT, /**< \brief 读写MVS_HeartbeatTimeoutConfig，配置心跳超时 */
    MVS_CFG_TEMPERATURE,      /**< \brief 只读MVS_TemperatureConfig，获取相机温度 */
    MVS_CFG_ACQUISITION_BURST, /**< \brief 读写MVS_AcquisitionBurstConfig，配置缓存连拍帧数 */
    MVS_CFG_IMAGE_BUFFER_INFO, /**< \brief 只读MVS_ImageBufferInfo，获取相机缓存能力 */
    MVS_CFG_STREAM_ENDPOINT,  /**< \brief 只读MVS_StreamEndpointConfig，获取自动管理的stream端点 */
    MVS_CFG_TRIGGER_SOFTWARE, /**< \brief 写MVS_TriggerSoftwareConfig，执行软件触发 */
    MVS_CFG_ID_MAX           /**< \brief 相机配置项上界，不作为有效配置项使用 */
} MVS_CameraConfigItem;

/**
 * @ingroup mvssubsys
 * @brief MVS光源参数配置项枚举
 * @enum MVS_LightConfigItem
 */
typedef enum {
    MVS_LIGHT_CFG_ENABLE = 0, /**< \brief 读写MVS_LightEnableConfig，控制光源开关 */
    MVS_LIGHT_CFG_INTENSITY,  /**< \brief 读写MVS_LightIntensityConfig，光源强度单位为mA */
    MVS_LIGHT_CFG_ID_MAX      /**< \brief 光源配置项上界，不作为有效配置项使用 */
} MVS_LightConfigItem;

/** 8位单色像素格式。 */
#define MVS_PIXEL_FORMAT_MONO8 0U
#define MVS_PIXEL_FORMAT_MONO10P 1U
#define MVS_PIXEL_FORMAT_MONO12P 2U
#define MVS_PIXEL_FORMAT_MONO16 3U
/** 不进行像素合并的1x1 binning模式。 */
#define MVS_BINNING_1X1 1U
/** 水平和垂直方向均进行2像素合并的2x2 binning模式。 */
#define MVS_BINNING_2X2 2U
/** 停止图像采集。 */
#define MVS_CAPTURE_STOP 0U
/** 开始图像采集。 */
#define MVS_CAPTURE_START 1U
/** 关闭相机触发模式。 */
#define MVS_TRIGGER_MODE_OFF 0U
#define MVS_TRIGGER_MODE_ON 1U
#define MVS_TRIGGER_SOURCE_SOFTWARE 0U
#define MVS_TRIGGER_SOURCE_LINE0 1U
#define MVS_TRIGGER_SOURCE_LINE1 2U
#define MVS_TRIGGER_SOURCE_LINE2 3U
#define MVS_TRIGGER_SOURCE_LINE3 4U
/** 连续（自由运行）采集模式。 */
#define MVS_ACQUISITION_MODE_CONTINUOUS 0U
#define MVS_ACQUISITION_MODE_SINGLE_FRAME 1U
#define MVS_ACQUISITION_MODE_MULTI_FRAME 2U

/** MVS支持的相机型号名称。 */
#define MVS_CAMERA_MODEL_NAME "Blackfly S BFS-PGE-50S5M"
/** 相机型号名称缓冲区建议长度，单位为字节。 */
#define MVS_CAMERA_MODEL_NAME_LEN 32U

/**
 * @ingroup mvssubsys
 * @brief MVS相机图像属性配置
 * @struct MVS_ImageConfig
 */
typedef struct {
    UINT32 width;       /**< \brief ROI图像宽度，单位为像素 */
    UINT32 height;      /**< \brief ROI图像高度，单位为像素 */
    UINT32 offsetX;     /**< \brief ROI相对传感器有效区域的水平偏移，单位为像素 */
    UINT32 offsetY;     /**< \brief ROI相对传感器有效区域的垂直偏移，单位为像素 */
    UINT32 pixelFormat; /**< \brief 像素格式，取MVS_PIXEL_FORMAT_MONO*常量 */
} MVS_ImageConfig;

/**
 * @ingroup mvssubsys
 * @brief MVS相机采集参数配置
 * @struct MVS_AcquisitionConfig
 */
typedef struct {
    UINT32 exposureTimeUs; /**< \brief 曝光时间，单位为微秒 */
    FLOAT gainDb;          /**< \brief 模拟增益，单位为dB */
    UINT32 binning;        /**< \brief binning模式，取值为MVS_BINNING_1X1或MVS_BINNING_2X2 */
    FLOAT frameRateFps;    /**< \brief 采集帧率，单位为帧每秒 */
} MVS_AcquisitionConfig;

/**
 * @ingroup mvssubsys
 * @brief MVS相机采集启停控制
 * @struct MVS_CaptureControl
 */
typedef struct {
    UINT32 enable; /**< \brief MVS_CAPTURE_STOP停止采集，MVS_CAPTURE_START开始采集 */
} MVS_CaptureControl;

typedef struct { UINT32 mode; UINT32 source; } MVS_TriggerModeConfig;
typedef struct { FLOAT frameRateFps; } MVS_FrameRateConfig; /**< 单位为fps。 */
typedef struct { UINT32 exposureTimeUs; } MVS_ExposureTimeConfig; /**< 单位为微秒。 */
typedef struct { UINT32 mode; UINT32 frameCount; } MVS_AcquisitionModeConfig;
typedef struct { UINT32 payloadSizeBytes; } MVS_PayloadSizeConfig; /**< 单位为字节，只读。 */
typedef struct { UINT32 width; UINT32 height; UINT32 offsetX; UINT32 offsetY; } MVS_RoiConfig;
typedef struct { UINT32 packetSizeBytes; } MVS_PacketSizeConfig; /**< 单位为字节。 */
typedef struct { FLOAT delayUs; } MVS_PacketDelayConfig; /**< 单位为微秒。 */
typedef struct { FLOAT gainDb; } MVS_GainConfig; /**< 单位为dB。 */
typedef struct { UINT32 binning; } MVS_BinningConfig;
typedef struct { UINT32 pixelFormat; } MVS_PixelFormatConfig;
typedef struct { UINT32 timeoutMs; } MVS_HeartbeatTimeoutConfig; /**< 单位为毫秒。 */
typedef struct { FLOAT temperatureCelsius; } MVS_TemperatureConfig; /**< 只读，单位为摄氏度。 */
typedef struct { UINT32 frameCount; } MVS_AcquisitionBurstConfig;
typedef struct { UINT32 capacityBytes; } MVS_ImageBufferInfo; /**< 只读。 */
typedef struct { UINT32 ipv4Address; UINT16 port; UINT16 reserved; } MVS_StreamEndpointConfig; /**< 只读。 */
typedef struct { UINT32 execute; } MVS_TriggerSoftwareConfig; /**< execute必须为1。 */

/**
 * @ingroup mvssubsys
 * @brief MVS光源开关配置
 * @struct MVS_LightEnableConfig
 */
typedef struct {
    UINT32 enable; /**< \brief 光源开关，0表示关闭，1表示开启 */
} MVS_LightEnableConfig;

/**
 * @ingroup mvssubsys
 * @brief MVS光源强度配置
 * @struct MVS_LightIntensityConfig
 */
typedef struct {
    DOUBLE intensityMa; /**< \brief 光源强度，单位为mA */
} MVS_LightIntensityConfig;

/**
 * @ingroup mvssubsys
 * @brief MVS相机采集及网络统计
 * @struct MVS_CameraStats
 * @note 统计值在相机每次成功Open时清零并重新累计。
 */
typedef struct {
    UINT64 receivedPackets;         /**< \brief 接收到的GVSP数据包总数 */
    UINT64 resendRequests;          /**< \brief 成功发送的GVSP丢包重传请求次数 */
    UINT64 resentPackets;           /**< \brief 发送重传请求后收到的缺失数据包数量 */
    UINT64 lostPackets;             /**< \brief 首次检测到缺失的GVSP数据包数量 */
    UINT64 completedFrames;         /**< \brief 完成组帧并进入帧队列的图像数量 */
    UINT64 incompleteDroppedFrames; /**< \brief 因超时、缺包、格式或尺寸异常丢弃的图像数量 */
    UINT64 overflowDroppedFrames;   /**< \brief 因帧缓冲队列已满丢弃的完整图像数量 */
} MVS_CameraStats;

/**
 * @ingroup mvssubsys
 * @brief MVS相机零拷贝图像描述符
 * @struct MVS_ImageData
 * @note 描述符和data指向的像素数据仅在调用LTSensor_ReadMiscDataFinish前有效。
 */
typedef struct {
    const UINT8 *data; /**< \brief 图像像素数据地址，调用方不得释放或修改 */
    UINT32 dataLen;    /**< \brief 图像像素数据长度，单位为字节 */
    UINT32 width;      /**< \brief 图像有效宽度，单位为像素 */
    UINT32 height;     /**< \brief 图像有效高度，单位为像素 */
    UINT32 stride;     /**< \brief 相邻两行起始地址间隔，单位为字节 */
    UINT32 pixelFormat; /**< \brief 像素格式，当前为MVS_PIXEL_FORMAT_MONO8 */
    UINT64 timestampUs; /**< \brief 软件完成组帧时的单调时钟时间，单位为微秒 */
    UINT16 blockId;     /**< \brief 相机GVSP图像块编号 */
    UINT16 reserved;    /**< \brief 保留字段，当前为0，调用方不得使用 */
} MVS_ImageData;

#ifdef __cplusplus
}
#endif

#endif
