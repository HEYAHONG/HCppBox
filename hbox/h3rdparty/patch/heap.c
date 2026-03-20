#include "hdefaults.h"

//定义(堆)内存分配函数
#undef  malloc
#define malloc     hmalloc
#undef  free
#define free       hfree
#undef  calloc
#define calloc     hcalloc
#undef  realloc
#define realloc    hrealloc
