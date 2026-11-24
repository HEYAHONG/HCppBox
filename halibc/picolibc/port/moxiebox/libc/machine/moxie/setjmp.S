/***************************************************************
 * Name:      setjmp.S
 * Purpose:   实现moxie setjmp/longjmp
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-11-24
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include <picolibc.h>

/*
 * jmp_buf布局(moxie _JBLEN = 10):
 *
 *   0x00  $r6
 *   0x04  $r7
 *   0x08  $r8
 *   0x0c  $r9
 *   0x10  $r10
 *   0x14  $sp
 *   0x18  $fp
 *   0x1c  调用者fp
 *   0x20  调用者ra
 *   0x24  调用者sc
 */

        .text
        .global setjmp
        .type   setjmp,@function
setjmp:
        st.l    ($r0), $r6
        sto.l   0x04($r0), $r7
        sto.l   0x08($r0), $r8
        sto.l   0x0c($r0), $r9
        sto.l   0x10($r0), $r10
        sto.l   0x14($r0), $sp
        sto.l   0x18($r0), $fp

        ldo.l   $r1, 0x00($fp)
        sto.l   0x1c($r0), $r1
        ldo.l   $r1, 0x04($fp)
        sto.l   0x20($r0), $r1
        ldo.l   $r1, 0x08($fp)
        sto.l   0x24($r0), $r1

        xor     $r0, $r0
        ret
.Lsetjmp_end:
        .size   setjmp, .Lsetjmp_end - setjmp

        .global longjmp
        .type   longjmp,@function
longjmp:
        ldo.l   $r6, 0x00($r0)
        ldo.l   $r7, 0x04($r0)
        ldo.l   $r8, 0x08($r0)
        ldo.l   $r9, 0x0c($r0)
        ldo.l   $r10, 0x10($r0)
        ldo.l   $sp, 0x14($r0)
        ldo.l   $fp, 0x18($r0)

        ldo.l   $r2, 0x1c($r0)
        sto.l   0x00($fp), $r2
        ldo.l   $r2, 0x20($r0)
        sto.l   0x04($fp), $r2
        ldo.l   $r2, 0x24($r0)
        sto.l   0x08($fp), $r2

        /*
         * longjmp(env, 0)必须让setjmp返回非0值。
         */
        ldo.l   $r2, 0x08($r0)
        mov     $r0, $r1
        xor     $r2, $r2
        cmp     $r0, $r2
        beq     .Llongjmp_nonzero
        ret
.Llongjmp_nonzero:
        inc     $r0, 1
        ret
.Llongjmp_end:
        .size   longjmp, .Llongjmp_end - longjmp
