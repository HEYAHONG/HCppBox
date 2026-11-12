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
#include "rvvmgenericgui.h"
#include "mcs51genericgui.h"
#include "minirv32imav2genericgui.h"
#include "minirisvcgenericgui.h"

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

    /*
     * 添加HBox内置模拟器菜单
     */
    {
        wxMenu* hboxMenu=new wxMenu();
        wxMenuItem* mcs51Item=new wxMenuItem(hboxMenu,wxID_ANY,_T("Mini-MCS51"));
        hboxMenu->Append(mcs51Item);

        wxMenuItem* riscvItem=new wxMenuItem(hboxMenu,wxID_ANY,_T("Mini-RISCV"));
        hboxMenu->Append(riscvItem);

        wxMenuItem* hboxItem=new wxMenuItem(m_menu_virtualmachine,wxID_ANY,_T("HBox"),wxEmptyString,wxITEM_NORMAL,hboxMenu);
        m_menu_virtualmachine->Append(hboxItem);

        wxMenuItem* rv32imaItem=new wxMenuItem(m_menu_virtualmachine,wxID_ANY,_T("mini-rv32ima"));
        m_menu_virtualmachine->Append(rv32imaItem);

        Bind(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(hsimulatortoolsFrame::OnMenuSelectionHBoxMCS51),this,mcs51Item->GetId());
        Bind(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(hsimulatortoolsFrame::OnMenuSelectionMiniRV32IMA),this,rv32imaItem->GetId());
        Bind(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(hsimulatortoolsFrame::OnMenuSelectionMiniRISCV),this,riscvItem->GetId());
    }

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

void hsimulatortoolsFrame::OnMenuSelectionRVVMGeneric( wxCommandEvent& event )
{
    RVVMGenericGui *vm=new RVVMGenericGui(mainauinotebook);
    mainauinotebook->AddPage(vm,_T("RVVM-通用"),true);
}

void hsimulatortoolsFrame::OnMenuSelectionHBoxMCS51( wxCommandEvent& event )
{
    MCS51GenericGui *vm=new MCS51GenericGui(mainauinotebook);
    mainauinotebook->AddPage(vm,_T("Mini-MCS51"),true);
}

void hsimulatortoolsFrame::OnMenuSelectionMiniRV32IMA( wxCommandEvent& event )
{
    MiniRV32IMAV2GenericGui *vm=new MiniRV32IMAV2GenericGui(mainauinotebook);
    mainauinotebook->AddPage(vm,_T("mini-rv32ima"),true);
}

void hsimulatortoolsFrame::OnMenuSelectionMiniRISCV( wxCommandEvent& event )
{
    MiniRISCVGenericGui *vm=new MiniRISCVGenericGui(mainauinotebook);
    mainauinotebook->AddPage(vm,_T("Mini-RISCV"),true);
}

