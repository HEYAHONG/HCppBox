#include "hdefaults.h"

#undef  open
#define open     hopen
#undef  read
#define read     hread
#undef  write
#define write    hwrite
#undef  close
#define close    hclose
#undef  lseek
#define lseek    hlseek
#undef  lseek64
#define lseek64  hlseek64
#undef  fcntl
#define fcntl    hfcntl
