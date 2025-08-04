#include "modbussessiontcpclientgui.h"
#include "wx/log.h"
#include "wx/msgdlg.h"
#include "wx/grid.h"
#include <string>

ModbusSessionTCPClientGui::ModbusSessionTCPClientGui(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ):modbussessiontcpclientbase(parent,id,pos,size,style,name)
{
    {
        //初始化线圈表格
        m_grid_coils->SetColLabelValue(0,_T("线圈值"));
        for(size_t i=0; i<m_grid_coils->GetNumberRows(); i++)
        {
            m_grid_coils->SetCellEditor(i,0,new wxGridCellBoolEditor());
            m_grid_coils->SetCellRenderer(i,0,new wxGridCellBoolRenderer());
            m_grid_coils->SetRowLabelValue(i,std::to_string(i).c_str());
            m_grid_coils->SetRowLabelAlignment(wxALIGN_LEFT,wxALIGN_CENTER);
        }
    }

    {
        //初始化离散输入表格
        m_grid_discrete_input->SetColLabelValue(0,_T("离散输入值"));
        for(size_t i=0; i<m_grid_discrete_input->GetNumberRows(); i++)
        {
            m_grid_discrete_input->SetCellEditor(i,0,new wxGridCellBoolEditor());
            m_grid_discrete_input->SetCellRenderer(i,0,new wxGridCellBoolRenderer());
            m_grid_discrete_input->SetRowLabelValue(i,std::to_string(i).c_str());
            m_grid_discrete_input->SetRowLabelAlignment(wxALIGN_LEFT,wxALIGN_CENTER);
        }
        m_grid_discrete_input->EnableEditing(false);
    }

    {
        //初始化保持寄存器表格
        m_grid_holding_registers->SetColLabelValue(0,_T("寄存器值"));
        for(size_t i=0; i<m_grid_holding_registers->GetNumberRows(); i++)
        {
            m_grid_holding_registers->SetCellEditor(i,0,new wxGridCellNumberEditor(0,0xFFFF));
            m_grid_holding_registers->SetCellRenderer(i,0,new wxGridCellNumberRenderer());
            m_grid_holding_registers->SetCellAlignment(i,0,wxALIGN_CENTER,wxALIGN_CENTER);
            m_grid_holding_registers->SetCellOverflow(i,0,true);
            m_grid_holding_registers->SetRowLabelValue(i,std::to_string(i).c_str());
            m_grid_holding_registers->SetRowLabelAlignment(wxALIGN_LEFT,wxALIGN_CENTER);
        }
    }

    {
        //初始化输入寄存器表格
        m_grid_input_registers->SetColLabelValue(0,_T("寄存器值"));
        for(size_t i=0; i<m_grid_input_registers->GetNumberRows(); i++)
        {
            m_grid_input_registers->SetCellEditor(i,0,new wxGridCellNumberEditor(0,0xFFFF));
            m_grid_input_registers->SetCellRenderer(i,0,new wxGridCellNumberRenderer());
            m_grid_input_registers->SetCellAlignment(i,0,wxALIGN_CENTER,wxALIGN_CENTER);
            m_grid_input_registers->SetCellOverflow(i,0,true);
            m_grid_input_registers->SetRowLabelValue(i,std::to_string(i).c_str());
            m_grid_input_registers->SetRowLabelAlignment(wxALIGN_LEFT,wxALIGN_CENTER);
        }
        m_grid_input_registers->EnableEditing(false);
    }


}

void ModbusSessionTCPClientGui::OnSetFocus( wxFocusEvent& event )
{
    UpdateState();
}

void ModbusSessionTCPClientGui::OnButtonClick_Coils_Read( wxCommandEvent& event )
{
    if(!ModbusSessionTCPClient::IsConnected())
    {
        wxMessageBox(_T("未连接服务器"),_T("错误"));
        return;
    }
    uint16_t addr=0;
    {
        wxString addr_string=m_textCtrl_coils_addr_base->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if(num < 0x10000)
            {
                isValid=true;
                addr=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("地址错误或者超范围"),_T("错误"));
            return;
        }
    }
    size_t   length=0;
    {
        wxString addr_string=m_textCtrl_coils_addr_length->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if((addr+num < 0x10000) && num <= MODBUS_MAX_READ_BITS && num > 0)
            {
                isValid=true;
                length=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("长度错误或者超范围"),_T("错误"));
            return;
        }
    }

    LocalLog(_T("准备读取线圈addr=%d,length=%d"),(int)addr,(int)length);

    if(!ModbusReadCoils(addr,length))
    {
        wxMessageBox(_T("读取失败"),_T("错误"));
    }

    UpdateModbusCoils();

}

void ModbusSessionTCPClientGui::OnButtonClick_Coils_Write( wxCommandEvent& event )
{
    if(!ModbusSessionTCPClient::IsConnected())
    {
        wxMessageBox(_T("未连接服务器"),_T("错误"));
        return;
    }
    uint16_t addr=0;
    {
        wxString addr_string=m_textCtrl_coils_addr_base->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if(num < 0x10000 )
            {
                isValid=true;
                addr=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("地址错误或者超范围"),_T("错误"));
            return;
        }
    }
    size_t   length=0;
    {
        wxString addr_string=m_textCtrl_coils_addr_length->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if((addr+num < 0x10000) && num <= MODBUS_MAX_WRITE_BITS && num > 0)
            {
                isValid=true;
                length=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("长度错误或者超范围"),_T("错误"));
            return;
        }
    }

    LocalLog(_T("准备写入线圈addr=%d,length=%d"),(int)addr,(int)length);

    if(!ModbusWriteCoils(addr,length))
    {
        wxMessageBox(_T("写入失败"),_T("错误"));
    }

}

void ModbusSessionTCPClientGui::OnButtonClick_Coils_Write_Single( wxCommandEvent& event )
{
    if(!ModbusSessionTCPClient::IsConnected())
    {
        wxMessageBox(_T("未连接服务器"),_T("错误"));
        return;
    }
    uint16_t addr=0;
    {
        wxString addr_string=m_textCtrl_coils_addr_base->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if(num < 0x10000 )
            {
                isValid=true;
                addr=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("地址错误或者超范围"),_T("错误"));
            return;
        }
    }

    LocalLog(_T("准备写入单个线圈addr=%d"),(int)addr);

    if(!ModbusWriteSingleCoil(addr))
    {
        wxMessageBox(_T("写入失败"),_T("错误"));
    }

}

void ModbusSessionTCPClientGui::OnButtonClick_Discrete_Input_Read( wxCommandEvent& event )
{
    if(!ModbusSessionTCPClient::IsConnected())
    {
        wxMessageBox(_T("未连接服务器"),_T("错误"));
        return;
    }
    uint16_t addr=0;
    {
        wxString addr_string=m_textCtrl_discrete_input_addr_base->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if(num < 0x10000)
            {
                isValid=true;
                addr=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("地址错误或者超范围"),_T("错误"));
            return;
        }
    }
    size_t   length=0;
    {
        wxString addr_string=m_textCtrl_discrete_input_addr_length->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if((addr+num < 0x10000) && num <= MODBUS_MAX_READ_BITS && num > 0)
            {
                isValid=true;
                length=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("长度错误或者超范围"),_T("错误"));
            return;
        }
    }

    LocalLog(_T("准备读取离散输入addr=%d,length=%d"),(int)addr,(int)length);

    if(!ModbusReadDiscreteInput(addr,length))
    {
        wxMessageBox(_T("读取失败"),_T("错误"));
    }

    UpdateModbusDiscreteInput();
}

void ModbusSessionTCPClientGui::OnButtonClick_Holding_Registers_Read( wxCommandEvent& event )
{
    if(!ModbusSessionTCPClient::IsConnected())
    {
        wxMessageBox(_T("未连接服务器"),_T("错误"));
        return;
    }
    uint16_t addr=0;
    {
        wxString addr_string=m_textCtrl_holding_registers_addr_base->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if(num < 0x10000)
            {
                isValid=true;
                addr=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("地址错误或者超范围"),_T("错误"));
            return;
        }
    }
    size_t   length=0;
    {
        wxString addr_string=m_textCtrl_holding_registers_addr_length->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if((addr+num < 0x10000) && num <= MODBUS_MAX_READ_REGISTERS && num > 0)
            {
                isValid=true;
                length=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("长度错误或者超范围"),_T("错误"));
            return;
        }
    }

    LocalLog(_T("准备读取保持寄存器addr=%d,length=%d"),(int)addr,(int)length);

    if(!ModbusReadHoldingRegisters(addr,length))
    {
        wxMessageBox(_T("读取失败"),_T("错误"));
    }

    UpdateModbusHoldingRegisters();

}

void ModbusSessionTCPClientGui::OnButtonClick_Holding_Registers_Write( wxCommandEvent& event )
{
    if(!ModbusSessionTCPClient::IsConnected())
    {
        wxMessageBox(_T("未连接服务器"),_T("错误"));
        return;
    }
    uint16_t addr=0;
    {
        wxString addr_string=m_textCtrl_holding_registers_addr_base->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if(num < 0x10000 )
            {
                isValid=true;
                addr=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("地址错误或者超范围"),_T("错误"));
            return;
        }
    }
    size_t   length=0;
    {
        wxString addr_string=m_textCtrl_holding_registers_addr_length->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if((addr+num < 0x10000) && num <= MODBUS_MAX_WRITE_REGISTERS && num > 0)
            {
                isValid=true;
                length=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("长度错误或者超范围"),_T("错误"));
            return;
        }
    }

    LocalLog(_T("准备写入保持寄存器addr=%d,length=%d"),(int)addr,(int)length);

    if(!ModbusWriteHoldingRegisters(addr,length))
    {
        wxMessageBox(_T("写入失败"),_T("错误"));
    }

}

void ModbusSessionTCPClientGui::OnButtonClick_Holding_Registers_Write_Single( wxCommandEvent& event )
{
    if(!ModbusSessionTCPClient::IsConnected())
    {
        wxMessageBox(_T("未连接服务器"),_T("错误"));
        return;
    }
    uint16_t addr=0;
    {
        wxString addr_string=m_textCtrl_holding_registers_addr_base->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if(num < 0x10000 )
            {
                isValid=true;
                addr=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("地址错误或者超范围"),_T("错误"));
            return;
        }
    }

    LocalLog(_T("准备写入单个保持寄存器addr=%d"),(int)addr);

    if(!ModbusWriteSingleHoldingRegister(addr))
    {
        wxMessageBox(_T("写入失败"),_T("错误"));
    }

}

void ModbusSessionTCPClientGui::OnButtonClick_Input_Registers_Read( wxCommandEvent& event )
{
    if(!ModbusSessionTCPClient::IsConnected())
    {
        wxMessageBox(_T("未连接服务器"),_T("错误"));
        return;
    }
    uint16_t addr=0;
    {
        wxString addr_string=m_textCtrl_input_registers_addr_base->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if(num < 0x10000)
            {
                isValid=true;
                addr=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("地址错误或者超范围"),_T("错误"));
            return;
        }
    }
    size_t   length=0;
    {
        wxString addr_string=m_textCtrl_input_registers_addr_length->GetValue();
        unsigned int num=0;
        bool isValid=false;
        if(addr_string.ToUInt(&num))
        {
            if((addr+num < 0x10000) && num <= MODBUS_MAX_READ_REGISTERS && num > 0)
            {
                isValid=true;
                length=num;
            }
        }
        if(!isValid)
        {
            wxMessageBox(_T("长度错误或者超范围"),_T("错误"));
            return;
        }
    }

    LocalLog(_T("准备读取输入寄存器addr=%d,length=%d"),(int)addr,(int)length);

    if(!ModbusReadInputRegisters(addr,length))
    {
        wxMessageBox(_T("读取失败"),_T("错误"));
    }

    UpdateModbusInputRegisters();
}


void ModbusSessionTCPClientGui::OnButtonClick_Connect_DisConnect( wxCommandEvent& event )
{
    if(ModbusSessionTCPClient::IsConnected())
    {
        LocalLog(_T("断开连接！"));
        ModbusSessionTCPClient::Close();
    }
    else
    {
        LocalLog(_T("开始连接！"));
        std::string addr_str=m_textCtrl_ip->GetValue().ToStdString();
        int port=502;
        {
            size_t addr_end_pos=addr_str.find(']');
            if(addr_end_pos==std::string::npos)
            {
                addr_end_pos=0;
            }
            if(addr_str.find(':',addr_end_pos)!=std::string::npos)
            {
                std::string port_str=addr_str.substr(addr_str.find(':',addr_end_pos)+1);
                addr_str=addr_str.substr(0,addr_str.find(':',addr_end_pos));
                if(port_str.length()>0)
                {
                    port=std::stol(port_str);
                }
            }
        }

        LocalLog(wxString(_T("连接 "))+wxString::FromUTF8(addr_str.c_str())+_T(":%d!"),(int)port);

        bool connect_success=false;
        if(!connect_success)
        {
            wxIPV4address address;
            address.Hostname(addr_str);
            address.Service(port);
            connect_success=ModbusSessionTCPClient::Connect(address);
        }
        if(!connect_success)
        {
            wxIPV6address address;
            address.Hostname(addr_str);
            address.Service(port);
            connect_success=ModbusSessionTCPClient::Connect(address);
        }

        if(!connect_success)
        {
            LocalLog(_T("连接失败！"));
            wxMessageBox(_T("连接失败！"),_T("错误"));
        }
        else
        {
            LocalLog(_T("连接成功！"));
        }
    }

    UpdateState();
}

void ModbusSessionTCPClientGui::OnGridCellChange_Modbus_Coils( wxGridEvent& event )
{
    if(event.GetRow() >=0 && event.GetRow() < 0x10000 && event.GetCol() ==0)
    {
        wxString val_string=m_grid_coils->GetCellValue(event.GetRow(),event.GetCol());
        bool val=!val_string.IsEmpty();
        if(val)
        {
            val=false;
            unsigned int num=0;
            if(val_string.ToUInt(&num))
            {
                val=(num!=0);
            }
        }
        {
            /*
             * 设置进数据映射
             */
            std::lock_guard<std::recursive_mutex> lock(*m_lock);
            m_coils[event.GetRow()]=val;
        }
    }
}

void ModbusSessionTCPClientGui::OnGridCellChange_Modbus_Holding_Registers( wxGridEvent& event )
{
    if(event.GetRow() >=0 && event.GetRow() < 0x10000 && event.GetCol() ==0)
    {
        wxString val_string=m_grid_holding_registers->GetCellValue(event.GetRow(),event.GetCol());
        unsigned int val=0;
        val_string.ToUInt(&val);
        {
            /*
             * 设置进数据映射
             */
            std::lock_guard<std::recursive_mutex> lock(*m_lock);
            m_holding_registers[event.GetRow()]=val;
        }
    }
}

void ModbusSessionTCPClientGui::OnModbusException(uint8_t function_code,uint8_t exception_code)
{
    LocalLog(_T("异常%d(功能%d)"),(int)exception_code,(int)function_code);
    wxMessageBox(_T("发生异常，详情请查看日志！"),_T("错误"));
}

void ModbusSessionTCPClientGui::OnModbusWrite(const uint8_t *adu,size_t adu_length)
{
    if(adu==NULL || adu_length==0)
    {
        return;
    }
    char data_string[MODBUS_TCP_MAX_ADU_LENGTH*2+1]= {0};
    hbase16_encode_with_null_terminator(data_string,sizeof(data_string),adu,adu_length);
    LocalLog(wxString(_T("发送:"))+wxString::FromUTF8(data_string));
}

void ModbusSessionTCPClientGui::OnModbusRead(const uint8_t *buffer,size_t buffer_length)
{
    if(buffer==NULL || buffer_length==0)
    {
        return;
    }
    char data_string[MODBUS_TCP_MAX_ADU_LENGTH*2+1]= {0};
    hbase16_encode_with_null_terminator(data_string,sizeof(data_string),buffer,buffer_length);
    LocalLog(wxString(_T("接收:"))+wxString::FromUTF8(data_string));
}

void ModbusSessionTCPClientGui::UpdateState()
{
    if(ModbusSessionTCPClient::IsConnected())
    {
        m_textCtrl_ip->Enable(false);
        m_button_connect_disconnect->SetLabel(_T("断开"));
    }
    else
    {
        m_textCtrl_ip->Enable(true);
        m_button_connect_disconnect->SetLabel(_T("连接"));
    }
}

void ModbusSessionTCPClientGui::UpdateModbusCoils()
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    for(auto it=m_coils.begin(); it!=m_coils.end(); it++)
    {
        m_grid_coils->SetCellValue((int)it->first,0,it->second?_T("1"):_T(""));
    }
}

void ModbusSessionTCPClientGui::UpdateModbusDiscreteInput()
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    for(auto it=m_discrete_inputs.begin(); it!=m_discrete_inputs.end(); it++)
    {
        m_grid_discrete_input->SetCellValue((int)it->first,0,it->second?_T("1"):_T(""));
    }
}

void ModbusSessionTCPClientGui::UpdateModbusHoldingRegisters()
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    for(auto it=m_holding_registers.begin(); it!=m_holding_registers.end(); it++)
    {
        m_grid_holding_registers->SetCellValue((int)it->first,0,std::to_string((int)it->second));
    }
}

void ModbusSessionTCPClientGui::UpdateModbusInputRegisters()
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    for(auto it=m_input_registers.begin(); it!=m_input_registers.end(); it++)
    {
        m_grid_input_registers->SetCellValue((int)it->first,0,std::to_string((int)it->second));
    }
}

void ModbusSessionTCPClientGui::LocalLog(wxString format,...)
{
    va_list va;
    va_start(va,format);
    wxString msg=wxString::FormatV(format,va);
    va_end(va);
    m_textCtrl_coils_log->AppendText(msg+_T("\n"));
    m_textCtrl_discrete_input_log->AppendText(msg+_T("\n"));
    m_textCtrl_holding_registers_log->AppendText(msg+_T("\n"));
    m_textCtrl_input_registers_log->AppendText(msg+_T("\n"));
    wxLogMessage(_T("Modbus TCP客户端 %p %s"),this,msg);
}

ModbusSessionTCPClientGui::~ModbusSessionTCPClientGui()
{

}
