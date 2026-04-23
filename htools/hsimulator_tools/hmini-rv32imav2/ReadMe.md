# 说明

基于[mini-rv32ima](https://github.com/cnlohr/mini-rv32ima)的第二版。

# 设备树

本程序使用内置设备树，一般不需要修改，但如需修改chosen选项（内核启动命令），重新生成设备树二进制文件并且使用命令选项加载。

```dts
/dts-v1/;

/ {
	#address-cells = <0x02>;
	#size-cells = <0x02>;
	compatible = "riscv-minimal-nommu";
	model = "riscv-minimal-nommu,qemu";

	chosen {
		bootargs = "earlycon=uart8250,mmio,0x10000000,1000000 console=ttyS0";
	};

	memory@80000000 {
		device_type = "memory";
		reg = <0x00 0x80000000 0x00 0x3ffc000>;
	};

	cpus {
		#address-cells = <0x01>;
		#size-cells = <0x00>;
		timebase-frequency = <0xf4240>;

		cpu@0 {
			phandle = <0x01>;
			device_type = "cpu";
			reg = <0x00>;
			status = "okay";
			compatible = "riscv";
			riscv,isa = "rv32ima";
			mmu-type = "riscv,none";

			interrupt-controller {
				#interrupt-cells = <0x01>;
				interrupt-controller;
				compatible = "riscv,cpu-intc";
				phandle = <0x02>;
			};
		};

		cpu-map {

			cluster0 {

				core0 {
					cpu = <0x01>;
				};
			};
		};
	};

	soc {
		#address-cells = <0x02>;
		#size-cells = <0x02>;
		compatible = "simple-bus";
		ranges;

		uart@10000000 {
			clock-frequency = <0x1000000>;
			reg = <0x00 0x10000000 0x00 0x100>;
			compatible = "ns16850";
		};

		poweroff {
			value = <0x5555>;
			offset = <0x00>;
			regmap = <0x04>;
			compatible = "syscon-poweroff";
		};

		reboot {
			value = <0x7777>;
			offset = <0x00>;
			regmap = <0x04>;
			compatible = "syscon-reboot";
		};

		syscon@11100000 {
			phandle = <0x04>;
			reg = <0x00 0x11100000 0x00 0x1000>;
			compatible = "syscon";
		};

		clint@11000000 {
			interrupts-extended = <0x02 0x03 0x02 0x07>;
			reg = <0x00 0x11000000 0x00 0x10000>;
			compatible = "sifive,clint0\0riscv,clint0";
		};
	};
};
```



# 注意

- 本版不支持CSR测试。
- 本程序可直接加载Image文件（注意需要内嵌initrd到Image）。

# 编译

本工程采用CMake作为构建系统。



