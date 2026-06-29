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

**注意：本目录为应用目录，与west的工作区不同，一般用户需要在west的工作区手动指定应用目录，非west工作区有些子命令（如build）不可用。**

## west工作区

若用户无现有west工作区，可在当前目录创建west工作区。

创建好python的venv环境并安装好west后(注意需要安装好zephyr需要的工具)，可使用以下命令创建工作区:

- 初始化：`west init` ,初始化一般用于下载zephyr源代码。
- 更新：`west update`,更新一般用于更新zephyr源代码并下载子模块。
- 安装python依赖:`west packages pip --install` ,用于安装python脚本可能用到的依赖。**注意:此步骤需要安装官方要求进入python的venv环境,否则无法执行。**
- 获取二进制文件:`west blobs -a fetch`,某些芯片的某些功能不会发布源代码，因此需要下载其发布的二进制文件。**注意:此步骤下载了所有支持的二进制文件，如仅需要部分芯片请查看官方说明。**

- 安装SDK:`west sdk install`,**注意:此步骤下载了所有支持的SDK工具，如仅需部分SDK工具请查看官方说明。**

## 工程文件

可使用CMake的额外选项生成工程文件，方便编辑源代码。

若在ubuntu可使用以下命令生成codeblocks的工程文件：

```bash
west build  --cmake-only -b [板级支持名称] -d [生成的文件存放目录]  [应用目录] -- -G "CodeBlocks - Unix Makefiles"
```

若在windows可使用以下命令生成codeblocks的工程文件：

```bash
west build  --cmake-only -b [板级支持名称] -d [生成的文件存放目录]  [应用目录] -- -G "CodeBlocks - Ninja"
```

对于不使用codeblocks的用户，也可使用VSCode的插件进行源代码编辑。

## 测试

对于zephyr对qemu的板级支持，可在zephyr工作区可指定`run`目标用于测试：

```bash
west build -b [板级支持名称] -d [生成的文件存放目录] -t run [应用目录]
```

# 目录说明

- [001-base](001-base):基础应用

