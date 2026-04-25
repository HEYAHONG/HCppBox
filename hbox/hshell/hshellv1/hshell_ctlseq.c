/***************************************************************
 * Name:      hshell_ctlseq.c
 * Purpose:   实现hshell_ctlseq接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2025-03-21
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hshell_ctlseq.h"

hshell_ctlseq_control_set_t hshell_ctlseq_control_set_get_from_bytes(uint8_t *control_bytes,size_t length)
{
    hshell_ctlseq_control_set_t ret=HSHELL_CTLSEQ_CONTROL_SET_INVAILD;
    if(control_bytes==NULL || length == 0)
    {
        return ret;
    }

    //ECMA-48 C0 0x00 - 0x1F
    if(control_bytes[0] <= 0x1F)
    {
        ret=(hshell_ctlseq_control_set_t)control_bytes[0];
    }

    if(control_bytes[0]==((uint8_t)HSHELL_CTLSEQ_CONTROL_SET_DEL))
    {
        ret=HSHELL_CTLSEQ_CONTROL_SET_DEL;
    }

    if(ret==HSHELL_CTLSEQ_CONTROL_SET_C0_ESC)
    {
        ret=HSHELL_CTLSEQ_CONTROL_SET_INVAILD;
        if(length>1)
        {
            switch(control_bytes[1])
            {
            case HSHELL_CTLSEQ_CONTROL_SET_C0_ESC:
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C0_ESC;
            }
            break;
            case 'D':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_IND;
            }
            break;
            case 'E':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_NEL;
            }
            break;
            case 'H':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_HTS;
            }
            break;
            case 'M':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_RI;
            }
            break;
            case 'N':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_SS2;
            }
            break;
            case 'O':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_SS3;
            }
            break;
            case 'P':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_DCS;
            }
            break;
            case 'V':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_SPA;
            }
            break;
            case 'W':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_EPA;
            }
            break;
            case 'X':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_SOS;
            }
            break;
            case 'Z':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_DECID;
            }
            break;
            case '[':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_CSI;
            }
            break;
            case '\\':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_ST;
            }
            break;
            case ']':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_OSC;
            }
            break;
            case '^':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_PM;
            }
            break;
            case '_':
            {
                ret=HSHELL_CTLSEQ_CONTROL_SET_C1_APC;
            }
            break;
            default:
            {

            }
            break;
            }
        }
    }
    return ret;
}
