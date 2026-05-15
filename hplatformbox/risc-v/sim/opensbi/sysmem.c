#include <errno.h>
#include <stdint.h>
#include <stddef.h>

/**
 * Pointer to the current high watermark of the heap usage
 */
static uint8_t  __attribute__((unused)) *__sbrk_heap_end = NULL;

/**
 * @brief _sbrk() allocates memory to the newlib heap and is used by malloc
 *        and others from the C library
 *
 * @verbatim
 *
 *
 * @param incr Memory size
 * @return Pointer to allocated memory
 */
void *_sbrk(ptrdiff_t incr)
{
    extern uint8_t __heap_start[];
    extern uint8_t __heap_end[];
    static uint8_t *curbrk=__heap_start;
    if((curbrk+incr) < __heap_start || (curbrk+incr) > __heap_end)
    {
        errno = ENOMEM;
        return (void *)-1;
    }

    curbrk+=incr;
    return curbrk;
}
