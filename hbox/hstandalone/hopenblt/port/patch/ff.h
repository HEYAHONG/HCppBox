#ifndef __PATCH_FF_H__
#define __PATCH_FF_H__

#include "hdefaults.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(HOPENBLT_FILE_NO_IMPLEMENTATION)

typedef enum
{
    FR_OK = 0,              /* (0) Succeeded */
    FR_DISK_ERR,            /* (1) A hard error occurred in the low level disk I/O layer */
    FR_INT_ERR,             /* (2) Assertion failed */
    FR_NOT_READY,           /* (3) The physical drive cannot work */
    FR_NO_FILE,             /* (4) Could not find the file */
    FR_NO_PATH,             /* (5) Could not find the path */
    FR_INVALID_NAME,        /* (6) The path name format is invalid */
    FR_DENIED,              /* (7) Access denied due to prohibited access or directory full */
    FR_EXIST,               /* (8) Access denied due to prohibited access */
    FR_INVALID_OBJECT,      /* (9) The file/directory object is invalid */
    FR_WRITE_PROTECTED,     /* (10) The physical drive is write protected */
    FR_INVALID_DRIVE,       /* (11) The logical drive number is invalid */
    FR_NOT_ENABLED,         /* (12) The volume has no work area */
    FR_NO_FILESYSTEM,       /* (13) There is no valid FAT volume */
    FR_MKFS_ABORTED,        /* (14) The f_mkfs() aborted due to any problem */
    FR_TIMEOUT,             /* (15) Could not get a grant to access the volume within defined period */
    FR_LOCKED,              /* (16) The operation is rejected according to the file sharing policy */
    FR_NOT_ENOUGH_CORE,     /* (17) LFN working buffer could not be allocated */
    FR_TOO_MANY_OPEN_FILES, /* (18) Number of open files > FF_FS_LOCK */
    FR_INVALID_PARAMETER    /* (19) Given parameter is invalid */
} FRESULT;

#ifndef FA_READ
#define FA_READ             0x01
#endif
#ifndef FA_WRITE
#define FA_WRITE            0x02
#endif
#ifndef FA_OPEN_EXISTING
#define FA_OPEN_EXISTING    0x00
#endif
#ifndef FA_CREATE_NEW
#define FA_CREATE_NEW       0x04
#endif
#ifndef FA_CREATE_ALWAYS
#define FA_CREATE_ALWAYS    0x08
#endif
#ifndef FA_OPEN_ALWAYS
#define FA_OPEN_ALWAYS      0x10
#endif
#ifndef FA_OPEN_APPEND
#define FA_OPEN_APPEND      0x30
#endif

typedef struct
{
    uintptr_t ptr[1];
} FATFS;

typedef struct
{
    uintptr_t ptr[1];
} FIL;


FRESULT f_mount (FATFS* fs, const char* path, uint8_t opt);
FRESULT f_open (FIL* fp, const char* path, uint8_t mode);
FRESULT f_close (FIL* fp);                                              /* Close an open file object */
FRESULT f_lseek (FIL* fp, intptr_t ofs);                                /* Move file pointer of the file object */
char*   f_gets (char* buff, intptr_t len, FIL* fp);                     /* Get a string from the file */
int     f_error(FIL* fp);
int     f_eof(FIL* fp);


#else

#include "ff.h"

#endif

#ifdef __cplusplus
}
#endif


#endif

