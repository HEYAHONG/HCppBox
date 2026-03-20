#include "hdefaults.h"

#undef  fopen
#define fopen    hfopen
#undef  fread
#define fread    hfread
#undef  fwrite
#define fwrite   hfwrite
#undef  fclose
#define fclose   hfclose
#undef  ferror
#define ferror   hferror
#undef  fseek
#define fseek    hfseek
#undef  fprintf
#define fprintf  hfprintf
#undef  vfprintf
#define vfprintf hvfprintf


