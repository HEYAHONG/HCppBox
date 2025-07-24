/***************************************************************
 * Name:      hguitoolsApp.cpp
 * Purpose:   Code for Application Class
 * Author:    HYH (2229388563@qq.com)
 * Created:   2025-07-24
 * Copyright: HYH (https://hyhsystem.cn)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "hguitoolsApp.h"
#include "hguitoolsMain.h"

IMPLEMENT_APP(hguitoolsApp);

bool hguitoolsApp::OnInit()
{
    hguitoolsFrame* frame = new hguitoolsFrame(0L);

    frame->Show();

    return true;
}
