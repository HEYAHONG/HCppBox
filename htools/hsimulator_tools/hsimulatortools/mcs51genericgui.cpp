#include "mcs51genericgui.h"

#include "hbox.h"
#include "HCPPBox.h"
#include "hrc.h"

#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/msgdlg.h>
#include <wx/aui/auibook.h>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iterator>

/*
 * MCS51模拟器，参考hmini-mcs51实现。
 * 仅保留串口与定时器功能，不包含openblt。
 */
class MCS51VM: public hsimmcs51huge
{
public:
    explicit MCS51VM(MCS51GenericGui* owner)
        : m_owner(owner),m_timer_escaped_time_ns(0)
    {
        memset(m_rom,0,sizeof(m_rom));
    }

    void vm_init()
    {
        m_timer_escaped_time_ns=0;
        hs_mcs_51_timer_clk_freq_set(timer_get(),1000);
        hs_mcs_51_timer_timer2_enable(timer_get(),true);
        hs_mcs_51_core_set_1t(core_get(),true);
    }

    void load_rom(const uint8_t* rom_data,size_t rom_length)
    {
        hs_mcs_51_core_reset(core_get());
        memset(m_rom,0,sizeof(m_rom));
        if(rom_data!=NULL && rom_length>0)
        {
            memcpy(m_rom,rom_data,rom_length>sizeof(m_rom)?sizeof(m_rom):rom_length);
        }
        hs_mcs_51_rom_t rom_mcs51=HS_MCS_51_ROM_INITIALIZER;
        rom_mcs51.code=m_rom;
        rom_mcs51.len=sizeof(m_rom);
        rom_set(rom_mcs51);
    }

    void vm_execute(uint64_t escaped_ns,int tick_count)
    {
        update_timer(escaped_ns);
        hs_mcs_51_core_tick(core_get(),tick_count);
    }

protected:
    bool on_uart(hs_mcs_51_serial_t* serial,hs_mcs_51_serial_io_t io_type,uint16_t* data) override
    {
        if(serial==NULL || m_owner==NULL)
        {
            return false;
        }
        switch(io_type)
        {
        case HS_MCS_51_SERIAL_IO_TRANSMIT:
        {
            if(data!=NULL)
            {
                uint8_t value=(uint8_t)(*data);
                m_owner->AppendOutput(&value,1);
            }
        }
        break;
        case HS_MCS_51_SERIAL_IO_RECEIVE:
        {
            if(data!=NULL && m_owner->HasInput())
            {
                (*data)=m_owner->GetInput();
            }
        }
        break;
        default:
        {
            if(m_owner->HasInput())
            {
                hs_mcs_51_serial_status_dataready_set(core_get(),serial_get(),0);
            }
        }
        break;
        }
        return true;
    }

private:
    void update_timer(uint64_t escaped_time_ns)
    {
        m_timer_escaped_time_ns+=escaped_time_ns;
        uint64_t timer_tick_time_ns=1000000000ULL/hs_mcs_51_timer_clk_freq_get(timer_get());
        while(m_timer_escaped_time_ns>timer_tick_time_ns)
        {
            m_timer_escaped_time_ns-=timer_tick_time_ns;
            hs_mcs_51_timer_external_clk_tick(core_get(),timer_get(),true);
        }
    }

    MCS51GenericGui* m_owner;
    uint64_t m_timer_escaped_time_ns;
    uint8_t m_rom[256*1024];
};

static uint64_t get_time_ns()
{
    htimespec_t ts= {0};
    hclock_gettime(HCLOCK_MONOTONIC,&ts);
    return (uint64_t)ts.tv_sec*1000000000ULL+(uint64_t)ts.tv_nsec;
}

MCS51GenericGui::MCS51GenericGui(wxWindow* parent,wxWindowID id,
                                 const wxPoint& pos,const wxSize& size,
                                 long style,const wxString& name)
    : wxPanel(parent,id,pos,size,style,name),
      m_listbook_mcs51(NULL),
      m_panel_mcs51_settings(NULL),
      m_panel_mcs51_serialport(NULL),
      m_HCPPTerminal_Serialport0(NULL),
      m_filePicker_mcs51_program(NULL),
      m_button_mcs51_start(NULL),
      m_button_mcs51_stop(NULL),
      m_button_mcs51_quit(NULL),
      m_running(false),
      m_stop_requested(false),
      m_finished(false),
      m_vm(new MCS51VM(this))
{
    wxBoxSizer* topSizer=new wxBoxSizer(wxVERTICAL);
    m_listbook_mcs51=new wxListbook(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLB_LEFT);
    m_panel_mcs51_settings=new wxPanel(m_listbook_mcs51,wxID_ANY);
    m_panel_mcs51_serialport=new wxPanel(m_listbook_mcs51,wxID_ANY);

    /*
     * 设置页
     */
    wxBoxSizer* settingsSizer=new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer* grid=new wxFlexGridSizer(0,2,8,8);
    grid->AddGrowableCol(1,1);
    grid->Add(new wxStaticText(m_panel_mcs51_settings,wxID_ANY,_T("程序文件:")),0,wxALIGN_CENTER_VERTICAL);
    m_filePicker_mcs51_program=new wxFilePickerCtrl(m_panel_mcs51_settings,wxID_ANY,wxEmptyString,
            _T("选择程序文件"),
            _T("程序文件 (*.hex;*.ihx;*.bin)|*.hex;*.ihx;*.bin|所有文件 (*.*)|*.*"),
            wxDefaultPosition,wxDefaultSize,wxFLP_DEFAULT_STYLE);
    grid->Add(m_filePicker_mcs51_program,1,wxEXPAND);
    settingsSizer->Add(grid,0,wxEXPAND|wxALL,10);
    m_panel_mcs51_settings->SetSizer(settingsSizer);

    /*
     * 串口页
     */
    wxBoxSizer* serialSizer=new wxBoxSizer(wxVERTICAL);
    m_HCPPTerminal_Serialport0=new HCPPTerminal(m_panel_mcs51_serialport,wxID_ANY,wxDefaultPosition);
    serialSizer->Add(m_HCPPTerminal_Serialport0,1,wxEXPAND|wxALL,5);
    m_panel_mcs51_serialport->SetSizer(serialSizer);
    m_HCPPTerminal_Serialport0->set_input_callback([this](wxString input)
    {
        PushInput(input.ToStdString());
    });

    m_listbook_mcs51->AddPage(m_panel_mcs51_settings,_T("设置"),true);
    m_listbook_mcs51->AddPage(m_panel_mcs51_serialport,_T("串口"),false);
    topSizer->Add(m_listbook_mcs51,1,wxEXPAND|wxALL,5);

    wxBoxSizer* buttonSizer=new wxBoxSizer(wxHORIZONTAL);
    m_button_mcs51_start=new wxButton(this,wxID_ANY,_T("启动"));
    m_button_mcs51_stop=new wxButton(this,wxID_ANY,_T("停止"));
    m_button_mcs51_quit=new wxButton(this,wxID_ANY,_T("退出虚拟机"));
    m_button_mcs51_stop->Enable(false);
    buttonSizer->Add(m_button_mcs51_start,0,wxALL,5);
    buttonSizer->Add(m_button_mcs51_stop,0,wxALL,5);
    buttonSizer->Add(m_button_mcs51_quit,0,wxALL,5);
    topSizer->Add(buttonSizer,0);
    SetSizer(topSizer);

    m_button_mcs51_start->Bind(wxEVT_BUTTON,&MCS51GenericGui::OnButtonClick_MCS51_Start,this);
    m_button_mcs51_stop->Bind(wxEVT_BUTTON,&MCS51GenericGui::OnButtonClick_MCS51_Stop,this);
    m_button_mcs51_quit->Bind(wxEVT_BUTTON,&MCS51GenericGui::OnButtonClick_MCS51_Quit,this);

    m_timer_mcs51_ms_timer.SetOwner(this);
    Bind(wxEVT_TIMER,&MCS51GenericGui::OnTimer_MCS51_MS_Timer,this);
    m_timer_mcs51_ms_timer.Start(10);
}

MCS51GenericGui::~MCS51GenericGui()
{
    StopVM();
}

void MCS51GenericGui::AppendOutput(const void* data,size_t size)
{
    if(data==NULL || size==0)
    {
        return;
    }
    std::lock_guard<std::mutex> lock(m_io_mutex);
    m_output.append((const char *)data,size);
}

void MCS51GenericGui::PushInput(const std::string& data)
{
    std::lock_guard<std::mutex> lock(m_io_mutex);
    m_input.append(data);
}

bool MCS51GenericGui::HasInput()
{
    std::lock_guard<std::mutex> lock(m_io_mutex);
    return !m_input.empty();
}

uint8_t MCS51GenericGui::GetInput()
{
    std::lock_guard<std::mutex> lock(m_io_mutex);
    if(m_input.empty())
    {
        return 0;
    }
    uint8_t ret=(uint8_t)m_input.front();
    m_input.erase(0,1);
    return ret;
}

void MCS51GenericGui::DrainOutput()
{
    std::string data;
    {
        std::lock_guard<std::mutex> lock(m_io_mutex);
        if(m_output.empty())
        {
            return;
        }
        data.swap(m_output);
    }
    if(m_HCPPTerminal_Serialport0!=NULL && !data.empty())
    {
        m_HCPPTerminal_Serialport0->DisplayCharsUnsafe(wxString::FromUTF8(data.data(),data.size()));
        m_HCPPTerminal_Serialport0->Refresh(false);
    }
}

void MCS51GenericGui::SetRunningUI(bool running)
{
    m_button_mcs51_start->Enable(!running);
    m_button_mcs51_stop->Enable(running);
    m_panel_mcs51_settings->Enable(!running);
}

void MCS51GenericGui::OnButtonClick_MCS51_Start(wxCommandEvent& event)
{
    (void)event;
    if(m_running.load())
    {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(m_io_mutex);
        m_input.clear();
        m_output.clear();
    }
    m_stop_requested.store(false);
    m_finished.store(false);

    wxString error;
    if(!LoadProgram(error))
    {
        wxMessageBox(error,_T("启动失败"),wxOK|wxICON_ERROR,this);
        return;
    }

    m_running.store(true);
    SetRunningUI(true);
    m_listbook_mcs51->SetSelection(1);
    m_HCPPTerminal_Serialport0->SetFocus();

    m_thread=std::thread([this]()
    {
        RunVM();
        m_finished.store(true);
    });
}

void MCS51GenericGui::OnButtonClick_MCS51_Stop(wxCommandEvent& event)
{
    (void)event;
    m_stop_requested.store(true);
    m_button_mcs51_stop->Enable(false);
}

void MCS51GenericGui::OnButtonClick_MCS51_Quit(wxCommandEvent& event)
{
    (void)event;
    StopVM();
    if(GetParent()!=NULL && dynamic_cast<wxAuiNotebook*>(GetParent())!=nullptr)
    {
        int index=((wxAuiNotebook*)GetParent())->GetPageIndex(this);
        if(index!=wxNOT_FOUND)
        {
            ((wxAuiNotebook*)GetParent())->DeletePage(index);
        }
    }
}

void MCS51GenericGui::OnTimer_MCS51_MS_Timer(wxTimerEvent& event)
{
    (void)event;
    if(m_finished.exchange(false))
    {
        if(m_thread.joinable())
        {
            m_thread.join();
        }
        m_running.store(false);
        SetRunningUI(false);
    }
    DrainOutput();
}

void MCS51GenericGui::StopVM()
{
    m_stop_requested.store(true);
    m_timer_mcs51_ms_timer.Stop();
    if(m_thread.joinable())
    {
        m_thread.join();
    }
    m_running.store(false);
}

bool MCS51GenericGui::LoadProgram(wxString& error)
{
    std::string filename=m_filePicker_mcs51_program->GetPath().ToStdString();
    if(filename.empty())
    {
        error=_T("请选择程序文件!");
        return false;
    }

    bool is_hex=false;
    if(filename.length()>=3)
    {
        std::string suffix=filename.substr(filename.length()-3);
        std::transform(suffix.begin(),suffix.end(),suffix.begin(),::toupper);
        if(suffix=="HEX" || suffix=="IHX")
        {
            is_hex=true;
        }
    }

    std::ifstream file;
    file.open(filename.c_str(),is_hex?(std::ios::in):(std::ios::in|std::ios::binary));
    if(!file.is_open())
    {
        error=_T("无法打开程序文件:")+wxString::FromUTF8(filename.c_str());
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
    file.close();

    uint8_t rom[256*1024]= {0};
    if(is_hex)
    {
        hintelhex_reader reader;
        hintelhex_reader_init(&reader,[](hintelhex_reader_t* reader,hintelhex_reader_operate_t op,uint32_t address,const uint8_t* data,size_t data_len)
        {
            uint8_t* code=(uint8_t *)reader->usr;
            if(op==HINTELHEX_READER_OPERATE_DATA)
            {
                if(address+data_len<256*1024)
                {
                    memcpy(&code[address],data,data_len);
                }
            }
        },rom);
        hintelhex_reader_input(&reader,content.c_str(),content.length());
    }
    else
    {
        memcpy(rom,content.c_str(),content.length()>sizeof(rom)?sizeof(rom):content.length());
    }

    m_vm->load_rom(rom,sizeof(rom));
    return true;
}

void MCS51GenericGui::RunVM()
{
    m_vm->vm_init();
    uint64_t last=get_time_ns();
    while(!m_stop_requested.load())
    {
        uint64_t now=get_time_ns();
        m_vm->vm_execute(now-last,32);
        last=now;
        std::this_thread::yield();
    }
}
