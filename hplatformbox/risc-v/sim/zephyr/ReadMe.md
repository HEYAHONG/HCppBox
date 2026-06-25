# 说明

本目录存放zephyr的应用文件，并采用zephyr的元工具`west`编译。

若未特殊说明，本目录需要zephyr4.4及更新版本。

# 板级支持

本目录采用zephyr原生的板级支持,默认支持:

- qemu_riscv32
- qemu_riscv64

# 编译

本目录的应用均采用zephyr的生态编译，SDK、zephyr源代码均可使用`west`下载安装。

具体的说明见[https://zephyrproject.org/](https://zephyrproject.org/)。

注意：本目录为应用目录，与west的工作区不同，一般用户需要在west的工作区手动指定应用目录，非west工作区有些子命令（如build）不可用。

## 工程文件

可使用CMake的额外选项生成工程文件，方便编辑源代码。

若在ubuntu生成codeblocks的工程文件：

```bash
west build  --cmake-only -b [板级支持名称] -d [生成的文件存放目录]  [应用目录] -- -G "CodeBlocks - Unix Makefiles"
```

## 测试

对于zephyr对qemu的板级支持，可在zephyr工作区可指定`run_qemu`目标用于测试：

```bash
west build -b [板级支持名称] -d [生成的文件存放目录] -t run_qemu [应用目录]
```

# 目录说明

- [001-base](001-base):基础应用

