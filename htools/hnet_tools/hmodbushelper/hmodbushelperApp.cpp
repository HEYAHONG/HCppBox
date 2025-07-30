/***************************************************************
 * Name:      hmodbushelperApp.cpp
 * Purpose:   Code for Application Class
 * Author:    HYH (2229388563@qq.com)
 * Created:   2025-07-30
 * Copyright: HYH (https://hyhsystem.cn)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "hmodbushelperApp.h"
#include "hmodbushelperMain.h"

IMPLEMENT_APP(hmodbushelperApp);

bool hmodbushelperApp::OnInit()
{
    hmodbushelperFrame* frame = new hmodbushelperFrame(0L);

    frame->Show();

    return true;
}
