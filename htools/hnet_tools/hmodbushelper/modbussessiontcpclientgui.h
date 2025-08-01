#ifndef __MODBUSSESSIONTCPCLIENTGUI_H__
#define __MODBUSSESSIONTCPCLIENTGUI_H__
#ifdef __cplusplus
#include "guibase/mainframe.h"
#include "modbussessiontcpclient.h"

class ModbusSessionTCPClientGui:public ModbusSessionTCPClient,public modbussessiontcpclientbase
{
public:
    ModbusSessionTCPClientGui(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
    virtual ~ModbusSessionTCPClientGui();

protected:
    virtual void OnSetFocus( wxFocusEvent& event );
    virtual void OnButtonClick_Coils_Read( wxCommandEvent& event );
    virtual void OnButtonClick_Coils_Write( wxCommandEvent& event );
    virtual void OnButtonClick_Coils_Write_Single( wxCommandEvent& event );
    virtual void OnButtonClick_Connect_DisConnect( wxCommandEvent& event );
    virtual void OnGridCellChange_Modbus_Coils( wxGridEvent& event );
    virtual void OnModbusException(uint8_t function_code,uint8_t exception_code);
    virtual void OnModbusWrite(const uint8_t *adu,size_t adu_length);
    virtual void OnModbusRead(const uint8_t *buffer,size_t buffer_length);

private:
    void UpdateState();
    void UpdateModbusCoils();
    void LocalLog(wxString format,...);
};

#endif // __cplusplus
#endif // __MODBUSSESSIONTCPCLIENTGUI_H__
