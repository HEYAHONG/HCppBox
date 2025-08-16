#include "modbussessiontcpgatewaysimulatorgui.h"
#include <wx/arrstr.h>
#include <wx/msgdlg.h>

ModbusSessionTCPGatewaySimulatorGui::ModbusSessionTCPGatewaySimulatorGui(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ):modbussessiontcpgatewaysimulatorbase(parent,id,pos,size,style,name)
{
    {
        //初始化模拟器数据
        m_grid_simulator_data->SetColLabelValue(0,_T("线圈值"));
        m_grid_simulator_data->SetColLabelValue(1,_T("离散输入"));
        m_grid_simulator_data->SetColLabelValue(2,_T("保持寄存器"));
        m_grid_simulator_data->SetColLabelValue(3,_T("输入寄存器"));
        for(size_t i=0; i<m_grid_simulator_data->GetNumberRows(); i++)
        {
            m_grid_simulator_data->SetCellEditor(i,0,new wxGridCellBoolEditor());
            m_grid_simulator_data->SetCellRenderer(i,0,new wxGridCellBoolRenderer());
            m_grid_simulator_data->SetCellEditor(i,1,new wxGridCellBoolEditor());
            m_grid_simulator_data->SetCellRenderer(i,1,new wxGridCellBoolRenderer());
            m_grid_simulator_data->SetCellEditor(i,2,new wxGridCellNumberEditor(0,0xFFFF));
            m_grid_simulator_data->SetCellRenderer(i,2,new wxGridCellNumberRenderer());
            m_grid_simulator_data->SetCellAlignment(i,2,wxALIGN_CENTER,wxALIGN_CENTER);
            m_grid_simulator_data->SetCellEditor(i,3,new wxGridCellNumberEditor(0,0xFFFF));
            m_grid_simulator_data->SetCellRenderer(i,3,new wxGridCellNumberRenderer());
            m_grid_simulator_data->SetCellAlignment(i,3,wxALIGN_CENTER,wxALIGN_CENTER);
            m_grid_simulator_data->SetRowLabelValue(i,std::to_string(i).c_str());
            m_grid_simulator_data->SetRowLabelAlignment(wxALIGN_LEFT,wxALIGN_CENTER);
        }
    }

}

ModbusSessionTCPGatewaySimulatorGui::~ModbusSessionTCPGatewaySimulatorGui()
{

}

void ModbusSessionTCPGatewaySimulatorGui::OnGridCellChange_Simulator_Data( wxGridEvent& event )
{
    if(event.GetRow() >=0 && event.GetRow() < 0x10000)
    {
        wxString val_string=m_grid_simulator_data->GetCellValue(event.GetRow(),event.GetCol());
        switch(event.GetCol())
        {
        case 0:
        {
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
        break;
        case 1:
        {
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
                m_discrete_inputs[event.GetRow()]=val;
            }
        }
        break;
        case 2:
        {
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
        break;
        case 3:
        {
            unsigned int val=0;
            val_string.ToUInt(&val);
            {
                /*
                 * 设置进数据映射
                 */
                std::lock_guard<std::recursive_mutex> lock(*m_lock);
                m_input_registers[event.GetRow()]=val;
            }
        }
        break;
        default:
        {

        }
        break;
        }
    }
}

void ModbusSessionTCPGatewaySimulatorGui::OnButtonClick_IPV4_Gateway_Start_Stop( wxCommandEvent& event )
{
    unsigned long port=0;
    m_choice_port->GetStringSelection().ToULong(&port);
    bool localhost=m_checkBox_localhost->IsChecked();
    if(IsIPV4ServerOk())
    {
        StopIPV4();
        m_button_ipv4_gateway_start_stop->SetLabel(_T("启动"));
    }
    else
    {
        StartIPV4(localhost?_T("localhost"):_T("0.0.0.0"),port);
        if(IsIPV4ServerOk())
        {
            m_button_ipv4_gateway_start_stop->SetLabel(_T("停止"));
        }
        else
        {
            wxMessageBox(_T("启动失败,请更换端口号后重试!"),_T("错误"));
        }
    }
}

void ModbusSessionTCPGatewaySimulatorGui::OnButtonClick_IPV6_Gateway_Start_Stop( wxCommandEvent& event )
{
    unsigned long port=0;
    m_choice_port->GetStringSelection().ToULong(&port);
    bool localhost=m_checkBox_localhost->IsChecked();
    if(IsIPV6ServerOk())
    {
        StopIPV6();
        m_button_ipv6_gateway_start_stop->SetLabel(_T("启动"));
    }
    else
    {
        StartIPV6(localhost?_T("localhost"):_T("::"),port);
        if(IsIPV6ServerOk())
        {
            m_button_ipv6_gateway_start_stop->SetLabel(_T("停止"));
        }
        else
        {
            wxMessageBox(_T("启动失败,请更换端口号后重试!"),_T("错误"));
        }
    }
}

void ModbusSessionTCPGatewaySimulatorGui::OnTimer_Simulator_Update( wxTimerEvent& event )
{
    if(SimulatorDataUpdate)
    {
        OnDataUpdate();
        SimulatorDataUpdate=false;
    }
}

void ModbusSessionTCPGatewaySimulatorGui::OnDataUpdate()
{
    UpdateSimulatorData();
}

void ModbusSessionTCPGatewaySimulatorGui::UpdateSimulatorData()
{
    std::lock_guard<std::recursive_mutex> lock(*m_lock);
    for(auto it=m_coils.begin(); it!=m_coils.end(); it++)
    {
        m_grid_simulator_data->SetCellValue((int)it->first,0,it->second?_T("1"):_T(""));
    }
    for(auto it=m_discrete_inputs.begin(); it!=m_discrete_inputs.end(); it++)
    {
        m_grid_simulator_data->SetCellValue((int)it->first,1,it->second?_T("1"):_T(""));
    }
    for(auto it=m_holding_registers.begin(); it!=m_holding_registers.end(); it++)
    {
        m_grid_simulator_data->SetCellValue((int)it->first,2,std::to_string((int)it->second));
    }
    for(auto it=m_input_registers.begin(); it!=m_input_registers.end(); it++)
    {
        m_grid_simulator_data->SetCellValue((int)it->first,3,std::to_string((int)it->second));
    }
}
