#ifndef MODBUSSESSIONTCPGATEWAYSERIALPORTGUI_H
#define MODBUSSESSIONTCPGATEWAYSERIALPORTGUI_H

#ifdef __cplusplus
#include "guibase/mainframe.h"
#include "modbussessiontcpgatewayserialport.h"
#include <wx/msgqueue.h>


class ModbusSessionTCPGatewaySerialportGui:public modbussessiontcpgatewayserialportbase,public ModbusSessionTCPGatewaySerialport
{
public:
    ModbusSessionTCPGatewaySerialportGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 750,500 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
    virtual ~ModbusSessionTCPGatewaySerialportGui();

    virtual void LocalLog(wxString format,...);

protected:
    virtual void OnComboboxDropdown_Serialport_Name( wxCommandEvent& event );
    virtual void OnButtonClick_IPV4_Gateway_Start_Stop( wxCommandEvent& event );
    virtual void OnButtonClick_IPV6_Gateway_Start_Stop( wxCommandEvent& event );
    virtual void OnTimer_UIUpdate( wxTimerEvent& event );

    virtual void OnModbusRTUBegin();
    virtual void OnModbusRTUSend(std::string request);
    virtual void OnModbusRTUReceive(std::string reply);
    virtual void OnModbusRTUEnd();


private:
    void UpdateSerialportList();
    void SerialportConfigEnable(bool enable);
    wxMessageQueue<wxString> logqueue;
};

#endif // __cplusplus

#endif // MODBUSSESSIONTCPGATEWAYSERIALPORTGUI_H
