/***************************************************************
 * Name:      HCPPSocketCygwinHelper.cpp
 * Purpose:   HCPPSocketCygwinHelper
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-01-10
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/

#ifdef __CYGWIN__

#include "winsock2.h"
#include "windows.h"

void HCPPSocketCygwinHelperStartup()
{
    WSADATA wsaData;
    const WORD VersionList[]=
    {
        (2 << 8) | 2,
        (1 << 8) | 1,
    };
    for(size_t i=0; i<sizeof(VersionList)/sizeof(VersionList[0]); i++)
    {
        if(WSAStartup(VersionList[i],&wsaData)==0)
        {
            break;
        }
    }
}

void HCPPSocketCygwinHelperCleanup()
{
    WSACleanup();
}

#endif // __CYGWIN__
