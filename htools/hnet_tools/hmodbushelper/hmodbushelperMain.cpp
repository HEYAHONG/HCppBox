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
#include "modbussessiontcpclientgui.h"
#include <wx/fontutil.h>
#include <wx/log.h>
#include <wx/filedlg.h>
#include <wx/statbmp.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
#include <wx/webview.h>
#include <wx/webviewfshandler.h>
#include <wx/fs_mem.h>
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

    {
        //添加欢迎页数据
        wxFileSystem::AddHandler(new wxMemoryFSHandler);
        RCEnum([](const unsigned char *Name,size_t NameLength,const unsigned char *Resource,size_t ResourceLength)
        {
            wxMemoryFSHandler::AddFile(std::string((const char *)Name,NameLength).c_str(),Resource,ResourceLength);
        });
    }
}

hmodbushelperFrame::~hmodbushelperFrame()
{
    hcppbox_deinit();
}

void hmodbushelperFrame::OnActivate( wxActivateEvent& event )
{
    if(m_main_auinotebook->GetPageCount() <=0 )
    {
        /*
         * 当首页无内容时,添加页面
         */
        wxWebView* browser=wxWebView::New();
        if(browser!=NULL)
        {
            browser->Create(m_main_auinotebook,wxNewId(),_T("memory:index.htm"));
            browser->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewFSHandler("memory")));
            browser->LoadURL(_T("memory:index.htm"));
            browser->EnableHistory(false);
            m_main_auinotebook->AddPage(browser,_T("主页"),true);
        }

    }
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

void hmodbushelperFrame::OnMenuSelection_New_Modbus_Session_TCP_Client( wxCommandEvent& event )
{
    ModbusSessionTCPClientGui *session=new ModbusSessionTCPClientGui(this);
    m_main_auinotebook->AddPage(session,_T("Modbus TCP Client"),true);
}

void hmodbushelperFrame::OnMSTimer( wxTimerEvent& event )
{
    hcppbox_softwaretimer_isr();
}
