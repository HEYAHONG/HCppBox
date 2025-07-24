/***************************************************************
 * Name:      hguitoolsMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    HYH (2229388563@qq.com)
 * Created:   2025-07-24
 * Copyright: HYH (https://hyhsystem.cn)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "hguitoolsMain.h"
#include "common_wxwidgets.h"
#include <wx/log.h>
#include <wx/filedlg.h>
#include <wx/file.h>
#include <set>

hguitoolsFrame::hguitoolsFrame(wxFrame *frame)
    : mainframe(frame)
{
    wxLog::EnableLogging(true);
    {
        //设置日志窗口
        wxLogTextCtrl *logger=new wxLogTextCtrl(m_textCtrl_log);
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

hguitoolsFrame::~hguitoolsFrame()
{
    hcppbox_deinit();
}

void hguitoolsFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void hguitoolsFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void hguitoolsFrame::OnAbout(wxCommandEvent &event)
{

}

void hguitoolsFrame::dotfontscan_OnFontChanged( wxFontPickerEvent& event )
{
    auto font=event.GetFont();
    wxLogMessage(wxT("dotfontscan字体已更改为%s,高度=%d,宽度%s"),font.GetFaceName(),font.GetPixelSize().GetHeight(),font.IsFixedWidth()?wxT("固定"):wxT("不固定"));
}
void hguitoolsFrame::dotdontscan_start_OnButtonClick( wxCommandEvent& event )
{
    wxFileDialog SourceFileDialog(this, wxT("选择待扫描的文件"),wxT(""), wxT(""),wxT("*"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if(SourceFileDialog.ShowModal() == wxID_CANCEL)
    {
        wxLogMessage(wxT("已取消操作！"));
        return;
    }
    wxLogMessage(wxT("准备打开文件 %s"),SourceFileDialog.GetPath());
    wxFile SourceFile(SourceFileDialog.GetPath());
    if(!SourceFile.IsOpened())
    {
        wxLogMessage(wxT("打开文件 %s 失败！"),SourceFileDialog.GetPath());
    }
    std::set<uint32_t> char_code_set;
    {
        wxString data;
        SourceFile.ReadAll(&data);
        SourceFile.Close();
        for(auto it=data.begin(); it!=data.end(); it++)
        {
            char_code_set.insert(*it);
        }
    }
    wxLogMessage(wxT("扫描完成，总字符数:%d"),(int)char_code_set.size());
    if(char_code_set.size()==0)
    {
        return;
    }
    {
        auto minval=char_code_set.begin();
        auto maxval=char_code_set.begin();
        std::advance(maxval,char_code_set.size()-1);
        wxLogMessage(wxT("字符编码最大值:%08X,字符编码最小值：%08X"),(int)(*maxval),(int)(*minval));
    }

}

void hguitoolsFrame::OnMSTimer( wxTimerEvent& event )
{
    hcppbox_softwaretimer_isr();
}
