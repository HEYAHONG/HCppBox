    .section .vector.interrupt
    .align 2
    .option norvc
_interrupt_vector_table:
    /*
     * 向量基地址为0，向量模式。
     * 中断号0~15使用标准中断号，全部跳转到FreeRTOS的陷入入口。
     */
    .rept 16
    j freertos_risc_v_trap_handler
    .endr

    /*
     * 中断号16: 复位中断
     */
    j vector_table_reset_handler

    /*
     * 中断号17~63: 平台/用户自定义中断
     */
    .rept 47
    j freertos_risc_v_trap_handler
    .endr
