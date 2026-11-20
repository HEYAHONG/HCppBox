#!/bin/sh
set -e
CC=${CC:-x86_64-w64-mingw32-gcc}
CC_i686=${CC_i686:-i686-w64-mingw32-gcc}
OUT=${OUT:-.}
$CC -shared -O2 -o "$OUT/helloworld.x86_64.dll" -Wl,--out-implib,"$OUT/libhelloworld.x86_64.a" pe_dll.c
$CC -O2 -o "$OUT/helloworld.x86_64.exe" pe_main.c -L"$OUT" -lhelloworld.x86_64
$CC_i686 -shared -O2 -o "$OUT/helloworld.i386.dll" -Wl,--out-implib,"$OUT/libhelloworld.i386.a" pe_dll.c
$CC_i686 -O2 -o "$OUT/helloworld.i386.exe" pe_main.c -L"$OUT" -lhelloworld.i386
$CC -nostdlib -Wl,-e,entry -o "$OUT/helloworld.static.x86_64.exe" static_main.c
$CC_i686 -nostdlib -Wl,-e,_entry -o "$OUT/helloworld.static.i386.exe" static_main.c
