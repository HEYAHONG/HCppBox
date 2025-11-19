#include "hsoftplc.h"
#include <iec_types_all.h>
#include <stdbool.h>

/*
 * %QX0.0
 */
extern BOOL * __QX0_0;

bool plc_get_qx0_0(void)
{
    return (*__QX0_0)!=0;
}
