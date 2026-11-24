# picolibc moxiebox hello-world

基于 picolibc 和 moxiebox C 运行库的 hello-world 模板，包含 C 与 C++ 示例。

## 构建

```bash
cd halibc/picolibc/port/moxiebox/hello-world
cmake -S . -B build
cmake --build build
```

`toolchain.cmake` 会从 `PATH` 中查找
`moxie-unknown-elf-gcc/g++` 或 `moxie-unknown-moxiebox-gcc/g++`。
也可以显式指定工具链目录：

```bash
cmake -S . -B build -DMOXIE_TOOLCHAIN=/path/to/moxie-toolchain/bin
```

或通过环境变量：

```bash
MOXIE_TOOLCHAIN=/path/to/moxie-toolchain/bin cmake -S . -B build
```

生成：

- `build/hello-world.elf`：C 版本
- `build/hello-world-cpp.elf`：C++ 版本

运行：

```bash
hmoxiebox -f build/hello-world.elf
hmoxiebox -f build/hello-world-cpp.elf
```

C++ 版本使用 moxie 工具链的 libstdc++，moxiebox C 运行库提供 newlib ABI 兼容符号。
