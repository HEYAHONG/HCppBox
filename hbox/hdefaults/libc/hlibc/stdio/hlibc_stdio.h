/***************************************************************
 * Name:      hlibc_stdio.h
 * Purpose:   声明hlibc_stdio接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-02-12
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HHLIBC_STDIO_H__
#define __HHLIBC_STDIO_H__
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


int hlibc_getchar(void);
int hlibc_putchar(int ch);

struct hfile;
typedef struct hfile hfile_t;

hfile_t * hlibc_fopen( const char* filename, const char* mode );
size_t hlibc_fwrite(const void*buffer,size_t obj_size,size_t obj_count,hfile_t * fp);
size_t hlibc_fread(void*buffer,size_t obj_size,size_t obj_count,hfile_t * fp);
int hlibc_fclose(hfile_t * fp);

int64_t hlibc_fseek(hfile_t * fp, int64_t offset, int whence);
int64_t hlibc_ftell(hfile_t * fp);
void hlibc_rewind(hfile_t * fp);


void hlibc_clearerr(hfile_t *fp);
int hlibc_ferror(hfile_t *fp);
int hlibc_feof(hfile_t *fp);


hfile_t *hlibc_stdin(void);
hfile_t *hlibc_stdout(void);
hfile_t *hlibc_stderr(void);
bool hlibc_is_stdin(void *fp);
bool hlibc_is_stdout(void *fp);
bool hlibc_is_stderr(void *fp);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __HHLIBC_STDIO_H__
