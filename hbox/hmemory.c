/***************************************************************
 * Name:      hmemory.c
 * Purpose:   实现hmemory接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-04-26
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hmemory.h"

/*
 * 堆
 */
#include "hmemory/hmemoryheap.c"

/*
 * hash
 */
#include "hmemory/hmemoryhash.c"

/*
 * 环形缓冲区
 */
#include "hmemory/hringbuf.c"


/*
 * 对象
 */
#include "hmemory/hobject.c"

/*
 * 双向链表
 */
#include "hmemory/hdoublylinkedlist.c"


/*
 * 软件虚拟内存
 */
#include "hmemory/hsoftwarevirtualmemory.c"

/*
 * 静态内存分配
 */
 #include "hmemory/hmemorystaticallocator.c"
