# 说明

本目录主要存放一些Demo程序。

本目录下的所有代码均采用[SDCC](https://sdcc.sourceforge.net/)作为C编译器。

若未特殊说明均采用单文件源代码，此时，如有多个源代码文件，可采用`#include`将其它源代码文件包含在主函数`main`所在文件。

# 编译

## 编译环境

代码采用`sdcc`作为C编译器.

在ubuntu24.04及更新版本的系统,可使用以下命令安装编译环境:

```bash
sudo apt-get install build-essential sdcc
```

对于Windows用户而言，可到[SDCC](https://sdcc.sourceforge.net/)官方网站下载安装。

## 编译ROM代码

编译ROM代码的过程大同小异，均采用以下步骤：

- 打开终端（Windows下可打开cmd.exe）。
- 进入ROM代码目录。
- 使用`sdcc`编译C源代码。
- 使用`makebin`将ihx文件转化为bin文件(此步骤可选)。

# 目录说明

- [helloworld](helloworld):helloworld程序
- [helloworld_stdio](helloworld_stdio):helloworld程序(采用C库实现)

