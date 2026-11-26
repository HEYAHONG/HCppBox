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

extern char __data_start[];
extern char __data_end[];

extern uint32_t __data_snapshot_magic[];
extern char     __data_snapshot_data[];

#define MOXIEBOX_DATA_MAGIC 0x4D584442UL

#if defined(__INIT_FINI_ARRAY)
extern void __libc_init_array(void);
#endif

extern void *__moxiebox_memset(void *dst, int c, size_t count);
extern void *__moxiebox_memcpy(void *dst, const void *src, size_t count);

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

/*
 * crt0早期不能依赖C运行库，memset/memcpy使用本地汇编实现。
 */
__asm__(".section .text\n"
        ".global __moxiebox_memset\n"
        ".type __moxiebox_memset,@function\n"
        "__moxiebox_memset:\n"
        "    mov $r3, $r0\n"
        "    xor $r5, $r5\n"
        "    cmp $r2, $r5\n"
        "    beq .Lmoxiebox_memset_done\n"
        "    ldi.l $r4, 0xff\n"
        "    and $r1, $r4\n"
        "    mov $r4, $r1\n"
        "    ldi.l $r5, 8\n"
        "    ashl $r4, $r5\n"
        "    or $r1, $r4\n"
        "    mov $r4, $r1\n"
        "    ldi.l $r5, 16\n"
        "    ashl $r4, $r5\n"
        "    or $r1, $r4\n"
        "    xor $r5, $r5\n"
        "    mov $r4, $r3\n"
        "    ldi.l $r5, 3\n"
        "    and $r4, $r5\n"
        "    xor $r5, $r5\n"
        "    cmp $r4, $r5\n"
        "    beq .Lmoxiebox_memset_words\n"
        "    neg $r4, $r4\n"
        "    ldi.l $r5, 3\n"
        "    and $r4, $r5\n"
        "    xor $r5, $r5\n"
        ".Lmoxiebox_memset_align:\n"
        "    st.b ($r3), $r1\n"
        "    inc $r3, 1\n"
        "    dec $r2, 1\n"
        "    cmp $r2, $r5\n"
        "    beq .Lmoxiebox_memset_done\n"
        "    dec $r4, 1\n"
        "    cmp $r4, $r5\n"
        "    bne .Lmoxiebox_memset_align\n"
        ".Lmoxiebox_memset_words:\n"
        "    mov $r4, $r2\n"
        "    ldi.l $r5, 2\n"
        "    lshr $r4, $r5\n"
        "    xor $r5, $r5\n"
        "    cmp $r4, $r5\n"
        "    beq .Lmoxiebox_memset_tail\n"
        ".Lmoxiebox_memset_word_loop:\n"
        "    st.l ($r3), $r1\n"
        "    inc $r3, 4\n"
        "    dec $r4, 1\n"
        "    cmp $r4, $r5\n"
        "    bne .Lmoxiebox_memset_word_loop\n"
        ".Lmoxiebox_memset_tail:\n"
        "    ldi.l $r5, 3\n"
        "    and $r2, $r5\n"
        "    xor $r5, $r5\n"
        "    cmp $r2, $r5\n"
        "    beq .Lmoxiebox_memset_done\n"
        ".Lmoxiebox_memset_tail_loop:\n"
        "    st.b ($r3), $r1\n"
        "    inc $r3, 1\n"
        "    dec $r2, 1\n"
        "    cmp $r2, $r5\n"
        "    bne .Lmoxiebox_memset_tail_loop\n"
        ".Lmoxiebox_memset_done:\n"
        "    ret\n"
        ".size __moxiebox_memset, .-__moxiebox_memset\n"
        "\n"
        ".global __moxiebox_memcpy\n"
        ".type __moxiebox_memcpy,@function\n"
        "__moxiebox_memcpy:\n"
        "    push $sp, $r6\n"
        "    push $sp, $r7\n"
        "    mov $r3, $r0\n"
        "    xor $r7, $r7\n"
        "    cmp $r2, $r7\n"
        "    beq .Lmoxiebox_memcpy_done\n"
        "    mov $r6, $r0\n"
        "    xor $r6, $r1\n"
        "    ldi.l $r7, 3\n"
        "    and $r6, $r7\n"
        "    xor $r7, $r7\n"
        "    cmp $r6, $r7\n"
        "    bne .Lmoxiebox_memcpy_bytes\n"
        "    mov $r6, $r3\n"
        "    ldi.l $r7, 3\n"
        "    and $r6, $r7\n"
        "    xor $r7, $r7\n"
        "    cmp $r6, $r7\n"
        "    beq .Lmoxiebox_memcpy_words\n"
        "    neg $r6, $r6\n"
        "    ldi.l $r7, 3\n"
        "    and $r6, $r7\n"
        "    xor $r7, $r7\n"
        ".Lmoxiebox_memcpy_align:\n"
        "    ld.b $r4, ($r1)\n"
        "    st.b ($r3), $r4\n"
        "    inc $r3, 1\n"
        "    inc $r1, 1\n"
        "    dec $r2, 1\n"
        "    cmp $r2, $r7\n"
        "    beq .Lmoxiebox_memcpy_done\n"
        "    dec $r6, 1\n"
        "    cmp $r6, $r7\n"
        "    bne .Lmoxiebox_memcpy_align\n"
        ".Lmoxiebox_memcpy_words:\n"
        "    mov $r6, $r2\n"
        "    ldi.l $r7, 2\n"
        "    lshr $r6, $r7\n"
        "    xor $r7, $r7\n"
        "    cmp $r6, $r7\n"
        "    beq .Lmoxiebox_memcpy_tail\n"
        ".Lmoxiebox_memcpy_word_loop:\n"
        "    ld.l $r4, ($r1)\n"
        "    st.l ($r3), $r4\n"
        "    inc $r3, 4\n"
        "    inc $r1, 4\n"
        "    dec $r6, 1\n"
        "    cmp $r6, $r7\n"
        "    bne .Lmoxiebox_memcpy_word_loop\n"
        ".Lmoxiebox_memcpy_tail:\n"
        "    ldi.l $r7, 3\n"
        "    and $r2, $r7\n"
        "    xor $r7, $r7\n"
        "    cmp $r2, $r7\n"
        "    beq .Lmoxiebox_memcpy_done\n"
        ".Lmoxiebox_memcpy_tail_loop:\n"
        "    ld.b $r4, ($r1)\n"
        "    st.b ($r3), $r4\n"
        "    inc $r3, 1\n"
        "    inc $r1, 1\n"
        "    dec $r2, 1\n"
        "    cmp $r2, $r7\n"
        "    bne .Lmoxiebox_memcpy_tail_loop\n"
        "    jmpa .Lmoxiebox_memcpy_done\n"
        ".Lmoxiebox_memcpy_bytes:\n"
        "    ld.b $r4, ($r1)\n"
        "    st.b ($r3), $r4\n"
        "    inc $r3, 1\n"
        "    inc $r1, 1\n"
        "    dec $r2, 1\n"
        "    cmp $r2, $r7\n"
        "    bne .Lmoxiebox_memcpy_bytes\n"
        ".Lmoxiebox_memcpy_done:\n"
        "    pop $sp, $r7\n"
        "    pop $sp, $r6\n"
        "    ret\n"
        ".size __moxiebox_memcpy, .-__moxiebox_memcpy\n"
        ".text\n");

/*
 * moxiebox复位不会重新加载ELF，因此需要在首次启动时保存.data初始值，
 * 复位后从快照恢复。
 */
static void
__moxiebox_data_init(void)
{
    size_t size = (size_t)(__data_end - __data_start);

    if (__data_snapshot_magic[0] != MOXIEBOX_DATA_MAGIC) {
        __moxiebox_memcpy(__data_snapshot_data, __data_start, size);
        __data_snapshot_magic[0] = MOXIEBOX_DATA_MAGIC;
    } else {
        __moxiebox_memcpy(__data_start, __data_snapshot_data, size);
    }
}

void
__start(void)
{
    uint32_t memmap = __moxiebox_sreg_read(MOXIEBOX_SREG_MEMMAP);

    __moxiebox_memset(__bss_start, 0, (size_t)(__bss_end - __bss_start));
    __moxiebox_data_init();
    moxie_memmap = (struct moxiebox_memory_map_ent *)(uintptr_t)memmap;

#if defined(__INIT_FINI_ARRAY)
    __libc_init_array();
#endif

    exit(main(0, NULL));

    for (;;) {
    }
}
