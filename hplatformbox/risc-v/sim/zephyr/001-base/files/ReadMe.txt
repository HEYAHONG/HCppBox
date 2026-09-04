riscv32架构的Qemu启动命令：
qemu-system-riscv32 -machine virt -bios none -m 256  -chardev stdio,id=con,mux=on -serial chardev:con -mon chardev=con,mode=readline -nographic -icount shift=6,align=off,sleep=off -rtc clock=vm -net none -kernel zephyr.elf
riscv64架构的Qemu启动命令：
qemu-system-riscv64 -machine virt -bios none -m 256  -chardev stdio,id=con,mux=on -serial chardev:con -mon chardev=con,mode=readline -nographic -icount shift=6,align=off,sleep=off -rtc clock=vm -net none -kernel zephyr.elf
