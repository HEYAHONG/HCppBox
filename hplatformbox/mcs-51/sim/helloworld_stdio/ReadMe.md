# 说明

本代码主要用于在串口中输出一个字符串。本代码采用C库函数输出信息。

注意：本代码要求模拟器/硬件实现串口功能。

# 编译

## ihx文件

### 使用cmake编译

与其它CMake工程一样，编译ROM代码分为以下步骤：

- 进入代码目录并创建构建目录。
- 配置CMake工程。
- 构建工程。

具体命令如下：

```bash
#进入代码目录并创建构建目录
mkdir build
cd build

#配置CMake工程
cmake ../

#构建工程
cmake --build .
```

### 手工编译

默认情况下，内存模型采用极简模型，程序空间为8KB，C标准采用SDCC扩展的C11标准，用户可更改`sdcc`的命令参数用于其它情况。

可采用以下命令生成ihx文件：

```bash
sdcc -mmcs51 --std-sdcc11  --model-small --iram-size 128 --xram-size 0 --code-size 8192   helloworld.c
```

编译完成后，SDCC将生成一大堆文件，其中ihx文件即为生成的hex文件。

## bin文件

某些情况下，用户可能需要生成bin文件，生成ihx文件后，可通过SDCC自带的`makebin`命令将ihx文件转化为bin文件。

```bash
makebin -p helloworld.ihx helloworld.bin
```

