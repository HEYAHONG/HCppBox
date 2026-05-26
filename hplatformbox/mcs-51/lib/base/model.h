
#if !defined(LIBMONO_BASE_MODEL_H)
#define LIBMONO_BASE_MODEL_H

/*
 * MCS-51模型
 */
#if   defined(__SDCC_MODEL_SMALL)

#elif defined(__SDCC_MODEL_MEDIUM)

/*
 * 默认数据放在pdata
 */
#if !defined(LIBMONO_DATA_ATTRIBUTE)
#define LIBMONO_DATA_ATTRIBUTE __pdata
#endif

#elif defined(__SDCC_MODEL_LARGE)

/*
 * 默认数据放在xdata
 */
#if !defined(LIBMONO_DATA_ATTRIBUTE)
#define LIBMONO_DATA_ATTRIBUTE __xdata
#endif

#elif defined(__SDCC_MODEL_HUGE)

/*
 * 默认数据放在xdata
 */
#if !defined(LIBMONO_DATA_ATTRIBUTE)
#define LIBMONO_DATA_ATTRIBUTE __xdata
#endif

#endif

#endif
