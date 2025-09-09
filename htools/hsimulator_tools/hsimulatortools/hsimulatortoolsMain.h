/***************************************************************
 * Name:      hsimulatortoolsMain.h
 * Purpose:   Defines Application Frame
 * Author:    HYH (2229388563@qq.com)
 * Created:   2025-09-05
 * Copyright: HYH (https://hyhsystem.cn)
 * License:
 **************************************************************/

#ifndef HGUITOOLSMAIN_H
#define HGUITOOLSMAIN_H



#include "hsimulatortoolsApp.h"
#include "common_wxwidgets.h"
#include "guibase/hsimulatortools_mainbase.h"

class hsimulatortoolsFrame: public mainframe
{
public:
    hsimulatortoolsFrame(wxFrame *frame);
    ~hsimulatortoolsFrame();
protected:
    virtual void OnMSTimer( wxTimerEvent& event );
    virtual void OnMenuSelectionRVVMGeneric( wxCommandEvent& event );
private:
    virtual void OnClose(wxCloseEvent& event);
    virtual void OnQuit(wxCommandEvent& event);
    virtual void OnAbout(wxCommandEvent& event);
};

#endif // HGUITOOLSMAIN_H
