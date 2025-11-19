/***************************************************************
 * Name:      hdefaults_arch.h
 * Purpose:   定义一些默认宏定义
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-11-19
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HDEFAULTS_ARCH_H__
#define __HDEFAULTS_ARCH_H__

//定义硬件架构(支持部分常用架构)
#if defined(_M_IX86) || defined(__i386__) || defined(i386) || defined(__i486__) || defined(i486) || defined(__i586__) || defined(i586) || defined(__i686__) || defined(i686) || defined(__i786__) || defined(i786)
#include "hdefaults_arch_x86.h"
#ifndef HDEFAULTS_ARCH_X86
#define HDEFAULTS_ARCH_X86 1
#endif
#elif defined(_M_X64) || defined(__x86_64) || defined(__x86_64__)
#include "hdefaults_arch_x86_64.h"
#ifndef HDEFAULTS_ARCH_X86_64
#define HDEFAULTS_ARCH_X86_64 1
#endif
#elif defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__)
#include "hdefaults_arch_aarch64.h"
#ifndef HDEFAULTS_ARCH_AARCH64
#define HDEFAULTS_ARCH_AARCH64 1
#endif
#elif defined(_M_ARM) || defined(__arm__) || defined(__ARMEL__)
#include "hdefaults_arch_arm.h"
#ifndef HDEFAULTS_ARCH_ARM
#define HDEFAULTS_ARCH_ARM 1
#endif
/*
 * Thumb指令集,通常用于MCU
 */
#if defined(_M_THUMB) || defined(__thumb__) || defined(__thumb2__)|| defined(__THUMBEL__)
#include "hdefaults_arch_arm_thumb.h"
#ifndef HDEFAULTS_ARCH_ARM_THUMB
#define HDEFAULTS_ARCH_ARM_THUMB 1
#endif
#endif
#elif defined(__riscv)
#include "hdefaults_arch_riscv.h"
#ifndef HDEFAULTS_ARCH_RISCV
#define HDEFAULTS_ARCH_RISCV 1
#endif
#if (__riscv_xlen)==64
#ifndef HDEFAULTS_ARCH_RISCV64
#define HDEFAULTS_ARCH_RISCV64 1
#endif
#endif
#if (__riscv_xlen)==32
#ifndef HDEFAULTS_ARCH_RISCV32
#define HDEFAULTS_ARCH_RISCV32 1
#endif
#endif
#elif defined(__wasm) || defined(__wasm__)
#include "hdefaults_arch_wasm.h"
#ifndef HDEFAULTS_ARCH_WASM
#define HDEFAULTS_ARCH_WASM 1
#endif
/*
 * wasm默认情况下使用wasm32
 */
#if defined(__wasm32) || defined(__wasm32__)
#ifndef HDEFAULTS_ARCH_WASM32
#define HDEFAULTS_ARCH_WASM32 1
#endif
#endif
/*
 * wasm当指定MEMORY64=1时使用wasm64
 */
#if defined(__wasm64) || defined(__wasm64__)
#ifndef HDEFAULTS_ARCH_WASM64
#define HDEFAULTS_ARCH_WASM64 1
#endif
#endif
#endif


#endif
