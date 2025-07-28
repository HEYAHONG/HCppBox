/***************************************************************
 * Name:      hguitoolsMain.h
 * Purpose:   Defines Application Frame
 * Author:    HYH (2229388563@qq.com)
 * Created:   2025-07-24
 * Copyright: HYH (https://hyhsystem.cn)
 * License:
 **************************************************************/

#ifndef HGUITOOLSMAIN_H
#define HGUITOOLSMAIN_H



#include "hguitoolsApp.h"
#include "common_wxwidgets.h"
#include "guibase/hguitools_mainbase.h"

class hguitoolsFrame: public mainframe
{
public:
    hguitoolsFrame(wxFrame *frame);
    ~hguitoolsFrame();
protected:
    virtual void dotfontscan_OnFontChanged( wxFontPickerEvent& event );
    virtual void dotdontscan_start_OnButtonClick( wxCommandEvent& event );
    virtual void dotdontscan_save_OnButtonClick( wxCommandEvent& event );
    virtual void imageresourcegenerate_load_OnButtonClick( wxCommandEvent& event );
    virtual void imageresourcegenerate_save_OnButtonClick( wxCommandEvent& event );
    virtual void OnMSTimer( wxTimerEvent& event );
private:
    virtual void OnClose(wxCloseEvent& event);
    virtual void OnQuit(wxCommandEvent& event);
    virtual void OnAbout(wxCommandEvent& event);
};

#endif // HGUITOOLSMAIN_H
