/***************************************************************
 * Name:      hhdefaults_symbol_table.c
 * Purpose:   实现hhdefaults_symbol_table接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-05-07
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hdefaults.h"
#include "hruntime.h"

static const hruntime_symbol_t hruntime_defaults_symbol_table[]=
{
    {
        hdefaults_str(hdefaults_tick_get),(uintptr_t)&hdefaults_tick_get
    },
    {
        hdefaults_str(hdefaults_malloc),(uintptr_t)&hdefaults_malloc
    },
    {
        hdefaults_str(hdefaults_free),(uintptr_t)&hdefaults_free
    },
    {
        hdefaults_str(hdefaults_mutex_lock),(uintptr_t)&hdefaults_mutex_lock
    },
    {
        hdefaults_str(hdefaults_mutex_unlock),(uintptr_t)&hdefaults_mutex_unlock
    },
    {
        hdefaults_str(hdefaults_get_api_table),(uintptr_t)&hdefaults_get_api_table
    },
    {
        hdefaults_str(hdefaults_set_api_table),(uintptr_t)&hdefaults_set_api_table
    },
};

