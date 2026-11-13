/***************************************************************
 * Name:      hdlt645_di.h
 * Purpose:   声明hdlt645_di接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-09-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDLT645_DI_H_INCLUDED__
#define __HDLT645_DI_H_INCLUDED__

#include "hdlt645_common.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


typedef enum
{
    HDLT645_DI_DATA_DESC_TYPE_XXXXXX_XX=0,
    HDLT645_DI_DATA_DESC_TYPE_XXXXXX_XX_SIGNED,
    HDLT645_DI_DATA_DESC_TYPE_XX_XXXX_YYMMDDHHMM,
    HDLT645_DI_DATA_DESC_TYPE_XX_XXXX_YYMMDDHHMM_SIGNED,
    HDLT645_DI_DATA_DESC_TYPE_XXX_X,
    HDLT645_DI_DATA_DESC_TYPE_XXX_X_SIGNED,
    HDLT645_DI_DATA_DESC_TYPE_XX_XX,
    HDLT645_DI_DATA_DESC_TYPE_XX_XX_SIGNED,
    HDLT645_DI_DATA_DESC_TYPE_X_XXX,
    HDLT645_DI_DATA_DESC_TYPE_X_XXX_SIGNED,
    HDLT645_DI_DATA_DESC_TYPE_XXX_XXX,
    HDLT645_DI_DATA_DESC_TYPE_XXX_XXX_SIGNED,
    HDLT645_DI_DATA_DESC_TYPE_XX_XXXX,
    HDLT645_DI_DATA_DESC_TYPE_XX_XXXX_SIGNED,
    HDLT645_DI_DATA_DESC_TYPE_XXXXXXXX,
    HDLT645_DI_DATA_DESC_TYPE_XXXXXXXX_SIGNED,
    HDLT645_DI_DATA_DESC_TYPE_NN,
    HDLT645_DI_DATA_DESC_TYPE_NNNN,
    HDLT645_DI_DATA_DESC_TYPE_XXXXXX,
    HDLT645_DI_DATA_DESC_TYPE_NNNNNNNNNNNN,
    HDLT645_DI_DATA_DESC_TYPE_YYMMDDWW,
    HDLT645_DI_DATA_DESC_TYPE_HHMMSS,
    HDLT645_DI_DATA_DESC_TYPE_YYMMDDHHMM,
    HDLT645_DI_DATA_DESC_TYPE_MMDDHHMM,
    HDLT645_DI_DATA_DESC_TYPE_DDHH,
    HDLT645_DI_DATA_DESC_TYPE_YYMMDDHHMMSS,
    HDLT645_DI_DATA_DESC_TYPE_MMDDNN,
    HDLT645_DI_DATA_DESC_TYPE_HHMMNN,
    HDLT645_DI_DATA_DESC_TYPE_YYMMDDNN,
    HDLT645_DI_DATA_DESC_TYPE_YYMMDDHHMMNN,
    HDLT645_DI_DATA_DESC_TYPE_ASCII,
    HDLT645_DI_DATA_DESC_TYPE_XXXX,
    HDLT645_DI_DATA_DESC_TYPE_XXXX_XX,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_SUMMARY_ABC,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_SUMMARY,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_PHASE_VOLTAGE,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_PHASE_CURRENT,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_REVERSE,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_IMBALANCE,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_FULL_VOLTAGE_LOSS,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_TWO_TIMES,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_DEMAND_EXCEED,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_VOLTAGE_QUALIFICATION,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_PROGRAMMING,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_METER_CLEAR,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_DEMAND_CLEAR,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_EVENT_CLEAR,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_TIME_SYNC,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_SETTLEMENT,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_TIME_TABLE,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_TIMEZONE,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_WEEKDAY,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_HOLIDAY,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_COMBINATION,
    HDLT645_DI_DATA_DESC_TYPE_EVENT_COVER,
    HDLT645_DI_DATA_DESC_TYPE_FREEZE_VARIABLE,
} hdlt645_di_data_desc_type_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXXXXX_XX
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xxxxxx_xx_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXXXXX_XX_SIGNED
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xxxxxx_xx_signed_t;


/*
 * HDLT645_DI_DATA_DESC_TYPE_XX_XXXX_YYMMDDHHMM
 */
typedef struct
{
    double val;                                             /**< 值 */
    uint8_t YY;                                             /**< 年,只取后两位,数值 */
    uint8_t MM;                                             /**< 月,数值 */
    uint8_t DD;                                             /**< 日,数值 */
    uint8_t HH;                                             /**< 时,数值 */
    uint8_t mm;                                             /**< 分,数值 */
} hdlt645_di_data_desc_type_xx_xxxx_yymmddhhmm_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XX_XXXX_YYMMDDHHMM_SIGNED
 */
typedef struct
{
    double val;                                             /**< 值 */
    uint8_t YY;                                             /**< 年,只取后两位,数值 */
    uint8_t MM;                                             /**< 月,数值 */
    uint8_t DD;                                             /**< 日,数值 */
    uint8_t HH;                                             /**< 时,数值 */
    uint8_t mm;                                             /**< 分,数值 */
} hdlt645_di_data_desc_type_xx_xxxx_yymmddhhmm_signed_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXX_X
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xxx_x_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXX_X_SIGNED
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xxx_x_signed_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XX_XX
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xx_xx_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XX_XX_SIGNED
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xx_xx_signed_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_X_XXX
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_x_xxx_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_X_XXX_SIGNED
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_x_xxx_signed_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXX_XXX
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xxx_xxx_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXX_XXX_SIGNED
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xxx_xxx_signed_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XX_XXXX
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xx_xxxx_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XX_XXXX_SIGNED
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xx_xxxx_signed_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXXXXXXX
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xxxxxxxx_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXXXXXXX_SIGNED
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xxxxxxxx_signed_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_NN
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_nn_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_NNNN
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_nnnn_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXXXXX
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_xxxxxx_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_NNNNNNNNNNNN
 */
typedef struct
{
    double data;
} hdlt645_di_data_desc_type_nnnnnnnnnnnn_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_YYMMDDWW
 */
typedef struct
{
    uint8_t YY;                                             /**< 年,只取后两位,数值 */
    uint8_t MM;                                             /**< 月,数值 */
    uint8_t DD;                                             /**< 日,数值 */
    uint8_t WW;                                             /**< 星期,数值,0代表星期天 */
} hdlt645_di_data_desc_type_yymmddww_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_HHMMSS
 */
typedef struct
{
    uint8_t HH;                                             /**< 时,数值 */
    uint8_t mm;                                             /**< 分,数值 */
    uint8_t ss;                                             /**< 秒,数值 */
} hdlt645_di_data_desc_type_hhmmss_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_YYMMDDHHMM
 */
typedef struct
{
    uint8_t YY;                                             /**< 年,只取后两位,数值 */
    uint8_t MM;                                             /**< 月,数值 */
    uint8_t DD;                                             /**< 日,数值 */
    uint8_t HH;                                             /**< 时,数值 */
    uint8_t mm;                                             /**< 分,数值 */
} hdlt645_di_data_desc_type_yymmddhhmm_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_MMDDHHMM
 */
typedef struct
{
    uint8_t MM;                                             /**< 月,数值 */
    uint8_t DD;                                             /**< 日,数值 */
    uint8_t HH;                                             /**< 时,数值 */
    uint8_t mm;                                             /**< 分,数值 */
} hdlt645_di_data_desc_type_mmddhhmm_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_DDHH
 */
typedef struct
{
    uint8_t DD;                                             /**< 日,数值 */
    uint8_t HH;                                             /**< 时,数值 */
} hdlt645_di_data_desc_type_ddhh_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_YYMMDDHHMMSS
 */
typedef struct
{
    uint8_t YY;                                             /**< 年,只取后两位,数值 */
    uint8_t MM;                                             /**< 月,数值 */
    uint8_t DD;                                             /**< 日,数值 */
    uint8_t HH;                                             /**< 时,数值 */
    uint8_t mm;                                             /**< 分,数值 */
    uint8_t ss;                                             /**< 秒,数值 */
} hdlt645_di_data_desc_type_yymmddhhmmss_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_MMDDNN
 * 单个成员，多个成员由用户自行分离与组装
 */
typedef struct
{
    uint8_t MM;                                             /**< 月,数值 */
    uint8_t DD;                                             /**< 日,数值 */
    uint8_t NN;                                             /**< 日时段表号,数值 */
} hdlt645_di_data_desc_type_mmddnn_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_HHMMNN
 * 单个成员，多个成员由用户自行分离与组装
 */
typedef struct
{
    uint8_t HH;                                             /**< 时,数值 */
    uint8_t mm;                                             /**< 分,数值 */
    uint8_t NN;                                             /**< 费率号,数值 */
} hdlt645_di_data_desc_type_hhmmnn_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_YYMMDDNN
 */
typedef struct
{
    uint8_t YY;                                             /**< 年,只取后两位,数值 */
    uint8_t MM;                                             /**< 月,数值 */
    uint8_t DD;                                             /**< 日,数值 */
    uint8_t NN;                                             /**< 日时段表号,数值 */
} hdlt645_di_data_desc_type_yymmddnn_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_YYMMDDHHMMNN
 */
typedef struct
{
    uint8_t YY;                                             /**< 年,只取后两位,数值 */
    uint8_t MM;                                             /**< 月,数值 */
    uint8_t DD;                                             /**< 日,数值 */
    uint8_t HH;                                             /**< 时,数值 */
    uint8_t mm;                                             /**< 分,数值 */
    uint8_t NN;                                             /**< 记录块数,数值 */
} hdlt645_di_data_desc_type_yymmddhhmmnn_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_ASCII
 */
typedef struct
{
    char data[33];                                          /**< ASCII字符串,最长32字节,以NUL结尾 */
} hdlt645_di_data_desc_type_ascii_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXXX
 */
typedef struct
{
    uint16_t data;                                          /**< 16位状态字/特征字 */
} hdlt645_di_data_desc_type_xxxx_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_XXXX_XX
 */
typedef struct
{
    double data;                                            /**< 两位小数 */
} hdlt645_di_data_desc_type_xxxx_xx_t;

/*
 * 事件记录中的次数、累计时间
 */
typedef struct
{
    hdlt645_di_data_desc_type_xxxxxx_t count;               /**< 次数 */
    hdlt645_di_data_desc_type_xxxxxx_t time;                /**< 累计时间 */
} hdlt645_di_data_desc_type_event_count_time_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_SUMMARY_ABC
 */
typedef struct
{
    hdlt645_di_data_desc_type_event_count_time_t phase[3];  /**< A/B/C相 */
} hdlt645_di_data_desc_type_event_summary_abc_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_SUMMARY
 */
typedef struct
{
    hdlt645_di_data_desc_type_event_count_time_t data;      /**< 总次数、总累计时间 */
} hdlt645_di_data_desc_type_event_summary_t;

/*
 * 事件记录中的电压、电流、功率、功率因数
 */
typedef struct
{
    hdlt645_di_data_desc_type_xxx_x_t voltage;              /**< 电压 */
    hdlt645_di_data_desc_type_xxx_xxx_t current;            /**< 电流 */
    hdlt645_di_data_desc_type_xx_xxxx_t active_power;       /**< 有功功率 */
    hdlt645_di_data_desc_type_xx_xxxx_t reactive_power;     /**< 无功功率 */
    hdlt645_di_data_desc_type_x_xxx_t power_factor;         /**< 功率因数 */
} hdlt645_di_data_desc_type_event_instantaneous_t;

/*
 * 事件记录中单相的电能增量及瞬时量
 */
typedef struct
{
    hdlt645_di_data_desc_type_xx_xxxx_t energy[4];          /**< 该相电能增量 */
    hdlt645_di_data_desc_type_event_instantaneous_t instantaneous;/**< 该相瞬时量 */
} hdlt645_di_data_desc_type_event_phase_energy_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_PHASE_VOLTAGE
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t start;         /**< 发生时刻 */
    hdlt645_di_data_desc_type_yymmddhhmmss_t end;           /**< 结束时刻 */
    hdlt645_di_data_desc_type_xx_xxxx_t total_energy[4];    /**< 总电能增量 */
    hdlt645_di_data_desc_type_event_phase_energy_t phase[3];/**< A/B/C相 */
    hdlt645_di_data_desc_type_xx_xxxx_t ah[4];              /**< 安时数 */
} hdlt645_di_data_desc_type_event_phase_voltage_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_PHASE_CURRENT
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t start;         /**< 发生时刻 */
    hdlt645_di_data_desc_type_yymmddhhmmss_t end;           /**< 结束时刻 */
    hdlt645_di_data_desc_type_xx_xxxx_t total_energy[4];    /**< 总电能增量 */
    hdlt645_di_data_desc_type_event_phase_energy_t phase[3];/**< A/B/C相 */
} hdlt645_di_data_desc_type_event_phase_current_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_REVERSE
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t start;         /**< 发生时刻 */
    hdlt645_di_data_desc_type_yymmddhhmmss_t end;           /**< 结束时刻 */
    hdlt645_di_data_desc_type_xx_xxxx_t energy[16];         /**< 电能增量 */
} hdlt645_di_data_desc_type_event_reverse_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_IMBALANCE
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t start;         /**< 发生时刻 */
    hdlt645_di_data_desc_type_yymmddhhmmss_t end;           /**< 结束时刻 */
    hdlt645_di_data_desc_type_xx_xx_t imbalance;            /**< 最大不平衡率 */
    hdlt645_di_data_desc_type_xx_xxxx_t energy[16];         /**< 电能增量 */
} hdlt645_di_data_desc_type_event_imbalance_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_FULL_VOLTAGE_LOSS
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t start;         /**< 发生时刻 */
    hdlt645_di_data_desc_type_xxx_xxx_t current;            /**< 电流值 */
    hdlt645_di_data_desc_type_yymmddhhmmss_t end;           /**< 结束时刻 */
} hdlt645_di_data_desc_type_event_full_voltage_loss_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_TWO_TIMES
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t start;         /**< 发生时刻 */
    hdlt645_di_data_desc_type_yymmddhhmmss_t end;           /**< 结束时刻 */
} hdlt645_di_data_desc_type_event_two_times_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_DEMAND_EXCEED
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t start;         /**< 发生时刻 */
    hdlt645_di_data_desc_type_yymmddhhmmss_t end;           /**< 结束时刻 */
    hdlt645_di_data_desc_type_xx_xxxx_t demand;             /**< 最大需量 */
    hdlt645_di_data_desc_type_yymmddhhmm_t time;            /**< 发生时间 */
} hdlt645_di_data_desc_type_event_demand_exceed_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_VOLTAGE_QUALIFICATION
 */
typedef struct
{
    hdlt645_di_data_desc_type_xxxxxx_t monitor_time;        /**< 电压监测时间 */
    hdlt645_di_data_desc_type_xxxx_xx_t pass_rate;          /**< 电压合格率 */
    hdlt645_di_data_desc_type_xxxx_xx_t over_limit_rate;    /**< 电压超限率 */
    hdlt645_di_data_desc_type_xxxxxx_t over_upper_time;     /**< 电压超上限时间 */
    hdlt645_di_data_desc_type_xxxxxx_t over_lower_time;     /**< 电压超下限时间 */
    hdlt645_di_data_desc_type_xxx_x_t max_voltage;          /**< 最高电压 */
    hdlt645_di_data_desc_type_mmddhhmm_t max_voltage_time;  /**< 最高电压出现时间 */
    hdlt645_di_data_desc_type_xxx_x_t min_voltage;          /**< 最低电压 */
    hdlt645_di_data_desc_type_mmddhhmm_t min_voltage_time;  /**< 最低电压出现时间 */
} hdlt645_di_data_desc_type_event_voltage_qualification_t;

/*
 * 操作者代码
 */
typedef struct
{
    uint8_t c[4];                                           /**< 操作者代码 */
} hdlt645_di_data_desc_type_operator_t;

/*
 * 数据标识
 */
typedef struct
{
    uint8_t di[4];                                          /**< 数据标识 */
} hdlt645_di_data_desc_type_di_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_PROGRAMMING
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t time;          /**< 发生时刻 */
    hdlt645_di_data_desc_type_operator_t operator_code;     /**< 操作者代码 */
    hdlt645_di_data_desc_type_di_t di[10];                  /**< 编程的前10个数据标识 */
} hdlt645_di_data_desc_type_event_programming_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_METER_CLEAR
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t time;          /**< 发生时刻 */
    hdlt645_di_data_desc_type_operator_t operator_code;     /**< 操作者代码 */
    hdlt645_di_data_desc_type_xx_xxxx_t energy[24];         /**< 电表清零前电能 */
} hdlt645_di_data_desc_type_event_meter_clear_t;

/*
 * 需量清零记录中的数据项
 */
typedef struct
{
    hdlt645_di_data_desc_type_xx_xxxx_t demand;             /**< 最大需量 */
    hdlt645_di_data_desc_type_yymmddhhmm_t time;            /**< 发生时间 */
} hdlt645_di_data_desc_type_event_demand_item_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_DEMAND_CLEAR
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t time;          /**< 发生时刻 */
    hdlt645_di_data_desc_type_operator_t operator_code;     /**< 操作者代码 */
    hdlt645_di_data_desc_type_event_demand_item_t demand[24];/**< 需量清零前最大需量 */
} hdlt645_di_data_desc_type_event_demand_clear_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_EVENT_CLEAR
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t time;          /**< 发生时刻 */
    hdlt645_di_data_desc_type_operator_t operator_code;     /**< 操作者代码 */
    hdlt645_di_data_desc_type_di_t di;                      /**< 事件清零数据标识 */
} hdlt645_di_data_desc_type_event_event_clear_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_TIME_SYNC
 */
typedef struct
{
    hdlt645_di_data_desc_type_operator_t operator_code;     /**< 操作者代码 */
    hdlt645_di_data_desc_type_yymmddhhmmss_t before;        /**< 校时前时间 */
    hdlt645_di_data_desc_type_yymmddhhmmss_t after;         /**< 校时后时间 */
} hdlt645_di_data_desc_type_event_time_sync_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_SETTLEMENT
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t time;          /**< 发生时刻 */
    hdlt645_di_data_desc_type_operator_t operator_code;     /**< 操作者代码 */
    hdlt645_di_data_desc_type_ddhh_t day[3];                /**< 每月第1～3结算日 */
} hdlt645_di_data_desc_type_event_settlement_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_TIME_TABLE
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t time;          /**< 发生时刻 */
    hdlt645_di_data_desc_type_operator_t operator_code;     /**< 操作者代码 */
    hdlt645_di_data_desc_type_hhmmnn_t table[2][8][14];     /**< 两套日时段表 */
} hdlt645_di_data_desc_type_event_time_table_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_TIMEZONE
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t time;          /**< 发生时刻 */
    hdlt645_di_data_desc_type_operator_t operator_code;     /**< 操作者代码 */
    hdlt645_di_data_desc_type_mmddnn_t table[2][14];        /**< 两套时区表 */
} hdlt645_di_data_desc_type_event_timezone_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_WEEKDAY
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t time;          /**< 发生时刻 */
    hdlt645_di_data_desc_type_operator_t operator_code;     /**< 操作者代码 */
    hdlt645_di_data_desc_type_nn_t weekday;                 /**< 周休日采用的日时段表号 */
} hdlt645_di_data_desc_type_event_weekday_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_HOLIDAY
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t time;          /**< 发生时刻 */
    hdlt645_di_data_desc_type_operator_t operator_code;     /**< 操作者代码 */
    hdlt645_di_data_desc_type_yymmddnn_t holiday[254];      /**< 公共假日数据 */
} hdlt645_di_data_desc_type_event_holiday_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_COMBINATION
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t time;          /**< 发生时刻 */
    hdlt645_di_data_desc_type_operator_t operator_code;     /**< 操作者代码 */
    hdlt645_di_data_desc_type_nn_t mode;                    /**< 组合方式特征字 */
} hdlt645_di_data_desc_type_event_combination_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_EVENT_COVER
 */
typedef struct
{
    hdlt645_di_data_desc_type_yymmddhhmmss_t start;         /**< 开表盖时刻 */
    hdlt645_di_data_desc_type_yymmddhhmmss_t end;           /**< 合表盖时刻 */
    hdlt645_di_data_desc_type_xx_xxxx_t energy[12];         /**< 开合表盖前后电能 */
} hdlt645_di_data_desc_type_event_cover_t;

/*
 * HDLT645_DI_DATA_DESC_TYPE_FREEZE_VARIABLE
 */
typedef struct
{
    hdlt645_di_data_desc_type_xx_xxxx_t data[8];            /**< 变量数据 */
} hdlt645_di_data_desc_type_freeze_variable_t;

struct hdlt645_di_data_desc;
typedef struct hdlt645_di_data_desc hdlt645_di_data_desc_t;
struct hdlt645_di_data_desc
{
    const char *format;                                                                                                 /**< 格式自字符串 */
    size_t data_length;                                                                                                 /**< 数据存储长度 */
    hdlt645_di_data_desc_type_t type;                                                                                   /**< 数据对应的格式 */
    bool (*set_data)(const hdlt645_di_data_desc_t *desc,uint8_t *buffer,size_t buffer_size,const void *data);           /**< 设置数据 */
    bool (*get_data)(const hdlt645_di_data_desc_t *desc,const uint8_t *buffer,size_t buffer_size,void *data);           /**< 读取数据 */
};

/** \brief 读取数据标识数据描述
 *
 * \param di hdlt645_data_di_t* 数据标识
 * \return const hdlt645_di_data_desc_t* 数据标识描述,失败返回NULL
 *
 */
const hdlt645_di_data_desc_t * hdlt645_di_data_desc_get(hdlt645_data_di_t *di);


/** \brief 读取数据标识数据描述
 *
 * \param di_num uint32_t 数据标识
 * \return const hdlt645_di_data_desc_t* 数据标识描述,失败返回NULL
 *
 */
const hdlt645_di_data_desc_t * hdlt645_di_data_desc_get2(uint32_t di_num);


/** \brief 读取数据标识数据描述
 *
 * \return const hdlt645_di_data_desc_t** 数据标识描述数组指针，以NULL结尾
 *
 */
const hdlt645_di_data_desc_t *const * hdlt645_di_data_desc_get_all(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDLT645_DI_H_INCLUDED
