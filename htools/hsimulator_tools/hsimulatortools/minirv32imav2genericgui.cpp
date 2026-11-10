#include "minirv32imav2genericgui.h"

#include "hbox.h"
#include "HCPPBox.h"
#include "hrc.h"

#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/msgdlg.h>
#include <wx/aui/auibook.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

/*
 * mini-rv32imav2模拟器，参考hmini-rv32imav2实现。
 * 仅保留镜像、设备树与串口功能，不包含openblt。
 */
struct MiniRV32IMAImpl
{
    explicit MiniRV32IMAImpl(MiniRV32IMAV2GenericGui* owner)
        : m_owner(owner),m_display_dtb(false)
    {
        memset(&m_machine,0,sizeof(m_machine));
    }

    static MiniRV32IMAV2GenericGui* owner(const hminirv32ima_machine_default64mb_t* machine)
    {
        MiniRV32IMAImpl* self=(MiniRV32IMAImpl *)(((char *)machine)-offsetof(MiniRV32IMAImpl,m_machine));
        return self->m_owner;
    }

    static bool console_has_data(const hminirv32ima_machine_default64mb_t* machine)
    {
        return owner(machine)->HasInput();
    }

    static void console_put_data(const hminirv32ima_machine_default64mb_t* machine,uint8_t data)
    {
        owner(machine)->AppendOutput(&data,1);
    }

    static uint8_t console_get_data(const hminirv32ima_machine_default64mb_t* machine)
    {
        return owner(machine)->GetInput();
    }

    void setup_callbacks()
    {
        m_machine.console_has_data=console_has_data;
        m_machine.console_put_data=console_put_data;
        m_machine.console_get_data=console_get_data;
    }

    bool reset()
    {
        hminirv32ima_machine_default64mb_init(&m_machine);
        setup_callbacks();
        if(!m_image.empty())
        {
            if(!hminirv32ima_machine_default64mb_load_image(&m_machine,m_image.data(),m_image.size()))
            {
                return false;
            }
        }
        if(!m_dtb.empty())
        {
            if(!hminirv32ima_machine_default64mb_load_dtb(&m_machine,m_dtb.data(),m_dtb.size()))
            {
                return false;
            }
        }
        return true;
    }

    MiniRV32IMAV2GenericGui* m_owner;
    hminirv32ima_machine_default64mb_t m_machine;
    std::vector<uint8_t> m_image;
    std::vector<uint8_t> m_dtb;
    bool m_display_dtb;
};

static uint64_t get_time_us()
{
    htimespec_t ts= {0};
    hclock_gettime(HCLOCK_MONOTONIC,&ts);
    return (uint64_t)ts.tv_sec*1000000ULL+(uint64_t)ts.tv_nsec/1000ULL;
}

static void fdt_dump(const void* fdt,std::string& output)
{
    struct NodeCtx
    {
        std::string* output;
        intptr_t address_cells;
        intptr_t size_cells;
        intptr_t blank_space;
    };

    NodeCtx ctx= {&output,2,1,0};
    hlibfdt_traverse_node(fdt,[](const void* fdt,int offset,const char* name,int depth,void* usr)
    {
        NodeCtx* ctx=(NodeCtx *)usr;
        std::string blank_space;
        for(int i=0; i<(depth-1); i++)
        {
            blank_space+="  ";
        }
        if(name==NULL || strlen(name)==0)
        {
            name="/";
        }
        (*ctx->output)+=blank_space+name+"\r\n";
        ctx->blank_space=(intptr_t)blank_space.c_str();
        hlibfdt_traverse_node_property(fdt,offset,[](const void* fdt,int offset,const char* name,const uint8_t* value,size_t value_length,void* usr)
        {
            (void)fdt;
            (void)offset;
            NodeCtx* ctx=(NodeCtx *)usr;
            const char* blank_space=(const char *)ctx->blank_space;
            char buffer[64];
            (*ctx->output)+=blank_space;
            (*ctx->output)+="  ";
            (*ctx->output)+=name;
            (*ctx->output)+=":";
            if(strcmp("#address-cells",name)==0 || strcmp("#size-cells",name)==0)
            {
                uint32_t val=0;
                for(size_t i=0; i<value_length; i++)
                {
                    val=(val<<8)+value[i];
                }
                if(strcmp("#address-cells",name)==0)
                {
                    ctx->address_cells=val;
                }
                else
                {
                    ctx->size_cells=val;
                }
                snprintf(buffer,sizeof(buffer)," %u\r\n",val);
                (*ctx->output)+=buffer;
            }
            else if(strcmp("reg",name)==0)
            {
                for(size_t i=0; i<value_length; i++)
                {
                    if(i==(size_t)(ctx->address_cells*4) || i==(size_t)((ctx->address_cells+ctx->size_cells)*4))
                    {
                        (*ctx->output)+=" ";
                    }
                    snprintf(buffer,sizeof(buffer),"%02X",(unsigned)value[i]);
                    (*ctx->output)+=buffer;
                }
                (*ctx->output)+="\r\n";
            }
            else
            {
                static const char* string_names[]=
                {
                    "model","compatible","status","device_type","mmu-type","riscv,isa","bootargs"
                };
                bool is_string=false;
                for(size_t i=0; i<sizeof(string_names)/sizeof(string_names[0]); i++)
                {
                    if(strcmp(string_names[i],name)==0)
                    {
                        is_string=true;
                        break;
                    }
                }
                if(is_string)
                {
                    size_t string_offset=0;
                    while(string_offset<value_length)
                    {
                        (*ctx->output)+=" ";
                        (*ctx->output)+=(const char *)&value[string_offset];
                        string_offset+=strlen((const char *)&value[string_offset])+1;
                    }
                }
                else
                {
                    for(size_t i=0; i<value_length; i++)
                    {
                        snprintf(buffer,sizeof(buffer),"%02X",(unsigned)value[i]);
                        (*ctx->output)+=buffer;
                    }
                }
                (*ctx->output)+="\r\n";
            }
        },ctx);
    },&ctx);
}

MiniRV32IMAV2GenericGui::MiniRV32IMAV2GenericGui(wxWindow* parent,wxWindowID id,
                                                 const wxPoint& pos,const wxSize& size,
                                                 long style,const wxString& name)
    : wxPanel(parent,id,pos,size,style,name),
      m_listbook_minirv32ima(NULL),
      m_panel_minirv32ima_settings(NULL),
      m_panel_minirv32ima_serialport(NULL),
      m_HCPPTerminal_Serialport0(NULL),
      m_filePicker_minirv32ima_image(NULL),
      m_filePicker_minirv32ima_dtb(NULL),
      m_checkBox_minirv32ima_display_dtb(NULL),
      m_button_minirv32ima_start(NULL),
      m_button_minirv32ima_stop(NULL),
      m_button_minirv32ima_quit(NULL),
      m_running(false),
      m_stop_requested(false),
      m_finished(false),
      m_impl(new MiniRV32IMAImpl(this))
{
    wxBoxSizer* topSizer=new wxBoxSizer(wxVERTICAL);
    m_listbook_minirv32ima=new wxListbook(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLB_LEFT);
    m_panel_minirv32ima_settings=new wxPanel(m_listbook_minirv32ima,wxID_ANY);
    m_panel_minirv32ima_serialport=new wxPanel(m_listbook_minirv32ima,wxID_ANY);

    /*
     * 设置页
     */
    wxBoxSizer* settingsSizer=new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer* grid=new wxFlexGridSizer(0,2,8,8);
    grid->AddGrowableCol(1,1);

    grid->Add(new wxStaticText(m_panel_minirv32ima_settings,wxID_ANY,_T("镜像文件:")),0,wxALIGN_CENTER_VERTICAL);
    m_filePicker_minirv32ima_image=new wxFilePickerCtrl(m_panel_minirv32ima_settings,wxID_ANY,wxEmptyString,
            _T("选择镜像文件"),
            _T("镜像文件 (*.bin;*.Image)|*.bin;*.Image|所有文件 (*.*)|*.*"),
            wxDefaultPosition,wxDefaultSize,wxFLP_DEFAULT_STYLE);
    grid->Add(m_filePicker_minirv32ima_image,1,wxEXPAND);

    grid->Add(new wxStaticText(m_panel_minirv32ima_settings,wxID_ANY,_T("设备树文件:")),0,wxALIGN_CENTER_VERTICAL);
    m_filePicker_minirv32ima_dtb=new wxFilePickerCtrl(m_panel_minirv32ima_settings,wxID_ANY,wxEmptyString,
            _T("选择设备树文件"),
            _T("设备树文件 (*.dtb)|*.dtb|所有文件 (*.*)|*.*"),
            wxDefaultPosition,wxDefaultSize,wxFLP_DEFAULT_STYLE);
    grid->Add(m_filePicker_minirv32ima_dtb,1,wxEXPAND);

    grid->Add(new wxStaticText(m_panel_minirv32ima_settings,wxID_ANY,_T("")),0);
    m_checkBox_minirv32ima_display_dtb=new wxCheckBox(m_panel_minirv32ima_settings,wxID_ANY,_T("启动前显示设备树"));
    grid->Add(m_checkBox_minirv32ima_display_dtb,0);

    settingsSizer->Add(grid,0,wxEXPAND|wxALL,10);
    m_panel_minirv32ima_settings->SetSizer(settingsSizer);

    /*
     * 串口页
     */
    wxBoxSizer* serialSizer=new wxBoxSizer(wxVERTICAL);
    m_HCPPTerminal_Serialport0=new HCPPTerminal(m_panel_minirv32ima_serialport,wxID_ANY,wxDefaultPosition);
    serialSizer->Add(m_HCPPTerminal_Serialport0,1,wxEXPAND|wxALL,5);
    m_panel_minirv32ima_serialport->SetSizer(serialSizer);
    m_HCPPTerminal_Serialport0->set_input_callback([this](wxString input)
    {
        PushInput(input.ToStdString());
    });

    m_listbook_minirv32ima->AddPage(m_panel_minirv32ima_settings,_T("设置"),true);
    m_listbook_minirv32ima->AddPage(m_panel_minirv32ima_serialport,_T("串口"),false);
    topSizer->Add(m_listbook_minirv32ima,1,wxEXPAND|wxALL,5);

    wxBoxSizer* buttonSizer=new wxBoxSizer(wxHORIZONTAL);
    m_button_minirv32ima_start=new wxButton(this,wxID_ANY,_T("启动"));
    m_button_minirv32ima_stop=new wxButton(this,wxID_ANY,_T("停止"));
    m_button_minirv32ima_quit=new wxButton(this,wxID_ANY,_T("退出虚拟机"));
    m_button_minirv32ima_stop->Enable(false);
    buttonSizer->Add(m_button_minirv32ima_start,0,wxALL,5);
    buttonSizer->Add(m_button_minirv32ima_stop,0,wxALL,5);
    buttonSizer->Add(m_button_minirv32ima_quit,0,wxALL,5);
    topSizer->Add(buttonSizer,0);
    SetSizer(topSizer);

    m_button_minirv32ima_start->Bind(wxEVT_BUTTON,&MiniRV32IMAV2GenericGui::OnButtonClick_MiniRV32IMA_Start,this);
    m_button_minirv32ima_stop->Bind(wxEVT_BUTTON,&MiniRV32IMAV2GenericGui::OnButtonClick_MiniRV32IMA_Stop,this);
    m_button_minirv32ima_quit->Bind(wxEVT_BUTTON,&MiniRV32IMAV2GenericGui::OnButtonClick_MiniRV32IMA_Quit,this);

    m_timer_minirv32ima_ms_timer.SetOwner(this);
    Bind(wxEVT_TIMER,&MiniRV32IMAV2GenericGui::OnTimer_MiniRV32IMA_MS_Timer,this);
    m_timer_minirv32ima_ms_timer.Start(10);
}

MiniRV32IMAV2GenericGui::~MiniRV32IMAV2GenericGui()
{
    StopVM();
}

void MiniRV32IMAV2GenericGui::AppendOutput(const void* data,size_t size)
{
    if(data==NULL || size==0)
    {
        return;
    }
    std::lock_guard<std::mutex> lock(m_io_mutex);
    m_output.append((const char *)data,size);
}

void MiniRV32IMAV2GenericGui::PushInput(const std::string& data)
{
    std::lock_guard<std::mutex> lock(m_io_mutex);
    m_input.append(data);
}

bool MiniRV32IMAV2GenericGui::HasInput()
{
    std::lock_guard<std::mutex> lock(m_io_mutex);
    return !m_input.empty();
}

uint8_t MiniRV32IMAV2GenericGui::GetInput()
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

void MiniRV32IMAV2GenericGui::DrainOutput()
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

void MiniRV32IMAV2GenericGui::SetRunningUI(bool running)
{
    m_button_minirv32ima_start->Enable(!running);
    m_button_minirv32ima_stop->Enable(running);
    m_panel_minirv32ima_settings->Enable(!running);
}

void MiniRV32IMAV2GenericGui::OnButtonClick_MiniRV32IMA_Start(wxCommandEvent& event)
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
    if(!LoadImageFile(error))
    {
        wxMessageBox(error,_T("启动失败"),wxOK|wxICON_ERROR,this);
        return;
    }

    m_running.store(true);
    SetRunningUI(true);
    m_listbook_minirv32ima->SetSelection(1);
    m_HCPPTerminal_Serialport0->SetFocus();

    m_thread=std::thread([this]()
    {
        RunVM();
        m_finished.store(true);
    });
}

void MiniRV32IMAV2GenericGui::OnButtonClick_MiniRV32IMA_Stop(wxCommandEvent& event)
{
    (void)event;
    m_stop_requested.store(true);
    m_button_minirv32ima_stop->Enable(false);
}

void MiniRV32IMAV2GenericGui::OnButtonClick_MiniRV32IMA_Quit(wxCommandEvent& event)
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

void MiniRV32IMAV2GenericGui::OnTimer_MiniRV32IMA_MS_Timer(wxTimerEvent& event)
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

void MiniRV32IMAV2GenericGui::StopVM()
{
    m_stop_requested.store(true);
    m_timer_minirv32ima_ms_timer.Stop();
    if(m_thread.joinable())
    {
        m_thread.join();
    }
    m_running.store(false);
}

bool MiniRV32IMAV2GenericGui::LoadImageFile(wxString& error)
{
    std::string image_name=m_filePicker_minirv32ima_image->GetPath().ToStdString();
    if(image_name.empty())
    {
        error=_T("请选择镜像文件!");
        return false;
    }

    std::ifstream image_file(image_name.c_str(),std::ios::in|std::ios::binary);
    if(!image_file.is_open())
    {
        error=_T("无法打开镜像文件:")+wxString::FromUTF8(image_name.c_str());
        return false;
    }
    image_file.seekg(0,std::ios::end);
    std::streamoff image_size=image_file.tellg();
    image_file.seekg(0,std::ios::beg);
    if(image_size<=0)
    {
        error=_T("镜像文件为空!");
        return false;
    }
    m_impl->m_image.resize((size_t)image_size);
    image_file.read((char *)m_impl->m_image.data(),(std::streamsize)m_impl->m_image.size());
    image_file.close();

    m_impl->m_dtb.clear();
    std::string dtb_name=m_filePicker_minirv32ima_dtb->GetPath().ToStdString();
    if(!dtb_name.empty())
    {
        std::ifstream dtb_file(dtb_name.c_str(),std::ios::in|std::ios::binary);
        if(!dtb_file.is_open())
        {
            error=_T("无法打开设备树文件:")+wxString::FromUTF8(dtb_name.c_str());
            return false;
        }
        dtb_file.seekg(0,std::ios::end);
        std::streamoff dtb_size=dtb_file.tellg();
        dtb_file.seekg(0,std::ios::beg);
        if(dtb_size<=0)
        {
            error=_T("设备树文件为空!");
            return false;
        }
        m_impl->m_dtb.resize((size_t)dtb_size);
        dtb_file.read((char *)m_impl->m_dtb.data(),(std::streamsize)m_impl->m_dtb.size());
        dtb_file.close();
    }

    m_impl->m_display_dtb=m_checkBox_minirv32ima_display_dtb->IsChecked();
    if(!m_impl->reset())
    {
        error=_T("mini-rv32ima初始化失败，镜像或设备树可能过大!");
        return false;
    }

    if(m_impl->m_display_dtb)
    {
        std::string dump;
        fdt_dump(m_impl->m_machine.dtb,dump);
        AppendOutput(dump.data(),dump.size());
    }

    return true;
}

void MiniRV32IMAV2GenericGui::RunVM()
{
    hminirv32ima_machine_default64mb_t& machine=m_impl->m_machine;
    uint64_t last=get_time_us();
    while(!m_stop_requested.load())
    {
        uint64_t now=get_time_us();
        int code=hminirv32ima_machine_default64mb_step(&machine,(uint32_t)(now-last),1000);
        last=now;
        switch(code)
        {
        case 0:
        {
            std::this_thread::yield();
        }
        break;
        case 1:
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        break;
        case 0x5555:
        {
            return;
        }
        case 0x7777:
        {
            if(!m_impl->reset())
            {
                return;
            }
            last=get_time_us();
        }
        break;
        default:
        {
        }
        break;
        }
    }
}
