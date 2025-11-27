# 说明

本工具提供hash计算功能,作为某些`unix`系统命令(如`sha256sum`、`sha512sum`)的简化版本。

包含的工具如下:

- `hmd4sum`
- `hmd5sum` 
- `hsha1sum` 
- `hsha224sum` 
- `hsha256sum` 
- `hsha384sum` 
- `hsha512sum` 
- `hsha3sum224`
- `hsha3sum256`
- `hsha3sum384`
- `hsha3sum512`
- `hripemd160sum` 
- `hsm3sum`

所有工具的用法与系统哈希命令(由[coreutils](https://www.gnu.org/software/coreutils/)或者[busybox](http://www.busybox.net/)提供，如`md5sum`、`sha256sum`、`sha512sum`等)类似,可用于计算文件的hash值，也可计算标准输入的hash值。

