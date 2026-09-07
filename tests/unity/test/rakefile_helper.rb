/**
 * @file cls_sensor_type.h
 * 对外提供CLS子系统传感器标识及数据结构
 * @brief CLS sensor类型对外头文件
 * @version 1.0
 */
#ifndef CLS_SENSOR_TYPE_H
#define CLS_SENSOR_TYPE_H

#include "helf_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/** @brief CLS CCH传感器ID起始值 */
#define CLS_CCH_SENSOR_ID_BASE 3400U
/** @brief CLS THA传感器ID起始值 */
#define CLS_THA_SENSOR_ID_BASE (CLS_CCH_SENSOR_ID_BASE + 50U)

/**
 * @ingroup clssubsys
 * @brief CLS CCH传感器ID枚举
 * @enum CLS_CchSensorId
 */
typedef enum {
    CLS_SENSOR_CCH = CLS_CCH_SENSOR_ID_BASE, /**< \brief CCH共聚焦传感器 */
    CLS_CCH_SENSOR_ID_MAX                   /**< \brief CCH传感器ID最大值 */
} CLS_CchSensorId;

/**
 * @ingroup clssubsys
 * @brief CLS THA传感器ID枚举
 * @enum CLS_ThaSensorId
 */
typedef enum {
    CLS_SENSOR_THA1 = CLS_THA_SENSOR_ID_BASE, /**< \brief THA1传感器 */
    CLS_THA_SENSOR_ID_MAX                     /**< \brief THA传感器ID最大值 */
} CLS_ThaSensorId;

/** @brief CLS传感器ID起始值 */
#define CLS_SENSOR_ID_BASE CLS_CCH_SENSOR_ID_BASE
/** @brief CLS传感器ID最大值 */
#define CLS_SENSOR_ID_MAX CLS_THA_SENSOR_ID_MAX
/** @brief CLS传感器默认通道号 */
#define CLS_SENSOR_DEFAULT_CHANNEL 1U

/**
 * @ingroup clssubsys
 * @brief CLS CCH参数配置项枚举
 * @enum CLS_CchConfigItem
 */
typedef enum {
    CLS_CCH_CFG_CONTROLLER_TEMPERATURE = 0U, /**< \brief 控制器温度，参数类型：EthCchOptionalDouble */
    CLS_CCH_CFG_SAMPLING_INTERVAL,            /**< \brief 采样间隔枚举值，参数类型：CLS_CchSamplingInterval */
    CLS_CCH_CFG_EXTENDED_CHANNEL_PARAMETERS,  /**< \brief 扩展通道参数，参数类型：EthCchExtendedChannelParameters */
    CLS_CCH_CFG_CHANNEL_MEASUREMENT,          /**< \brief 通道测量值，参数类型：EthCchChannelMeasurement */
    CLS_CCH_CFG_ALL_COMMON_MEASUREMENTS,      /**< \brief 全部通用测量值，参数类型：EthCchAllCommonMeasurements */
    CLS_CCH_CFG_CHANNEL_PARAMETERS,           /**< \brief 通道参数，参数类型：EthCchChannelParameters */
    CLS_CCH_CFG_MOVING_AVERAGE_FILTER,        /**< \brief 滑动平均滤波枚举值，参数类型：CLS_CchMovingAverageFilter */
    CLS_CCH_CFG_CACHE_REPORT_COUNT,            /**< \brief 缓存上报数量，参数类型：UINT16 */
    CLS_CCH_CFG_CACHE_LOCK,                    /**< \brief 缓存锁定状态，参数类型：CLS_CchCacheLockState */
    CLS_CCH_CFG_CACHE_CHANNEL_DATA_TYPE,       /**< \brief 缓存通道数据类型，参数类型：EthCchCacheChannelDataTypeConfig */
    CLS_CCH_CFG_CACHE_PARAMETERS,             /**< \brief 缓存参数，参数类型：EthCchCacheParameters */
    CLS_CCH_CFG_CACHE_ENABLED_MASK,            /**< \brief 缓存通道使能位图，参数类型：UINT32 */
    CLS_CCH_CFG_CACHE_WINDOW,                 /**< \brief 缓存窗口，参数类型：EthCchCacheWindow */
    CLS_CCH_CFG_ERROR_HOLD_COUNT,             /**< \brief 错误数据保持次数，参数类型：UINT16 */
    CLS_CCH_CFG_CHANNEL_LIGHT,                 /**< \brief 通道光源状态，参数类型：EthCchChannelLightConfig */
    CLS_CCH_CFG_SAVE_PARAMETERS,               /**< \brief 参数保存命令，无参数 */
    CLS_CCH_CFG_CHANNEL_ZERO_POSITION,         /**< \brief 通道位置清零状态，参数类型：EthCchChannelZeroPositionConfig */
    CLS_CCH_CFG_UPLOAD_IMAGE_TYPE,            /**< \brief 上传图像类型，参数类型：CLS_CchUploadImageType */
    CLS_CCH_CFG_CHANNEL_DARK_CALIBRATION,      /**< \brief 通道暗校准，参数类型：EthCchChannelConfig */
    CLS_CCH_CFG_CACHE_CHANNEL_IMAGE,           /**< \brief 通道图像缓存，参数类型：EthCchChannelConfig */
    CLS_CCH_CFG_CACHE_CHANNEL_DARK_TABLE,      /**< \brief 通道暗校准表缓存，参数类型：EthCchChannelConfig */
    CLS_CCH_CFG_IMAGE_WINDOW,                 /**< \brief 图像窗口，参数类型：EthCchImageWindow */
    CLS_CCH_CFG_THICKNESS_CORRECTION,         /**< \brief 厚度修正参数，参数类型：EthCchThicknessCorrectionConfig */
    CLS_CCH_CFG_CHANNEL_EXPOSURE_MODE,         /**< \brief 通道曝光模式，参数类型：EthCchChannelExposureModeConfig */
    CLS_CCH_CFG_CHANNEL_EXPOSURE_TIME,         /**< \brief 通道曝光时间，参数类型：EthCchChannelExposureConfig */
    CLS_CCH_CFG_CHANNEL_TARGET_EXPOSURE,       /**< \brief 通道目标曝光值，参数类型：EthCchChannelValueConfig */
    CLS_CCH_CFG_THICKNESS_REFRACTIVE_INDEX,   /**< \brief 厚度折射率，参数类型：EthCchThicknessRefractiveIndexConfig */
    CLS_CCH_CFG_CHANNEL_PEAK_SELECTION_MODE,   /**< \brief 通道峰选择模式，参数类型：EthCchChannelPeakSelectionConfig */
    CLS_CCH_CFG_CHANNEL_PEAK_HEIGHT_THRESHOLD, /**< \brief 通道峰高阈值，参数类型：EthCchChannelValueConfig */
    CLS_CCH_CFG_CHANNEL_SHARPNESS_THRESHOLD,   /**< \brief 通道锐度阈值，参数类型：EthCchChannelValueConfig */
    CLS_CCH_CFG_CHANNEL_PEAK_SPACING,          /**< \brief 通道峰间隔，参数类型：EthCchChannelValueConfig */
    CLS_CCH_CFG_CHANNEL_IMAGE_FILTER_WIDTH,    /**< \brief 通道图像滤波宽度，参数类型：EthCchChannelImageFilterConfig */
    CLS_CCH_CFG_CACHE_MATH_DATA_TYPE,          /**< \brief 缓存 MATH 数据类型，参数类型：UINT16 */
    CLS_CCH_CFG_MATH_VALUE,                   /**< \brief MATH 值，参数类型：EthCchMathValue */
    CLS_CCH_CFG_CHANNEL_PEAK_ID,               /**< \brief 通道峰 ID，参数类型：EthCchChannelPeakIdConfig */
    CLS_CCH_CFG_CHANNEL_PEAK_WINDOW,           /**< \brief 通道峰窗口，参数类型：EthCchChannelPeakWindowConfig */
    CLS_CCH_CFG_ID_MAX                           /**< \brief CCH配置项上界，不作为有效配置项使用 */
} CLS_CchConfigItem;

/** CCH 采样间隔枚举值。 */
typedef enum {
    CLS_CCH_SAMPLING_INTERVAL_250_US = 0U,
    CLS_CCH_SAMPLING_INTERVAL_500_US,
    CLS_CCH_SAMPLING_INTERVAL_1_MS,
    CLS_CCH_SAMPLING_INTERVAL_2_MS,
    CLS_CCH_SAMPLING_INTERVAL_5_MS,
    CLS_CCH_SAMPLING_INTERVAL_10_MS,
    CLS_CCH_SAMPLING_INTERVAL_100_US,
    CLS_CCH_SAMPLING_INTERVAL_125_US,
    CLS_CCH_SAMPLING_INTERVAL_160_US,
    CLS_CCH_SAMPLING_INTERVAL_200_US,
    CLS_CCH_SAMPLING_INTERVAL_50_US,
    CLS_CCH_SAMPLING_INTERVAL_55_5_US,
    CLS_CCH_SAMPLING_INTERVAL_62_5_US,
    CLS_CCH_SAMPLING_INTERVAL_66_5_US,
    CLS_CCH_SAMPLING_INTERVAL_80_US,
    CLS_CCH_SAMPLING_INTERVAL_90_5_US,
    CLS_CCH_SAMPLING_INTERVAL_110_US,
    CLS_CCH_SAMPLING_INTERVAL_142_5_US,
    CLS_CCH_SAMPLING_INTERVAL_166_5_US,
    CLS_CCH_SAMPLING_INTERVAL_400_US,
    CLS_CCH_SAMPLING_INTERVAL_4_MS,
    CLS_CCH_SAMPLING_INTERVAL_MAX
} CLS_CchSamplingInterval;

/** CCH 滑动平均滤波次数。 */
typedef enum {
    CLS_CCH_MOVING_AVERAGE_4 = 0U,
    CLS_CCH_MOVING_AVERAGE_16,
    CLS_CCH_MOVING_AVERAGE_64,
    CLS_CCH_MOVING_AVERAGE_256,
    CLS_CCH_MOVING_AVERAGE_1024,
    CLS_CCH_MOVING_AVERAGE_4096,
    CLS_CCH_MOVING_AVERAGE_1,
    CLS_CCH_MOVING_AVERAGE_2,
    CLS_CCH_MOVING_AVERAGE_MAX
} CLS_CchMovingAverageFilter;

/** CCH 缓存锁定状态。 */
typedef enum {
    CLS_CCH_CACHE_UNLOCKED = 0U,
    CLS_CCH_CACHE_LOCKED,
    CLS_CCH_CACHE_LOCK_STATE_MAX
} CLS_CchCacheLockState;

/** CCH 缓存使能目标位标志，可按位或组合。 */
typedef enum {
    CLS_CCH_CACHE_CHANNEL_1 = (1U << 0),
    CLS_CCH_CACHE_CHANNEL_2 = (1U << 1),
    CLS_CCH_CACHE_CHANNEL_3 = (1U << 2),
    CLS_CCH_CACHE_CHANNEL_4 = (1U << 3),
    CLS_CCH_CACHE_CHANNEL_5 = (1U << 4),
    CLS_CCH_CACHE_CHANNEL_6 = (1U << 5),
    CLS_CCH_CACHE_CHANNEL_7 = (1U << 6),
    CLS_CCH_CACHE_CHANNEL_8 = (1U << 7),
    CLS_CCH_CACHE_CHANNEL_9 = (1U << 8),
    CLS_CCH_CACHE_CHANNEL_10 = (1U << 9),
    CLS_CCH_CACHE_CHANNEL_11 = (1U << 10),
    CLS_CCH_CACHE_CHANNEL_12 = (1U << 11),
    CLS_CCH_CACHE_CHANNEL_13 = (1U << 12),
    CLS_CCH_CACHE_CHANNEL_14 = (1U << 13),
    CLS_CCH_CACHE_CHANNEL_15 = (1U << 14),
    CLS_CCH_CACHE_CHANNEL_16 = (1U << 15),
    CLS_CCH_CACHE_CONTROLLER = (1U << 16)
} CLS_CchCacheEnabledTarget;

/** CCH 单通道缓存数据类型位标志，可按位或组合。 */
typedef enum {
    CLS_CCH_CACHE_DATA_DISTANCE_1 = (1U << 0),
    CLS_CCH_CACHE_DATA_DISTANCE_2 = (1U << 1),
    CLS_CCH_CACHE_DATA_THICKNESS_1 = (1U << 2),
    CLS_CCH_CACHE_DATA_DISTANCE_3 = (1U << 3),
    CLS_CCH_CACHE_DATA_DISTANCE_4 = (1U << 4),
    CLS_CCH_CACHE_DATA_DISTANCE_5 = (1U << 5),
    CLS_CCH_CACHE_DATA_DISTANCE_6 = (1U << 6),
    CLS_CCH_CACHE_DATA_THICKNESS_2 = (1U << 7),
    CLS_CCH_CACHE_DATA_THICKNESS_3 = (1U << 8),
    CLS_CCH_CACHE_DATA_THICKNESS_4 = (1U << 9),
    CLS_CCH_CACHE_DATA_THICKNESS_5 = (1U << 10)
} CLS_CchCacheChannelDataType;

/** CCH MATH 缓存数据类型位标志，可按位或组合。 */
typedef enum {
    CLS_CCH_CACHE_MATH_1 = (1U << 0),
    CLS_CCH_CACHE_MATH_2 = (1U << 1),
    CLS_CCH_CACHE_MATH_3 = (1U << 2),
    CLS_CCH_CACHE_MATH_4 = (1U << 3),
    CLS_CCH_CACHE_MATH_5 = (1U << 4),
    CLS_CCH_CACHE_MATH_6 = (1U << 5),
    CLS_CCH_CACHE_MATH_7 = (1U << 6),
    CLS_CCH_CACHE_MATH_8 = (1U << 7),
    CLS_CCH_CACHE_MULTI_POINT_1 = (1U << 8),
    CLS_CCH_CACHE_MULTI_POINT_2 = (1U << 9),
    CLS_CCH_CACHE_MULTI_POINT_3 = (1U << 10),
    CLS_CCH_CACHE_MULTI_POINT_4 = (1U << 11)
} CLS_CchCacheMathDataType;

/** CCH 通道光源状态。 */
typedef enum {
    CLS_CCH_LIGHT_OFF = 0U,
    CLS_CCH_LIGHT_ON,
    CLS_CCH_LIGHT_STATE_MAX
} CLS_CchChannelLightState;

/** CCH 通道位置清零命令。 */
typedef enum {
    CLS_CCH_ZERO_POSITION_CANCEL = 0U,
    CLS_CCH_ZERO_POSITION_EXECUTE,
    CLS_CCH_ZERO_POSITION_COMMAND_MAX
} CLS_CchZeroPositionCommand;

/** CCH 上传图像类型。 */
typedef enum {
    CLS_CCH_UPLOAD_IMAGE_RAW = 0U,
    CLS_CCH_UPLOAD_IMAGE_CALIBRATED,
    CLS_CCH_UPLOAD_IMAGE_SHARPNESS,
    CLS_CCH_UPLOAD_IMAGE_TYPE_MAX
} CLS_CchUploadImageType;

/** CCH 通道曝光模式。 */
typedef enum {
    CLS_CCH_EXPOSURE_MANUAL = 0U,
    CLS_CCH_EXPOSURE_AUTOMATIC,
    CLS_CCH_EXPOSURE_MODE_MAX
} CLS_CchExposureMode;

/** CCH 通道暗校准及图像缓存动作。 */
typedef enum {
    CLS_CCH_CHANNEL_ACTION_DARK_CALIBRATION = 1U,
    CLS_CCH_CHANNEL_ACTION_CACHE_IMAGE,
    CLS_CCH_CHANNEL_ACTION_CACHE_DARK_TABLE,
    CLS_CCH_CHANNEL_ACTION_MAX
} CLS_CchChannelAction;

/** CCH 图像滤波模式。 */
typedef enum {
    CLS_CCH_IMAGE_FILTER_MANUAL = 0U,
    CLS_CCH_IMAGE_FILTER_AUTOMATIC,
    CLS_CCH_IMAGE_FILTER_MODE_MAX
} CLS_CchImageFilterMode;

/** CCH 图像滤波宽度索引。 */
typedef enum {
    CLS_CCH_IMAGE_FILTER_WIDTH_1 = 0U,
    CLS_CCH_IMAGE_FILTER_WIDTH_2,
    CLS_CCH_IMAGE_FILTER_WIDTH_3,
    CLS_CCH_IMAGE_FILTER_WIDTH_5,
    CLS_CCH_IMAGE_FILTER_WIDTH_7,
    CLS_CCH_IMAGE_FILTER_WIDTH_11,
    CLS_CCH_IMAGE_FILTER_WIDTH_15,
    CLS_CCH_IMAGE_FILTER_WIDTH_MAX
} CLS_CchImageFilterWidth;

/** CCH 峰排序模式。 */
typedef enum {
    CLS_CCH_PEAK_SORT_FIRST_6 = 0U,
    CLS_CCH_PEAK_SORT_BY_HEIGHT,
    CLS_CCH_PEAK_SORT_MODE_MAX
} CLS_CchPeakSortingMode;

/** CCH 峰选择模式。 */
typedef enum {
    CLS_CCH_PEAK_SELECTION_BY_ID = 0U,
    CLS_CCH_PEAK_SELECTION_BY_WINDOW,
    CLS_CCH_PEAK_SELECTION_MAXIMUM,
    CLS_CCH_PEAK_SELECTION_LAST,
    CLS_CCH_PEAK_SELECTION_MODE_MAX
} CLS_CchPeakSelectionMode;

/** @brief CCH无效INT32原始数据标志 */
#define ETH_CCH_INVALID_I32_RAW 0x80000000U
/** @brief CCH测量通道数量 */
#define ETH_CCH_CHANNEL_COUNT 16U
/** @brief CCH缓存窗口最大数据个数 */
#define ETH_CCH_CACHE_MAX_VALUES 60U
/** @brief CCH缓存参数支持的通道数据类型数量 */
#define ETH_CCH_CACHE_CHANNEL_TYPE_COUNT 16U
/** @brief CCH图像窗口最大像素数量 */
#define ETH_CCH_IMAGE_WINDOW_MAX_PIXELS 120U
/** @brief CCH厚度系数定点数缩放倍率 */
#define ETH_CCH_THICKNESS_FACTOR_SCALE 1000000.0

/** @brief THA1采样通道数量 */
#define ETH_THA1_CHANNEL_COUNT 11U
/** @brief THA1单通道每帧采样点数量 */
#define ETH_THA1_SAMPLES_PER_CHANNEL 50U
/** @brief THA1默认TCP端口号 */
#define ETH_THA1_DEFAULT_PORT 502U

/**
 * @ingroup clssubsys
 * @brief CLS THA1参数配置项枚举
 * @enum CLS_ThaConfigItem
 */
typedef enum {
    CLS_THA1_CFG_DAMPING_ENABLED = 0U, /**< \brief THA1 阻尼使能，参数类型：CLS_Tha1DampingState */
    CLS_THA1_CFG_SAMPLE_WINDOW_UPDATE_ENABLED, /**< \brief THA1 采样窗口更新使能，参数类型：CLS_Tha1SampleWindowUpdateState */
    CLS_THA1_CFG_VERTICAL_GAIN,        /**< \brief THA1 垂直增益，参数类型：UINT16 */
    CLS_THA1_CFG_HORIZONTAL_GAIN,      /**< \brief THA1 水平增益，参数类型：UINT16 */
    CLS_THA1_CFG_BATCH_GAIN,           /**< \brief THA1 垂直和水平增益，参数类型：EthTha1GainConfig */
    CLS_THA1_CFG_STATUS,            /**< \brief THA1 设备状态，参数类型：UINT16 */
    CLS_THA1_CFG_SAMPLE_RATE,       /**< \brief THA1 采样率，参数类型：UINT16，单位：Hz */
    CLS_THA1_CFG_CONNECTION_STATUS, /**< \brief THA1 设备连接状态，参数类型：CLS_Tha1ConnectionState */
    CLS_THA1_CFG_ID_MAX                /**< \brief THA1配置项上界，不作为有效配置项使用 */
} CLS_ThaConfigItem;

/* 旧名称保留以兼容既有业务源码；其语义已改为采样窗口更新使能。 */
#define CLS_THA1_CFG_UPLOAD_ENABLED CLS_THA1_CFG_SAMPLE_WINDOW_UPDATE_ENABLED

/** THA1 减振开关状态。 */
typedef enum {
    CLS_THA1_DAMPING_DISABLED = 0U,
    CLS_THA1_DAMPING_ENABLED,
    CLS_THA1_DAMPING_STATE_MAX
} CLS_Tha1DampingState;

/** THA1 采样窗口更新使能状态。 */
typedef enum {
    CLS_THA1_SAMPLE_WINDOW_UPDATE_DISABLED = 0U,
    CLS_THA1_SAMPLE_WINDOW_UPDATE_ENABLED,
    CLS_THA1_SAMPLE_WINDOW_UPDATE_STATE_MAX
} CLS_Tha1SampleWindowUpdateState;

/** THA1 设备连接状态。 */
typedef enum {
    CLS_THA1_DISCONNECTED = 0U,
    CLS_THA1_CONNECTED,
    CLS_THA1_CONNECTION_STATE_MAX
} CLS_Tha1ConnectionState;

/**
 * @ingroup clssubsys
 * @brief CCH带有效标志的浮点测量值
 * @struct EthCchOptionalDouble
 */
typedef struct {
    INT32 valid;  /**< \brief 数据有效标志，非0表示有效 */
    DOUBLE value; /**< \brief 测量值 */
} EthCchOptionalDouble;

/**
 * @ingroup clssubsys
 * @brief CCH单通道基础测量数据
 * @struct EthCchChannelMeasurement
 */
typedef struct {
    UINT8 channel;                            /**< \brief 通道号，范围：1至ETH_CCH_CHANNEL_COUNT */
    EthCchOptionalDouble distance_1_mm;       /**< \brief 第1峰距离，单位：mm */
    EthCchOptionalDouble distance_2_mm;       /**< \brief 第2峰距离，单位：mm */
    EthCchOptionalDouble thickness_1_mm;      /**< \brief 第1层厚度，单位：mm */
} EthCchChannelMeasurement;

/**
 * @ingroup clssubsys
 * @brief CCH单通道基础配置参数
 * @struct EthCchChannelParameters
 */
typedef struct {
    UINT8 channel;                  /**< \brief 通道号，范围：1至ETH_CCH_CHANNEL_COUNT */
    CLS_CchChannelLightState light_enabled; /**< \brief 光源使能状态 */
    CLS_CchExposureMode exposure_mode; /**< \brief 曝光模式 */
    UINT16 exposure_time_0_1us;     /**< \brief 曝光时间，单位：0.1 us */
    CLS_CchZeroPositionCommand zero_position; /**< \brief 零点位置 */
    UINT16 dark_calibration;        /**< \brief 暗场校准及缓存动作，取值见CLS_CchChannelAction */
    UINT16 target_exposure;         /**< \brief 目标曝光值 */
    UINT16 peak_height_threshold;   /**< \brief 峰值高度阈值 */
    UINT16 sharpness_threshold;     /**< \brief 锐度阈值 */
    UINT16 peak_spacing;            /**< \brief 峰值间距 */
    CLS_CchImageFilterMode image_filter_mode; /**< \brief 图像滤波模式 */
    CLS_CchImageFilterWidth image_filter_width; /**< \brief 图像滤波宽度 */
} EthCchChannelParameters;

/**
 * @ingroup clssubsys
 * @brief CCH通道选择参数
 * @struct EthCchChannelConfig
 */
typedef struct {
    UINT8 channel; /**< \brief 通道号，范围：1至ETH_CCH_CHANNEL_COUNT */
} EthCchChannelConfig;

/**
 * @ingroup clssubsys
 * @brief CCH通道UINT16配置参数
 * @struct EthCchChannelValueConfig
 */
typedef struct {
    UINT8 channel; /**< \brief 通道号，范围：1至ETH_CCH_CHANNEL_COUNT */
    UINT8 reserved;
    UINT16 value;  /**< \brief 配置值，取值范围由配置项定义 */
} EthCchChannelValueConfig;

typedef struct {
    UINT8 channel;
    UINT16 data_type; /**< \brief 可按位或组合，位定义见CLS_CchCacheChannelDataType */
} EthCchCacheChannelDataTypeConfig;

typedef struct {
    UINT8 channel;
    CLS_CchChannelLightState light_enabled;
} EthCchChannelLightConfig;

typedef struct {
    UINT8 channel;
    CLS_CchZeroPositionCommand zero_position;
} EthCchChannelZeroPositionConfig;

typedef struct {
    UINT8 channel;
    CLS_CchExposureMode exposure_mode;
} EthCchChannelExposureModeConfig;

typedef struct {
    UINT8 channel;
    CLS_CchPeakSelectionMode peak_selection_mode;
} EthCchChannelPeakSelectionConfig;

/**
 * @ingroup clssubsys
 * @brief CCH通道曝光时间配置
 * @struct EthCchChannelExposureConfig
 */
typedef struct {
    UINT8 channel;    /**< \brief 通道号，范围：1至ETH_CCH_CHANNEL_COUNT */
    UINT8 reserved[7];
    DOUBLE exposure_us; /**< \brief 曝光时间，单位：us */
} EthCchChannelExposureConfig;

/**
 * @ingroup clssubsys
 * @brief CCH通道图像滤波宽度配置
 * @struct EthCchChannelImageFilterConfig
 */
typedef struct {
    UINT8 channel;   /**< \brief 通道号，范围：1至ETH_CCH_CHANNEL_COUNT */
    CLS_CchImageFilterMode automatic; /**< \brief 自动滤波使能 */
    CLS_CchImageFilterWidth width_index; /**< \brief 滤波宽度索引 */
} EthCchChannelImageFilterConfig;

/**
 * @ingroup clssubsys
 * @brief CCH单通道多层测量数据
 * @struct EthCchMultiLayerMeasurement
 */
typedef struct {
    UINT8 channel;                                      /**< \brief 通道号，范围：1至ETH_CCH_CHANNEL_COUNT */
    EthCchOptionalDouble distances_3_to_6_mm[4];        /**< \brief 第3至第6峰距离，单位：mm */
    EthCchOptionalDouble thicknesses_2_to_5_mm[4];      /**< \brief 第2至第5层厚度，单位：mm */
    UINT16 peak_heights[6];                             /**< \brief 第1至第6峰高度 */
} EthCchMultiLayerMeasurement;

/**
 * @ingroup clssubsys
 * @brief CCH全部通道基础测量数据
 * @struct EthCchAllCommonMeasurements
 */
typedef struct {
    EthCchChannelMeasurement measurements[ETH_CCH_CHANNEL_COUNT];
} EthCchAllCommonMeasurements;

/**
 * @ingroup clssubsys
 * @brief CCH缓存窗口数据
 * @struct EthCchCacheWindow
 */
typedef struct {
    UINT16 read_count;                                         /**< \brief 读取的数据个数 */
    UINT16 valid_count;                                        /**< \brief 有效数据个数 */
    EthCchOptionalDouble values[ETH_CCH_CACHE_MAX_VALUES];      /**< \brief 缓存测量值 */
} EthCchCacheWindow;

/**
 * @ingroup clssubsys
 * @brief CCH缓存功能配置参数
 * @struct EthCchCacheParameters
 */
typedef struct {
    UINT16 report_count;                                                  /**< \brief 单次缓存上报数据个数 */
    CLS_CchCacheLockState locked;                                        /**< \brief 缓存参数锁定状态 */
    UINT32 enabled_mask;                                                  /**< \brief 通道使能位图，取值见CLS_CchCacheEnabledTarget */
    UINT16 channel_data_types[ETH_CCH_CACHE_CHANNEL_TYPE_COUNT];          /**< \brief 各通道缓存数据类型，取值见CLS_CchCacheChannelDataType */
    UINT16 math_data_type;                                                /**< \brief 数学运算数据类型，取值见CLS_CchCacheMathDataType */
} EthCchCacheParameters;

/**
 * @ingroup clssubsys
 * @brief CCH图像窗口数据
 * @struct EthCchImageWindow
 */
typedef struct {
    UINT16 read_count;                                  /**< \brief 读取的像素个数 */
    UINT16 valid_count;                                 /**< \brief 有效像素个数 */
    UINT16 pixels[ETH_CCH_IMAGE_WINDOW_MAX_PIXELS];     /**< \brief 图像像素数据 */
} EthCchImageWindow;

/**
 * @ingroup clssubsys
 * @brief CCH单通道扩展配置参数
 * @struct EthCchExtendedChannelParameters
 */
typedef struct {
    UINT8 channel;                         /**< \brief 通道号，范围：1至ETH_CCH_CHANNEL_COUNT */
    UINT16 exposure_time_upper_0_1us;      /**< \brief 曝光时间上限，单位：0.1 us */
    UINT16 exposure_time_lower_0_1us;      /**< \brief 曝光时间下限，单位：0.1 us */
    CLS_CchPeakSortingMode peak_sorting_mode; /**< \brief 峰值排序模式 */
    UINT16 max_valid_peak_count;           /**< \brief 最大有效峰值数量 */
    CLS_CchPeakSelectionMode peak_selection_mode; /**< \brief 峰值选择模式 */
    UINT16 peak_1_id;                      /**< \brief 第1个选定峰值ID */
    UINT16 peak_1_window_start;            /**< \brief 第1个峰值搜索窗口起点 */
    UINT16 peak_1_window_end;              /**< \brief 第1个峰值搜索窗口终点 */
    UINT16 peak_2_id;                      /**< \brief 第2个选定峰值ID */
    UINT16 peak_2_window_start;            /**< \brief 第2个峰值搜索窗口起点 */
    UINT16 peak_2_window_end;              /**< \brief 第2个峰值搜索窗口终点 */
} EthCchExtendedChannelParameters;

/**
 * @ingroup clssubsys
 * @brief CCH厚度修正系数配置
 * @struct EthCchThicknessCorrectionConfig
 */
typedef struct {
    UINT8 channel;         /**< \brief 通道号，范围：1至ETH_CCH_CHANNEL_COUNT */
    UINT8 thickness_index; /**< \brief 厚度编号，范围：1至5 */
    UINT8 reserved[6];
    DOUBLE factor;         /**< \brief 厚度修正系数，范围：0至10 */
} EthCchThicknessCorrectionConfig;

/**
 * @ingroup clssubsys
 * @brief CCH厚度折射率表配置
 * @struct EthCchThicknessRefractiveIndexConfig
 */
typedef struct {
    UINT8 channel;         /**< \brief 通道号，范围：1至ETH_CCH_CHANNEL_COUNT */
    UINT8 thickness_index; /**< \brief 厚度编号，范围：1至5 */
    UINT16 table_index;    /**< \brief 折射率表编号，范围：0至16 */
} EthCchThicknessRefractiveIndexConfig;

/**
 * @ingroup clssubsys
 * @brief CCH通道峰编号配置
 * @struct EthCchChannelPeakIdConfig
 */
typedef struct {
    UINT8 channel;   /**< \brief 通道号，范围：1至ETH_CCH_CHANNEL_COUNT */
    UINT8 peak_slot; /**< \brief 峰槽位，取值1或2 */
    UINT16 peak_id;  /**< \brief 峰编号 */
} EthCchChannelPeakIdConfig;

/**
 * @ingroup clssubsys
 * @brief CCH通道峰搜索窗口配置
 * @struct EthCchChannelPeakWindowConfig
 */
typedef struct {
    UINT8 channel;      /**< \brief 通道号，范围：1至ETH_CCH_CHANNEL_COUNT */
    UINT8 peak_slot;    /**< \brief 峰槽位，取值1或2 */
    UINT16 window_start; /**< \brief 峰搜索窗口起点 */
    UINT16 window_end;   /**< \brief 峰搜索窗口终点 */
} EthCchChannelPeakWindowConfig;

/**
 * @ingroup clssubsys
 * @brief CCH MATH测量值
 * @struct EthCchMathValue
 */
typedef struct {
    UINT8 math_index; /**< \brief MATH编号，范围：1至8 */
    UINT8 reserved[7];
    EthCchOptionalDouble value; /**< \brief MATH测量值 */
} EthCchMathValue;

/**
 * @ingroup clssubsys
 * @brief THA1采样窗口数据
 * @struct EthTha1UploadFrame
 */
typedef struct {
    UINT16 status;                                                                    /**< \brief 设备状态，bit0语义见CLS_Tha1DampingState */
    UINT16 vertical_gain;                                                             /**< \brief 垂直增益 */
    UINT16 horizontal_gain;                                                           /**< \brief 水平增益 */
    UINT16 reserved_words[30];                                                        /**< \brief 兼容保留字段，恒为0 */
    UINT16 sample_rate_hz;                                                            /**< \brief 采样率，单位：Hz */
    INT16 samples[ETH_THA1_CHANNEL_COUNT][ETH_THA1_SAMPLES_PER_CHANNEL];              /**< \brief 各通道采样数据 */
    UINT16 tail_reserved;                                                             /**< \brief 兼容保留字段，恒为0 */
} EthTha1UploadFrame;

/**
 * @ingroup clssubsys
 * @brief THA1垂直和水平增益批量配置参数
 * @struct EthTha1GainConfig
 */
typedef struct {
    UINT16 vertical_gain;   /**< \brief 垂直增益 */
    UINT16 horizontal_gain; /**< \brief 水平增益 */
} EthTha1GainConfig;

#ifdef __cplusplus
}
#endif

#endif
