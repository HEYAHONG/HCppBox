#include "hbox.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "setjmp.h"
#include "ctype.h"
#include "time.h"
static const hruntime_symbol_t hruntime_libc_symbol[]=
{
    {hdefaults_str(abort),(uintptr_t)&abort},
    {hdefaults_str(free),(uintptr_t)&free},
    {hdefaults_str(getchar),(uintptr_t)&getchar},
    {hdefaults_str(getenv),(uintptr_t)&getenv},
    {hdefaults_str(malloc),(uintptr_t)&malloc},
    {hdefaults_str(memcmp),(uintptr_t)&memcmp},
    {hdefaults_str(memcpy),(uintptr_t)&memcpy},
    {hdefaults_str(memset),(uintptr_t)&memset},
    {hdefaults_str(putchar),(uintptr_t)&putchar},
    {hdefaults_str(realloc),(uintptr_t)&realloc},
    {hdefaults_str(snprintf),(uintptr_t)&snprintf},
    {hdefaults_str(sprintf),(uintptr_t)&sprintf},
    {hdefaults_str(strcat),(uintptr_t)&strcat},
    {hdefaults_str(strcmp),(uintptr_t)&strcmp},
    {hdefaults_str(strcpy),(uintptr_t)&strcpy},
    {hdefaults_str(strlen),(uintptr_t)&strlen},
    {hdefaults_str(strncmp),(uintptr_t)&strncmp},
    {hdefaults_str(strrchr),(uintptr_t)&strrchr},
    {hdefaults_str(strstr),(uintptr_t)&strstr},
    {hdefaults_str(strtod),(uintptr_t)&strtod},
    {hdefaults_str(strtoul),(uintptr_t)&strtoul},
    {hdefaults_str(strtoull),(uintptr_t)&strtoull},
    {hdefaults_str(tolower),(uintptr_t)&tolower},
};
