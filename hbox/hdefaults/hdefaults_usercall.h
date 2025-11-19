/***************************************************************
 * Name:      hdefaults_usercall.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_USERCALL_H__
#define __HDEFAULTS_USERCALL_H__
#include "stdint.h"
#include "stdlib.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


enum
{
    HDEFAULTS_USERCALL_NUMBER_RESERVED_SYSCALL_BEGIN=0,                                 /**< 为syscall保留的调用号，一般调用号均从此值开始 */
    HDEFAULTS_USERCALL_NUMBER_RESERVED_SYSCALL_END=1024,                                /**< 为syscall保留的调用号, 常见系统的调用号均不超过此值（一般在500左右）*/
    HDEFAULTS_USERCALL_NUMBER_BEGIN=HDEFAULTS_USERCALL_NUMBER_RESERVED_SYSCALL_END,     /**< 一般调用号均从此值开始 */
    HDEFAULTS_USERCALL_NUMBER_TICK=HDEFAULTS_USERCALL_NUMBER_BEGIN,                     /**< 同hdefaults_tick_get*/
    HDEFAULTS_USERCALL_NUMBER_MALLOC,                                                   /**< 同hdefaults_malloc*/
    HDEFAULTS_USERCALL_NUMBER_FREE,                                                     /**< 同hdefaults_free,注意：返回值类型为int*/
    HDEFAULTS_USERCALL_NUMBER_GLOCK,                                                    /**< 同 hdefaults_mutex_lock,注意：返回值类型为int */
    HDEFAULTS_USERCALL_NUMBER_GUNLOCK,                                                  /**< 同 hdefaults_mutex_unlock,注意：返回值类型为int */
    HDEFAULTS_USERCALL_NUMBER_SYMBOL_FIND,                                              /**< 同 hdefaults_symbol_find */
    HDEFAULTS_USERCALL_NUMBER_END=4096,                                                 /**< 调用号均不超过此值 */
};


/** \brief 默认用户调用返回值检查
 *
 * \param usercall_number uintptr_t 用户调用号
 * \param return_value intptr_t 返回值（注意:这不是用户调用实现的返回值,通常用于找不到相应调用时使用）
 * \return intptr_t 同返回值
 *
 */
intptr_t __hdefaults_usercall_return_check(uintptr_t usercall_number,intptr_t return_value);


/** \brief 用户调用返回值检查(如需使用，需要#undef重新#define)
 *
 *
 */
#define hdefaults_usercall_return_check(usercall_number,return_value) __hdefaults_usercall_return_check(usercall_number,return_value)

/** \brief 用户调用
 *
 * \param usercall_number 用户调用号,见HDEFAULTS_USERCALL_NUMBER_*
 * \param ...             参数（通常第一个参数才是返回值地址（若需要返回值））
 *
 */
#define hdefaults_usercall(usercall_number,...) \
        {\
            const hdefaults_api_table_t *api_table=hdefaults_get_api_table();\
            intptr_t hdefaults_usercall_ret_value=0;\
            if(api_table!=NULL && api_table->usercall!=NULL)\
            {\
                    hdefaults_usercall_ret_value=api_table->usercall((uintptr_t)(usercall_number),##__VA_ARGS__);\
            }\
            hdefaults_usercall_return_check(usercall_number,hdefaults_usercall_ret_value);\
        }

#ifdef __cplusplus
#define HDEFAULTS_USERCALL_DECLARE(NAME) \
        extern "C"\
        {\
            extern intptr_t __hdefaults_usercall_##NAME(uintptr_t number,va_list va_list_args);\
            extern intptr_t hdefaults_usercall_##NAME(uintptr_t number,...);\
        }

#else
#define HDEFAULTS_USERCALL_DECLARE(NAME) \
        extern intptr_t __hdefaults_usercall_##NAME(uintptr_t number,va_list va_list_args);\
        extern intptr_t hdefaults_usercall_##NAME(uintptr_t number,...);
#endif

/*
 * 用于定义参数列表，P表示处理除最后一对参数的参数，E表示处理最后一对参数，K表示键名，V表示键值
 */
#define __HDEFAULTS_MAP1(P,E,K,V)       E(K,V)
#define __HDEFAULTS_MAP2(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP1(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP3(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP2(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP4(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP3(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP5(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP4(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP6(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP5(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP7(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP6(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP8(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP7(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP9(P,E,K,V,...)   P(K,V) __HDEFAULTS_MAP8(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP10(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP9(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP11(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP10(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP12(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP11(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP13(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP12(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP14(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP13(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP15(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP14(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP16(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP15(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP17(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP16(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP18(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP17(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP19(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP18(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP20(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP19(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP21(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP20(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP22(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP21(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP23(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP22(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP24(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP23(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP25(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP24(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP26(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP24(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP27(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP26(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP28(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP27(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP29(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP28(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP30(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP29(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP31(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP30(P,E,##__VA_ARGS__)
#define __HDEFAULTS_MAP32(P,E,K,V,...)  P(K,V) __HDEFAULTS_MAP31(P,E,##__VA_ARGS__)

#define HDEFAULTS_USERCALL_PARAMETER_PROCESS(K,V)       K V,
#define HDEFAULTS_USERCALL_PARAMETER_END(K,V)           K V
#define HDEFAULTS_USERCALL_PARAMETER_CALL_PROCESS(K,V)  V,
#define HDEFAULTS_USERCALL_PARAMETER_CALL_END(K,V)      V
#define HDEFAULTS_USERCALL_VA_LOCAL_PROCESS(K,V)        K V=va_arg(va_list_args,K);
#define HDEFAULTS_USERCALL_VA_LOCAL_END(K,V)            K V=va_arg(va_list_args,K);


#define HDEFAULTS_USERCALL_DEFINEx(X,NAME,NUMBER,RETURN_TYPE,...) \
        HDEFAULTS_USERCALL_DECLARE(NAME)\
        static RETURN_TYPE do_hdefaults_usercall_##NAME(__HDEFAULTS_MAP##X(HDEFAULTS_USERCALL_PARAMETER_PROCESS,HDEFAULTS_USERCALL_PARAMETER_END,__VA_ARGS__));\
        intptr_t __hdefaults_usercall_##NAME(uintptr_t number,va_list va_list_args)\
        {\
            if(number!=(NUMBER))\
            {\
                return -1;\
            }\
            RETURN_TYPE *__hdefaults_usercall_##NAME##_return_ptr=(RETURN_TYPE *)va_arg(va_list_args,RETURN_TYPE *);\
            __HDEFAULTS_MAP##X(HDEFAULTS_USERCALL_VA_LOCAL_PROCESS,HDEFAULTS_USERCALL_VA_LOCAL_END,__VA_ARGS__);\
            RETURN_TYPE __hdefaults_usercall_##NAME##_return_value;\
            __hdefaults_usercall_##NAME##_return_value=do_hdefaults_usercall_##NAME(__HDEFAULTS_MAP##X(HDEFAULTS_USERCALL_PARAMETER_CALL_PROCESS,HDEFAULTS_USERCALL_PARAMETER_CALL_END,##__VA_ARGS__));\
            if(__hdefaults_usercall_##NAME##_return_ptr!=NULL)\
            {\
                (*__hdefaults_usercall_##NAME##_return_ptr)=__hdefaults_usercall_##NAME##_return_value;\
            }\
            return 0;\
        }\
        intptr_t hdefaults_usercall_##NAME(uintptr_t number,...)\
        {\
            va_list va;\
            va_start(va,number);\
            intptr_t ret=__hdefaults_usercall_##NAME(number,va);\
            va_end(va);\
            return ret;\
        }\
        static RETURN_TYPE do_hdefaults_usercall_##NAME(__HDEFAULTS_MAP##X(HDEFAULTS_USERCALL_PARAMETER_PROCESS,HDEFAULTS_USERCALL_PARAMETER_END,##__VA_ARGS__))

#define HDEFAULTS_USERCALL_DEFINE0(NAME,NUMBER,RETURN_TYPE) \
        HDEFAULTS_USERCALL_DECLARE(NAME)\
        static RETURN_TYPE do_hdefaults_usercall_##NAME(void);\
        intptr_t __hdefaults_usercall_##NAME(uintptr_t number,va_list va_list_args)\
        {\
            if(number!=(NUMBER))\
            {\
                return -1;\
            }\
            RETURN_TYPE *__hdefaults_usercall_##NAME##_return_ptr=(RETURN_TYPE *)va_arg(va_list_args,RETURN_TYPE *);\
            RETURN_TYPE __hdefaults_usercall_##NAME##_return_value;\
            __hdefaults_usercall_##NAME##_return_value=do_hdefaults_usercall_##NAME();\
            if(__hdefaults_usercall_##NAME##_return_ptr!=NULL)\
            {\
                (*__hdefaults_usercall_##NAME##_return_ptr)=__hdefaults_usercall_##NAME##_return_value;\
            }\
            return 0;\
        }\
        intptr_t hdefaults_usercall_##NAME(uintptr_t number,...)\
        {\
            va_list va;\
            va_start(va,number);\
            intptr_t ret=__hdefaults_usercall_##NAME(number,va);\
            va_end(va);\
            return ret;\
        }\
        static RETURN_TYPE do_hdefaults_usercall_##NAME(void)


#define HDEFAULTS_USERCALL_DEFINE1(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(1,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE2(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(2,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE3(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(3,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE4(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(4,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE5(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(5,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE6(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(6,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE7(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(7,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE8(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(8,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE9(NAME,NUMBER,RETURN_TYPE,...)     HDEFAULTS_USERCALL_DEFINEx(9,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE10(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(10,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE11(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(11,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE12(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(12,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE13(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(13,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE14(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(14,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE15(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(15,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE16(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(16,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE17(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(17,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE18(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(18,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE19(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(19,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE20(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(20,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE21(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(21,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE22(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(22,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE23(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(23,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE24(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(24,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE25(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(25,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE26(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(26,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE27(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(27,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE28(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(28,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE29(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(29,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE30(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(30,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE31(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(31,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)
#define HDEFAULTS_USERCALL_DEFINE32(NAME,NUMBER,RETURN_TYPE,...)    HDEFAULTS_USERCALL_DEFINEx(32,NAME,NUMBER,RETURN_TYPE,##__VA_ARGS__)



#ifdef __cplusplus
}
#endif // __cplusplus



#endif
