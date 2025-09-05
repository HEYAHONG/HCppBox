/***************************************************************
 * Name:      hsimulatortoolsApp.cpp
 * Purpose:   Code for Application Class
 * Author:    HYH (2229388563@qq.com)
 * Created:   2025-09-05
 * Copyright: HYH (https://hyhsystem.cn)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "hsimulatortoolsApp.h"
#include "hsimulatortoolsMain.h"

IMPLEMENT_APP(hsimulatortoolsApp);

bool hsimulatortoolsApp::OnInit()
{
    hsimulatortoolsFrame* frame = new hsimulatortoolsFrame(0L);

    frame->Show();

    return true;
}
