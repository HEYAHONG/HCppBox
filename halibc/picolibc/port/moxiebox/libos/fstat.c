/***************************************************************
 * Name:      fstat.c
 * Purpose:   实现moxiebox fstat接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

int
fstat(int fd, struct stat *statbuf)
{
    if (statbuf == NULL) {
        errno = EINVAL;
        return -1;
    }

    /*
     * Only the three standard streams are known.  moxiebox does not
     * provide a stat system call.
     */
    if (fd < 0 || fd > 2) {
        errno = EBADF;
        return -1;
    }

    memset(statbuf, 0, sizeof(*statbuf));
    statbuf->st_mode = S_IFCHR | 0666;
    statbuf->st_rdev = 0;
    statbuf->st_blksize = 4096;
    return 0;
}
