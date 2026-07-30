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
    ETH_CCH_CFG_CONTROLLER_TEMPERATURE_RO = 0U, /**< \brief 获取EthCchOptionalDouble控制器温度 */
    ETH_CCH_CFG_SAMPLING_INTERVAL_RW,            /**< \brief 读写UINT16采样间隔枚举值 */
    ETH_CCH_CFG_EXTENDED_CHANNEL_PARAMETERS_RO,  /**< \brief 获取EthCchExtendedChannelParameters */
    ETH_CCH_CFG_CHANNEL_MEASUREMENT_RO,          /**< \brief 获取EthCchChannelMeasurement */
    ETH_CCH_CFG_ALL_COMMON_MEASUREMENTS_RO,      /**< \brief 获取EthCchAllCommonMeasurements */
    ETH_CCH_CFG_CHANNEL_PARAMETERS_RO,           /**< \brief 获取EthCchChannelParameters */
    ETH_CCH_CFG_MOVING_AVERAGE_FILTER_RW,        /**< \brief 读写UINT16滑动平均滤波枚举值 */
    ETH_CCH_CFG_CACHE_REPORT_COUNT_W,            /**< \brief 设置UINT16缓存上报数量 */
    ETH_CCH_CFG_CACHE_LOCK_W,                    /**< \brief 设置UINT16缓存锁定状态，取值0或1 */
    ETH_CCH_CFG_CACHE_CHANNEL_DATA_TYPE_W,       /**< \brief 设置EthCchChannelValueConfig缓存通道数据类型 */
    ETH_CCH_CFG_CACHE_PARAMETERS_RO,             /**< \brief 获取EthCchCacheParameters */
    ETH_CCH_CFG_CACHE_ENABLED_MASK_W,            /**< \brief 设置UINT32缓存通道使能位图 */
    ETH_CCH_CFG_CACHE_WINDOW_RO,                 /**< \brief 获取EthCchCacheWindow */
    ETH_CCH_CFG_ERROR_HOLD_COUNT_RW,             /**< \brief 读写UINT16错误数据保持次数 */
    ETH_CCH_CFG_CHANNEL_LIGHT_W,                 /**< \brief 设置EthCchChannelValueConfig通道光源状态 */
    ETH_CCH_CFG_SAVE_PARAMETERS_W,               /**< \brief 保存参数，无参数 */
    ETH_CCH_CFG_CHANNEL_ZERO_POSITION_W,         /**< \brief 设置EthCchChannelValueConfig通道位置清零状态 */
    ETH_CCH_CFG_UPLOAD_IMAGE_TYPE_RW,            /**< \brief 读写UINT16上传图像类型 */
    ETH_CCH_CFG_CHANNEL_DARK_CALIBRATION_W,      /**< \brief 执行EthCchChannelConfig通道暗校准 */
    ETH_CCH_CFG_CACHE_CHANNEL_IMAGE_W,           /**< \brief 执行EthCchChannelConfig通道图像缓存 */
    ETH_CCH_CFG_CACHE_CHANNEL_DARK_TABLE_W,      /**< \brief 执行EthCchChannelConfig通道暗校准表缓存 */
    ETH_CCH_CFG_IMAGE_WINDOW_RO,                 /**< \brief 获取EthCchImageWindow */
    ETH_CCH_CFG_THICKNESS_CORRECTION_RW,         /**< \brief 读写EthCchThicknessCorrectionConfig */
    ETH_CCH_CFG_CHANNEL_EXPOSURE_MODE_W,         /**< \brief 设置EthCchChannelValueConfig通道曝光模式 */
    ETH_CCH_CFG_CHANNEL_EXPOSURE_TIME_W,         /**< \brief 设置EthCchChannelExposureConfig通道曝光时间 */
    ETH_CCH_CFG_CHANNEL_TARGET_EXPOSURE_W,       /**< \brief 设置EthCchChannelValueConfig通道目标曝光值 */
    ETH_CCH_CFG_THICKNESS_REFRACTIVE_INDEX_RW,   /**< \brief 读写EthCchThicknessRefractiveIndexConfig */
    ETH_CCH_CFG_CHANNEL_PEAK_SELECTION_MODE_W,   /**< \brief 设置EthCchChannelValueConfig峰选择模式 */
    ETH_CCH_CFG_CHANNEL_PEAK_HEIGHT_THRESHOLD_W, /**< \brief 设置EthCchChannelValueConfig峰高阈值 */
    ETH_CCH_CFG_CHANNEL_SHARPNESS_THRESHOLD_W,   /**< \brief 设置EthCchChannelValueConfig锐度阈值 */
    ETH_CCH_CFG_CHANNEL_PEAK_SPACING_W,          /**< \brief 设置EthCchChannelValueConfig峰间隔 */
    ETH_CCH_CFG_CHANNEL_IMAGE_FILTER_WIDTH_W,    /**< \brief 设置EthCchChannelImageFilterConfig */
    ETH_CCH_CFG_CACHE_MATH_DATA_TYPE_W,          /**< \brief 设置UINT16缓存MATH数据类型 */
    ETH_CCH_CFG_MATH_VALUE_RO,                   /**< \brief 获取EthCchMathValue */
    ETH_CCH_CFG_CHANNEL_PEAK_ID_W,               /**< \brief 设置EthCchChannelPeakIdConfig */
    ETH_CCH_CFG_CHANNEL_PEAK_WINDOW_W,           /**< \brief 设置EthCchChannelPeakWindowConfig */
    ETH_CCH_CFG_ID_MAX                           /**< \brief CCH配置项上界，不作为有效配置项使用 */
} CLS_CchConfigItem;

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
/** @brief THA1上行数据帧长度，单位：字节 */
#define ETH_THA1_UPLOAD_FRAME_SIZE 1176U
/** @brief THA1下行配置帧长度，单位：字节 */
#define ETH_THA1_DOWNLINK_FRAME_SIZE 12U
/** @brief THA1默认TCP端口号 */
#define ETH_THA1_DEFAULT_PORT 3434U

/**
 * @ingroup clssubsys
 * @brief CLS THA1参数配置项枚举
 * @enum CLS_ThaConfigItem
 */
typedef enum {
    ETH_THA1_CFG_DAMPING_ENABLED = 0U, /**< \brief 读写THA1阻尼使能，参数类型：UINT16 */
    ETH_THA1_CFG_UPLOAD_ENABLED,       /**< \brief 只写THA1数据上送使能，参数类型：UINT16 */
    ETH_THA1_CFG_VERTICAL_GAIN,        /**< \brief 读写THA1垂直增益，参数类型：UINT16 */
    ETH_THA1_CFG_HORIZONTAL_GAIN,      /**< \brief 读写THA1水平增益，参数类型：UINT16 */
    ETH_THA1_CFG_BATCH_GAIN,           /**< \brief 读写THA1垂直和水平增益，参数类型：EthTha1GainConfig */
    ETH_THA1_CFG_STATUS_RO,            /**< \brief 获取THA1设备状态，参数类型：UINT16 */
    ETH_THA1_CFG_SAMPLE_RATE_RO,       /**< \brief 获取THA1采样率，参数类型：UINT16，单位：Hz */
    ETH_THA1_CFG_ID_MAX                /**< \brief THA1配置项上界，不作为有效配置项使用 */
} CLS_ThaConfigItem;

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
    UINT16 light_enabled;           /**< \brief 光源使能状态 */
    UINT16 exposure_mode;           /**< \brief 曝光模式 */
    UINT16 exposure_time_0_1us;     /**< \brief 曝光时间，单位：0.1 us */
    UINT16 zero_position;           /**< \brief 零点位置 */
    UINT16 dark_calibration;        /**< \brief 暗场校准参数 */
    UINT16 target_exposure;         /**< \brief 目标曝光值 */
    UINT16 peak_height_threshold;   /**< \brief 峰值高度阈值 */
    UINT16 sharpness_threshold;     /**< \brief 锐度阈值 */
    UINT16 peak_spacing;            /**< \brief 峰值间距 */
    UINT16 image_filter_width;      /**< \brief 图像滤波宽度 */
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
    UINT8 automatic; /**< \brief 自动滤波使能，取值0或1 */
    UINT16 width_index; /**< \brief 滤波宽度枚举值，范围：0至6 */
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
    UINT16 locked;                                                        /**< \brief 缓存参数锁定状态 */
    UINT32 enabled_mask;                                                  /**< \brief 通道使能位图 */
    UINT16 channel_data_types[ETH_CCH_CACHE_CHANNEL_TYPE_COUNT];          /**< \brief 各通道缓存数据类型 */
    UINT16 math_data_type;                                                /**< \brief 数学运算数据类型 */
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
    UINT16 peak_sorting_mode;              /**< \brief 峰值排序模式 */
    UINT16 max_valid_peak_count;           /**< \brief 最大有效峰值数量 */
    UINT16 peak_selection_mode;            /**< \brief 峰值选择模式 */
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
 * @brief THA1上行采样数据帧
 * @struct EthTha1UploadFrame
 */
typedef struct {
    UINT16 status;                                                                    /**< \brief 设备状态 */
    UINT16 vertical_gain;                                                             /**< \brief 垂直增益 */
    UINT16 horizontal_gain;                                                           /**< \brief 水平增益 */
    UINT16 reserved_words[30];                                                        /**< \brief 协议保留字段 */
    UINT16 sample_rate_hz;                                                            /**< \brief 采样率，单位：Hz */
    INT16 samples[ETH_THA1_CHANNEL_COUNT][ETH_THA1_SAMPLES_PER_CHANNEL];              /**< \brief 各通道采样数据 */
    UINT16 tail_reserved;                                                             /**< \brief 帧尾保留字段 */
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
