# 工具

此目录下COFF文件采用以下工具编译:

- GCC：ubuntu下可使用`sudo apt-get install gcc-mingw-w64-x86-64 gcc-mingw-w64-i686`安装。
- Clang:ubuntu下可使用`sudo apt-get install clang`安装。

# COFF文件编译

- `helloworld.i386.obj`:` i686-w64-mingw32-gcc -c helloworld.c -o helloworld.i386.obj`
- `helloworld.x86_64.obj`:` x86_64-w64-mingw32-gcc -c helloworld.c -o helloworld.x86_64.obj`
- `helloworld.aarch64.obj`:`clang --target=aarch64-w64-windows-gnu -c helloworld.c -o helloworld.aarch64.obj`
- `helloworld.i386.msvc.obj`:MSVC X86环境下执行:`cl /c /utf-8 /Fohelloworld.i386.msvc.obj /nologo helloworld.c`
- `helloworld.x86_64.msvc.obj`:MSVC X64环境下执行:`cl /c /utf-8 /Fohelloworld.x86_64.msvc.obj /nologo helloworld.c`
- `helloworld.aarch64.msvc.obj`:MSVC arm64环境下执行:`cl /c /utf-8 /Fohelloworld.aarch64.msvc.obj /nologo helloworld.c`

