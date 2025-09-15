#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>


char *__env[1] = { 0 };
char **environ = __env;

#ifdef __PICOLIBC__

__attribute__((weak)) FILE stdin_file={0};
__attribute__((weak)) FILE * const stdin=&stdin_file;

__attribute__((weak)) FILE stdout_file={0};
__attribute__((weak)) FILE * const stdout=&stdout_file;

#endif // __PICOLIBC__

int _getpid(void)
{
    return 1;
}

int _kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}

void _exit (int status)
{
    while (1) {}		/* Make sure we hang here */
}


__attribute__((weak)) int _read(int file, char *ptr, int len)
{

    if(ptr==NULL)
    {
        return 0;
    }

    if(file==0)
    {
        return 0;
    }

    return 0;
}

__attribute__((weak)) int _write(int file, char *ptr, int len)
{
    if(ptr==NULL)
    {
        return 0;
    }

    if(file==1 || file == 2)
    {
        return 0;
    }

    return 0;
}

int _close(int file)
{
    return -1;
}


int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

int _open(char *path, int flags, ...)
{
    /* Pretend like we always fail */
    return -1;
}

int _wait(int *status)
{
    errno = ECHILD;
    return -1;
}

int _unlink(char *name)
{
    errno = ENOENT;
    return -1;
}


int _stat(char *file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _link(char *old, char *new)
{
    errno = EMLINK;
    return -1;
}

int _fork(void)
{
    errno = EAGAIN;
    return -1;
}

int _execve(char *name, char **argv, char **env)
{
    errno = ENOMEM;
    return -1;
}

