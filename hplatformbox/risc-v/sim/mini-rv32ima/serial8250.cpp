#include "serial8250.h"

int serial8250_getchar(volatile serial8250_t *serial)
{
    if(serial==NULL)
    {
        return -1;
    }

    if(serial->lsr&0x01)
    {
        serial->lsr&=(~0x01);
        return serial->rbr_thr;
    }
    else
    {
        return -1;
    }
}
int serial8250_putchar(volatile serial8250_t *serial,uint8_t ch)
{
    if(serial==NULL)
    {
        return -1;
    }

    while((serial->lsr&0x40)==0);

    /*
     * 虚拟环境直接发送，不检查状态
     */

    serial->rbr_thr=ch;

    return ch;

}

