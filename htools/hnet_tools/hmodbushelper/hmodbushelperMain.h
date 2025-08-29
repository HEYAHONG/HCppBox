/***************************************************************
 * Name:      hmodbushelperMain.h
 * Purpose:   Defines Application Frame
 * Author:    HYH (2229388563@qq.com)
 * Created:   2025-07-30
 * Copyright: HYH (https://hyhsystem.cn)
 * License:
 **************************************************************/

#ifndef HMODBUSHELPERMAIN_H
#define HMODBUSHELPERMAIN_H



#include "hmodbushelperApp.h"


#include "guibase/mainframe.h"
#include "common_wxwidgets.h"
#include "Version.h"

class hmodbushelperFrame: public mainframe
{
public:
    hmodbushelperFrame(wxFrame *frame);
    ~hmodbushelperFrame();
private:
    virtual void OnActivate( wxActivateEvent& event );
    virtual void OnClose(wxCloseEvent& event);
    virtual void OnQuit(wxCommandEvent& event);
    virtual void OnAbout(wxCommandEvent& event);
    virtual void OnMenuSelection_Menu_Quit( wxCommandEvent& event );
    virtual void OnMenuSelection_Menu_Logdialog( wxCommandEvent& event );
    virtual void OnMenuSelection_New_Modbus_Session_TCP_Client( wxCommandEvent& event );
    virtual void OnMenuSelection_New_Modbus_Session_TCP_Gateway_Simulator( wxCommandEvent& event );
    virtual void OnMenuSelection_New_Modbus_Session_TCP_Gateway_Serialport( wxCommandEvent& event );
    virtual void OnMenuSelection_Modbus_Protocol( wxCommandEvent& event );
    virtual void OnMSTimer( wxTimerEvent& event );
    logdialog *log;
};

#endif // HMODBUSHELPERMAIN_H
