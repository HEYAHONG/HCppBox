/***************************************************************
 * Name:      hmodbushelperMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    HYH (2229388563@qq.com)
 * Created:   2025-07-30
 * Copyright: HYH (https://hyhsystem.cn)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "hmodbushelperMain.h"
#include <wx/fontutil.h>
#include <wx/log.h>
#include <wx/filedlg.h>
#include <wx/statbmp.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include "hmodbushelper.xpm"



hmodbushelperFrame::hmodbushelperFrame(wxFrame *frame): mainframe(frame),log(NULL)
{
    SetIcon(wxIcon(hmodbushelper_xpm));
    if(log==NULL)
    {
        log=new logdialog(this);
        wxFont font=log->m_logdialog_textCtrl->GetFont();
        log->m_logdialog_textCtrl->SetFont(*wxTheFontList->FindOrCreateFont(font.GetNativeFontInfo()->GetPointSize(),wxFONTFAMILY_TELETYPE,font.GetStyle(),font.GetWeight()));
        wxLogTextCtrl *logger=new wxLogTextCtrl(log->m_logdialog_textCtrl);
        wxLog *oldlog=wxLog::SetActiveTarget(logger);
        if(oldlog!=NULL)
        {
            delete oldlog;
        }
    }
    hcppbox_init();
    {
        const char *banner=(const char *)RCGetHandle("banner");
        if(banner!=NULL)
        {
            wxLogMessage(wxT("\r\n%s\r\n初始化完成!"),banner);
        }
    }
}

hmodbushelperFrame::~hmodbushelperFrame()
{
    hcppbox_deinit();
}

void hmodbushelperFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void hmodbushelperFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void hmodbushelperFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(wxString(wxT("hmodbushelper ")+wxString(PROJECT_VERSION_STR)+wxT(" BY HEYAHONG")),wxT(" 关于"));
}

void hmodbushelperFrame::OnMenuSelection_Menu_Quit( wxCommandEvent& event )
{
    Destroy();
}

void hmodbushelperFrame::OnMenuSelection_Menu_Logdialog( wxCommandEvent& event )
{
    if(log!=NULL)
    {
        log->Show(event.IsChecked());
    }
}

 void hmodbushelperFrame::OnMSTimer( wxTimerEvent& event )
 {
     hcppbox_softwaretimer_isr();
 }
