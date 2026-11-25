ENTRY(_start)

SECTIONS
{
    . = 0x00001000;
    __text_start = .;

    .text : {
        *(.text.startup .text.startup.*)
        *(.text .text.*)
        *(.gnu.linkonce.t.*)

        KEEP (*(.init))
        KEEP (*(.fini))
    }

    . = ALIGN(4);
    __text_end = .;
    PROVIDE (__etext = .);
    PROVIDE (_etext = .);
    PROVIDE (etext = .);

    .rodata : {
        *(.rodata .rodata.*)
        *(.srodata.cst16)
        *(.srodata.cst8)
        *(.srodata.cst4)
        *(.srodata.cst2)
        *(.srodata .srodata.*)
        *(.gnu.linkonce.r.*)
        *(.data.rel.ro .data.rel.ro.*)

        /*
         * HBox支持段。
         */
        . = ALIGN(4);
        PROVIDE (__hshell_command_start = .);
        KEEP (*(SORT(.HShellCommand*)))
        PROVIDE (__hshell_command_end = .);

        . = ALIGN(4);
        PROVIDE (__hruntime_init_start = .);
        KEEP (*(SORT(.HRuntimeInit*)))
        PROVIDE (__hruntime_init_end = .);

        . = ALIGN(4);
        PROVIDE (__hruntime_loop_start = .);
        KEEP (*(SORT(.HRuntimeLoop*)))
        PROVIDE (__hruntime_loop_end = .);

        . = ALIGN(4);
        PROVIDE (__hruntime_symbol_start = .);
        KEEP (*(SORT(.HRuntimeSymbol*)))
        PROVIDE (__hruntime_symbol_end = .);

        *(.gcc_except_table .gcc_except_table.*)
        *(.eh_frame_hdr)
        . = ALIGN(4);
        __eh_frame_start = .;
        KEEP (*(.eh_frame .eh_frame.*))
        . = ALIGN(4);
        LONG (0);
        __eh_frame_end = .;

        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        __init_array_start = .;
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*)
                SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array .ctors))
        __init_array_end = .;

        __fini_array_start = .;
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*)
                SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array .dtors))
        __fini_array_end = .;

        __bothinit_array_start = __preinit_array_start;
        __bothinit_array_end = __init_array_end;
    }

    . = ALIGN(4);
    __data_start = .;
    .data : {
        *(.data .data.*)
        *(.gnu.linkonce.d.*)
        *(.sdata .sdata.*)
    }

    . = ALIGN(4);
    __data_end = .;
    __data_source = LOADADDR(.data);
    __data_size = __data_end - __data_start;

    __bss_start = .;
    .bss : {
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        *(COMMON)

        . = ALIGN(4);
        __bss_end = .;
        __bss_size = __bss_end - __bss_start;

        /*
         * .data初始值快照，用于moxiebox重启时恢复.data。
         * 位于.bss末尾但不包含在__bss_start..__bss_end内，
         * 因此crt0清BSS时不会清除快照。
         */
        __data_snapshot_magic = .;
        . += 4;
        __data_snapshot_data = .;
        . += __data_size;
        __data_snapshot_end = .;
    }

    . = ALIGN(4);
    __heap_start = .;
    __heap_end = .;
    PROVIDE (end = .);
    PROVIDE (_end = .);

    /*
     * newlib风格的_ctype_别名，供moxie工具链的libstdc++使用。
     * picolibc以_ctype_b + 127作为字符类型表基址。
     */
    PROVIDE (_ctype_ = _ctype_b + 127);

    /DISCARD/ : {
        *(.comment)
        *(.note .note.*)
    }
}
