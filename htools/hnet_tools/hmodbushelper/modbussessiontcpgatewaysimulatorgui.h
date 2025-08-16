#ifndef MODBUSSESSIONTCPGATEWAYSIMULATORGUI_H
#define MODBUSSESSIONTCPGATEWAYSIMULATORGUI_H

#ifdef __cplusplus
#include "guibase/mainframe.h"
#include "modbussessiontcpgatewaysimulator.h"

class ModbusSessionTCPGatewaySimulatorGui:public ModbusSessionTCPGatewaySimulator,public modbussessiontcpgatewaysimulatorbase
{
public:
    ModbusSessionTCPGatewaySimulatorGui(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
    virtual ~ModbusSessionTCPGatewaySimulatorGui();

protected:
    virtual void OnGridCellChange_Simulator_Data( wxGridEvent& event );
    virtual void OnButtonClick_IPV4_Gateway_Start_Stop( wxCommandEvent& event );
    virtual void OnButtonClick_IPV6_Gateway_Start_Stop( wxCommandEvent& event );
    virtual void OnTimer_Simulator_Update( wxTimerEvent& event );
    virtual void OnDataUpdate();
private:
    void UpdateSimulatorData();
};

#endif

#endif // MODBUSSESSIONTCPGATEWAYSIMULATORGUI_H
