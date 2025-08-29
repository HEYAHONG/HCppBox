#include "modbussessiontcpgatewayserialportgui.h"
#include "libserialport.h"
#include "wx/arrstr.h"
#include "wx/msgdlg.h"

ModbusSessionTCPGatewaySerialportGui::ModbusSessionTCPGatewaySerialportGui(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):modbussessiontcpgatewayserialportbase(parent,id,pos,size,style,name)
{
}

ModbusSessionTCPGatewaySerialportGui::~ModbusSessionTCPGatewaySerialportGui()
{

}

void ModbusSessionTCPGatewaySerialportGui::LocalLog(wxString format,...)
{
    va_list va;
    va_start(va,format);
    wxString msg=wxString::FormatV(format,va);
    va_end(va);
    logqueue.Post(msg);
}

void ModbusSessionTCPGatewaySerialportGui::OnComboboxDropdown_Serialport_Name( wxCommandEvent& event )
{
    UpdateSerialportList();
}

void ModbusSessionTCPGatewaySerialportGui::OnButtonClick_IPV4_Gateway_Start_Stop( wxCommandEvent& event )
{
    if(m_comboBox_Serialport_Name->GetValue().IsEmpty())
    {
        LocalLog(_T("未选择串口！\r\n"));
        return;
    }
    unsigned long port=0;
    m_choice_port->GetStringSelection().ToULong(&port);
    bool localhost=m_checkBox_localhost->IsChecked();
    if(IsIPV4ServerOk())
    {
        StopIPV4();
        m_button_ipv4_gateway_start_stop->SetLabel(_T("启动"));
        LocalLog(_T("IPV4服务器已停止！\r\n"));
    }
    else
    {
        StartIPV4(localhost?_T("localhost"):_T("0.0.0.0"),port);
        if(IsIPV4ServerOk())
        {
            m_button_ipv4_gateway_start_stop->SetLabel(_T("停止"));
            LocalLog(_T("IPV4服务器已启动！\r\n"));
        }
        else
        {
            wxMessageBox(_T("启动失败,请更换端口号后重试!"),_T("错误"));
        }
    }

    SerialportConfigEnable(!(IsIPV6ServerOk() | IsIPV4ServerOk()));

}

void ModbusSessionTCPGatewaySerialportGui::OnButtonClick_IPV6_Gateway_Start_Stop( wxCommandEvent& event )
{
    if(m_comboBox_Serialport_Name->GetValue().IsEmpty())
    {
        LocalLog(_T("未选择串口！\r\n"));
        return;
    }
    unsigned long port=0;
    m_choice_port->GetStringSelection().ToULong(&port);
    bool localhost=m_checkBox_localhost->IsChecked();
    if(IsIPV6ServerOk())
    {
        StopIPV6();
        m_button_ipv6_gateway_start_stop->SetLabel(_T("启动"));
        LocalLog(_T("IPV6服务器已停止！\r\n"));
    }
    else
    {
        StartIPV6(localhost?_T("localhost"):_T("::"),port);
        if(IsIPV6ServerOk())
        {
            m_button_ipv6_gateway_start_stop->SetLabel(_T("停止"));
            LocalLog(_T("IPV6服务器已启动！\r\n"));
        }
        else
        {
            wxMessageBox(_T("启动失败,请更换端口号后重试!"),_T("错误"));
        }
    }

    SerialportConfigEnable(!(IsIPV6ServerOk() | IsIPV4ServerOk()));
}

void ModbusSessionTCPGatewaySerialportGui::OnTimer_UIUpdate( wxTimerEvent& event )
{
    if(m_comboBox_Serialport_Name->GetCount()==0)
    {
        UpdateSerialportList();
    }
    {
        wxString message;
        while(wxMSGQUEUE_NO_ERROR==logqueue.ReceiveTimeout(0,message))
        {
            m_textCtrl_Log->AppendText(message);
        }
    }
}

void ModbusSessionTCPGatewaySerialportGui::OnModbusRTUBegin()
{
    LocalLog(_T("ModbusRTU请求开始！\r\n"));
}
void ModbusSessionTCPGatewaySerialportGui::OnModbusRTUSend(std::string request)
{
    if(request.length() > MODBUS_RTU_MAX_ADU_LENGTH)
    {
        request=request.substr(0,MODBUS_RTU_MAX_ADU_LENGTH);
    }
    char strbuff[MODBUS_RTU_MAX_ADU_LENGTH*3+1];
    hbase16_encode_with_null_terminator(strbuff,sizeof(strbuff),(const uint8_t *)request.c_str(),request.length());
    LocalLog(wxString::Format(_T("ModbusRTU请求:%s\r\n"),wxString::FromUTF8(strbuff)));
}
void ModbusSessionTCPGatewaySerialportGui::OnModbusRTUReceive(std::string reply)
{
    if(reply.length() > MODBUS_RTU_MAX_ADU_LENGTH)
    {
        reply=reply.substr(0,MODBUS_RTU_MAX_ADU_LENGTH);
    }
    char strbuff[MODBUS_RTU_MAX_ADU_LENGTH*3+1];
    hbase16_encode_with_null_terminator(strbuff,sizeof(strbuff),(const uint8_t *)reply.c_str(),reply.length());
    LocalLog(wxString::Format(_T("ModbusRTU响应:%s\r\n"),wxString::FromUTF8(strbuff)));
}
void ModbusSessionTCPGatewaySerialportGui::OnModbusRTUEnd()
{
    LocalLog(_T("ModbusRTU请求结束！\r\n"));
}

void ModbusSessionTCPGatewaySerialportGui::UpdateSerialportList()
{
    {
        m_comboBox_Serialport_Name->Clear();
        struct sp_port **port_list=NULL;
        if(SP_OK==sp_list_ports(&port_list))
        {
            for(struct sp_port ** port_ptr=port_list; (*port_ptr)!=NULL; port_ptr++)
            {
                struct sp_port *port=(*port_ptr);
                LocalLog(wxString::Format(_T("已发现串口 %s (%s)!\r\n"),wxString::FromUTF8(sp_get_port_name(port)),wxString::FromUTF8(sp_get_port_description(port))));
                m_comboBox_Serialport_Name->Insert(wxString::FromUTF8(sp_get_port_name(port)),0);
            }
        }
        if(port_list!=NULL)
        {
            sp_free_port_list(port_list);
        }
        if(m_comboBox_Serialport_Name->GetValue().IsEmpty() && m_comboBox_Serialport_Name->GetCount() > 0)
        {
            m_comboBox_Serialport_Name->SetSelection(0);
        }
    }
}

void ModbusSessionTCPGatewaySerialportGui::SerialportConfigEnable(bool enable)
{
    if(!enable)
    {
        serialport_name=m_comboBox_Serialport_Name->GetValue();
        {
            wxString val=m_textCtrl_Serialport_Baudrate->GetValue();
            unsigned int int_val=115200;
            val.ToUInt(&int_val);
            sp_set_config_baudrate(&GetSerialportConfig(),int_val);
        }
        {
            wxString val=m_choice_Serialport_Bits->GetStringSelection();
            unsigned int int_val=8;
            val.ToUInt(&int_val);
            sp_set_config_bits(&GetSerialportConfig(),int_val);
        }
        {
            enum sp_parity Parity=SP_PARITY_NONE;
            const char P=m_choice_Serialport_Stopbits->GetStringSelection().ToStdString().c_str()[0];
            switch(P)
            {
            case 'N':
            {
                Parity=SP_PARITY_NONE;
            }
            break;
            case 'O':
            {
                Parity=SP_PARITY_ODD;
            }
            break;
            case 'E':
            {
                Parity=SP_PARITY_EVEN;
            }
            break;
            case 'M':
            {
                Parity=SP_PARITY_MARK;
            }
            break;
            case 'S':
            {
                Parity=SP_PARITY_SPACE;
            }
            break;
            default:
            {

            }
            break;
            }
            sp_set_config_parity(&GetSerialportConfig(),Parity);
        }
        {
            wxString val=m_choice_Serialport_Stopbits->GetStringSelection();
            unsigned int int_val=1;
            val.ToUInt(&int_val);
            sp_set_config_stopbits(&GetSerialportConfig(),int_val);
        }
        {
            enum sp_flowcontrol Flowcontrol=SP_FLOWCONTROL_NONE;
            const char F=m_choice_Serialport_Flowcontrol->GetStringSelection().ToStdString().c_str()[0];
            switch(F)
            {
            case 'N':
            {
                Flowcontrol=SP_FLOWCONTROL_NONE;
            }
            break;
            case 'R':
            {
                Flowcontrol=SP_FLOWCONTROL_RTSCTS;
            }
            break;
            case 'X':
            {
                Flowcontrol=SP_FLOWCONTROL_XONXOFF;
            }
            break;
            case 'D':
            {
                Flowcontrol=SP_FLOWCONTROL_DTRDSR;
            }
            break;
            default:
            {

            }
            break;
            }
            sp_set_config_flowcontrol(&GetSerialportConfig(),Flowcontrol);
        }
        {
            wxString val=m_choice_Modbus_Request_Timeout->GetStringSelection();
            unsigned int int_val=1;
            val.ToUInt(&int_val);
            ModbusRTURequestTimeout=int_val;
        }
    }
    m_comboBox_Serialport_Name->Enable(enable);
    m_textCtrl_Serialport_Baudrate->Enable(enable);
    m_choice_Serialport_Bits->Enable(enable);
    m_choice_Serialport_Parity->Enable(enable);
    m_choice_Serialport_Stopbits->Enable(enable);
    m_choice_Serialport_Flowcontrol->Enable(enable);
    m_choice_Modbus_Request_Timeout->Enable(enable);
}
