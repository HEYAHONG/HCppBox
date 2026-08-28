# 说明

mcs51指令集模拟（最小版），支持以下特性:

- 串口:直接通过控制台输出与输入。
- 定时器:频率1khz，无需12分频。
- 支持通过[openblt](https://github.com/feaser/openblt)加载程序。

# openblt

当用户未指定程序文件且未禁止openblt时,本模拟器将打开监听51000端口，此时用户可使用openblt的主机工具`BootCommander`或者`MicroBoot`。

对于`BootCommander`可使用以下命令上传程序文件:

```bash
BootCommander  -t=xcp_net -a=IP地址(本机则使用127.0.0.1) -p=51000 s19程序
```

对于`MicroBoot`，其为GUI工具，直接设定协议xcp_net并设定好端口地址即可上传程序文件。

注意：由于openblt使用的是srec格式的文件，可能不能直接使用ihx文件，可通过binutils工具中的objcopy转换：

```bash
objcopy -I ihex -O srec ihx程序文件 s19程序文件
```

# 程序

本模拟器支持bin文件与ihx文件。

示例程序见[../../../hplatformbox/mcs-51/sim/](../../../hplatformbox/mcs-51/sim/)

