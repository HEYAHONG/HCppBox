/***************************************************************
 * Name:      crt0.c
 * Purpose:   实现moxiebox C运行库启动代码(crt0)
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#include <picolibc.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "moxiebox_syscall.h"

struct moxiebox_memory_map_ent {
    uint32_t addr;
    uint32_t length;
    char     tags[24];
};

/*
 * moxiebox memory map.  The layout matches the descriptor table built
 * by the virtual machine and is kept for applications which expect the
 * traditional moxiebox runtime environment.
 */
struct moxiebox_memory_map_ent *moxie_memmap;

extern char __bss_start[];
extern char __bss_end[];
extern char __bss_size[];

#if defined(__INIT_FINI_ARRAY)
extern void __libc_init_array(void);
#endif

int main(int argc, char **argv);

/*
 * The ELF entry point.  This lives in the C file so that the complete
 * runtime, including the system call entry, is provided by the C
 * runtime library.
 */
__asm__(".section .text.startup,\"ax\",@progbits\n"
        ".global _start\n"
        ".type _start,@function\n"
        "_start:\n"
        "xor $fp, $fp\n"
        "gsr $sp, 7\n"
        "jsra __start\n"
        ".size _start, .-_start\n"
        ".text\n");

void
__start(void)
{
    uint32_t memmap = __moxiebox_sreg_read(MOXIEBOX_SREG_MEMMAP);

    memset(__bss_start, 0, (size_t)(__bss_end - __bss_start));
    moxie_memmap = (struct moxiebox_memory_map_ent *)(uintptr_t)memmap;

#if defined(__INIT_FINI_ARRAY)
    __libc_init_array();
#endif

    exit(main(0, NULL));

    for (;;) {
    }
}
