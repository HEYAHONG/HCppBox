/***************************************************************
 * Name:      hsimulatortoolsMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    HYH (2229388563@qq.com)
 * Created:   2025-09-05
 * Copyright: HYH (https://hyhsystem.cn)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "astyle.h"
#include "astyle_main.h"
#include "hsimulatortoolsMain.h"
#include "common_wxwidgets.h"
#include "Version.h"
#include <wx/fontutil.h>
#include <wx/log.h>
#include <wx/filedlg.h>
#include <wx/statbmp.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include <wx/mstream.h>
#include <wx/dcmemory.h>
#include <wx/rawbmp.h>
#include "hsimulatortools.xpm"
#include <set>
#include <sstream>
#include <string>
#include "guibase.h"
#include "rvvm/rvvm.h"

hsimulatortoolsFrame::hsimulatortoolsFrame(wxFrame *frame)
    : mainframe(frame)
{
    SetIcon(wxIcon(hsimulatortools_xpm));
    wxLog::EnableLogging(true);
    {
        //设置日志窗口
        wxFont font=m_textCtrl_log->GetFont();
        m_textCtrl_log->SetFont(*wxTheFontList->FindOrCreateFont(font.GetNativeFontInfo()->GetPointSize(),wxFONTFAMILY_TELETYPE,font.GetStyle(),font.GetWeight()));
        wxLogTextCtrl *logger=new wxLogTextCtrl(m_textCtrl_log);
        wxLog *oldlog=wxLog::SetActiveTarget(logger);
        if(oldlog!=NULL)
        {
            delete oldlog;
        }
    }
    hcppbox_init();

    if(!rvvm_check_abi(RVVM_ABI_VERSION))
    {
        wxMessageBox(wxString(wxT("RVVM ABI 不兼容，程序运行可能异常！")),wxT("错误"));
    }
    else
    {
         wxLogMessage(wxT("RVVM:"  RVVM_VERSION "!"));
    }

    {
        const char *banner=(const char *)RCGetHandle("banner");
        if(banner!=NULL)
        {
            wxLogMessage(wxT("\r\n%s\r\n初始化完成!"),banner);
        }
    }
}

hsimulatortoolsFrame::~hsimulatortoolsFrame()
{
    hcppbox_deinit();
}

void hsimulatortoolsFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void hsimulatortoolsFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void hsimulatortoolsFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(wxString(wxT("hsimulatortools ")+wxString(PROJECT_VERSION_STR)+wxT(" BY HEYAHONG")),wxT(" 关于"));
}


void hsimulatortoolsFrame::OnMSTimer( wxTimerEvent& event )
{
    hcppbox_softwaretimer_isr();
}

