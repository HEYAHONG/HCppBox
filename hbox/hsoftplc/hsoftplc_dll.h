/***************************************************************
 * Name:      hsoftplc_dll.h
 * Purpose:   声明hsoftplc_dll接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-28
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HSOFTPLC_DLL_H__
#define __HSOFTPLC_DLL_H__

#include "hsoftplc.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

struct hsoftplc_public_interface;
typedef struct hsoftplc_public_interface hsoftplc_public_interface_t;                                           /**< 软件PLC接口，用于导出动态链接库 */
struct hsoftplc_public_interface
{
    size_t interface_length;                                                                                    /**< 接口大小，用于校验 */
    void (*interface_init)(void);                                                                               /**< 接口初始化 */
    void (*interface_loop)(void);                                                                               /**< 接口循环 */
    hsoftplc_callback_t (*interface_set_callback)(hsoftplc_callback_t cb);                                      /**< 接口设置回调 */
    size_t (*interface_get_located_all_variables)(hsoftplc_located_variable_enum_callback_t cb,void *usr);      /**< 接口枚举变量 */
};

#define HSOFTPLC_PUBLIC_INTERFACE_INIT_DATA     \
    {\
        sizeof(hsoftplc_public_interface_t),\
        hsoftplc_init,\
        hsoftplc_loop,\
        hsoftplc_set_callback,\
        hsoftplc_get_located_all_variables,\
    }\

#ifdef __cplusplus
#define HSOFTPLC_EXPORT_INTERFACE() \
extern "C" __DLLEXPORT hsoftplc_public_interface_t  hsoftplc_interface;\
__DLLEXPORT hsoftplc_public_interface_t  hsoftplc_interface =\
HSOFTPLC_PUBLIC_INTERFACE_INIT_DATA
#else
#define HSOFTPLC_EXPORT_INTERFACE() \
__DLLEXPORT hsoftplc_public_interface_t hsoftplc_interface =\
HSOFTPLC_PUBLIC_INTERFACE_INIT_DATA
#endif // __cplusplus



#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HSOFTPLC_DLL_H__
