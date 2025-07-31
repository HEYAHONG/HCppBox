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

private:
};

#endif // __cplusplus
#endif // __MODBUSSESSIONTCPCLIENTGUI_H__
