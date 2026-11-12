#include "minirisvcgenericgui.h"

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

#include "hsimulator/risc-v/risc-v_machine_rv32.h"
#include "hsimulator/risc-v/risc-v_machine_rv64.h"

#define MINIRISCV_ROM_BASE      0xD0000000UL
#define MINIRISCV_RAM_BASE      0x20000000UL
#define MINIRISCV_REGION_SIZE   (2UL*1024*1024)

static uint64_t get_time_us()
{
    htimespec_t ts= {0};
    hclock_gettime(HCLOCK_MONOTONIC,&ts);
    return (uint64_t)ts.tv_sec*1000000ULL+(uint64_t)ts.tv_nsec/1000ULL;
}

template<typename M>
struct riscv_machine_ops;

template<>
struct riscv_machine_ops<hs_risc_v_machine_rv32_t>
{
    using machine_t=hs_risc_v_machine_rv32_t;
    using memory_t=hs_risc_v_machine_rv32_memory_t;
    static size_t size(void)
    {
        return hs_risc_v_machine_rv32_size();
    }
    static machine_t *init(void *mem)
    {
        return hs_risc_v_machine_rv32_init(mem);
    }
    static bool check(machine_t *machine)
    {
        return hs_risc_v_machine_rv32_check(machine);
    }
    static int step(machine_t *machine,uint32_t elapsed_us,size_t count)
    {
        return hs_risc_v_machine_rv32_step(machine,elapsed_us,count);
    }
    static void wfi_clear(machine_t *machine)
    {
        hs_risc_v_core_rv32_wfi_clear(hs_risc_v_machine_rv32_core(machine));
    }
};

template<>
struct riscv_machine_ops<hs_risc_v_machine_rv64_t>
{
    using machine_t=hs_risc_v_machine_rv64_t;
    using memory_t=hs_risc_v_machine_rv64_memory_t;
    static size_t size(void)
    {
        return hs_risc_v_machine_rv64_size();
    }
    static machine_t *init(void *mem)
    {
        return hs_risc_v_machine_rv64_init(mem);
    }
    static bool check(machine_t *machine)
    {
        return hs_risc_v_machine_rv64_check(machine);
    }
    static int step(machine_t *machine,uint32_t elapsed_us,size_t count)
    {
        return hs_risc_v_machine_rv64_step(machine,elapsed_us,count);
    }
    static void wfi_clear(machine_t *machine)
    {
        hs_risc_v_core_rv64_wfi_clear(hs_risc_v_machine_rv64_core(machine));
    }
};

static thread_local MiniRISCVImpl* t_active_impl=NULL;

struct MiniRISCVImpl
{
    MiniRISCVImpl(MiniRISCVGenericGui* owner)
        : m_owner(owner),
          m_rv64(false),
          m_machine(NULL),
          m_step(NULL),
          m_wfi_clear(NULL)
    {
    }

    template<typename MEM>
    static size_t rom_read(const MEM* mem,uint32_t address,void* data,size_t len)
    {
        (void)mem;
        MiniRISCVImpl* impl=t_active_impl;
        if(impl==NULL)
        {
            return 0;
        }
        size_t offset=0;
        if(address<0x10000000UL && address<impl->m_rom.size())
        {
            offset=address;
        }
        else if(address>=MINIRISCV_ROM_BASE && (uint64_t)(address-MINIRISCV_ROM_BASE)<impl->m_rom.size())
        {
            offset=(size_t)(address-MINIRISCV_ROM_BASE);
        }
        else
        {
            return 0;
        }
        if((uint64_t)offset+len>impl->m_rom.size())
        {
            return 0;
        }
        memcpy(data,impl->m_rom.data()+offset,len);
        return len;
    }

    template<typename MEM>
    static size_t rom_write(const MEM* mem,uint32_t address,const void* data,size_t len)
    {
        (void)mem;
        (void)address;
        (void)data;
        (void)len;
        return 0;
    }

    template<typename MEM>
    static size_t flash_read(const MEM* mem,uint32_t address,void* data,size_t len)
    {
        (void)mem;
        MiniRISCVImpl* impl=t_active_impl;
        if(impl==NULL)
        {
            return 0;
        }
        size_t offset=0;
        if(address<0x10000000UL && address<impl->m_flash.size())
        {
            offset=address;
        }
        else if(address>=0x10000000UL && (uint64_t)(address-0x10000000UL)<impl->m_flash.size())
        {
            offset=(size_t)(address-0x10000000UL);
        }
        else
        {
            return 0;
        }
        if((uint64_t)offset+len>impl->m_flash.size())
        {
            return 0;
        }
        memcpy(data,impl->m_flash.data()+offset,len);
        return len;
    }

    template<typename MEM>
    static size_t flash_write(const MEM* mem,uint32_t address,const void* data,size_t len)
    {
        (void)mem;
        (void)address;
        (void)data;
        (void)len;
        return 0;
    }

    template<typename MEM>
    static size_t ram_read(const MEM* mem,uint32_t address,void* data,size_t len)
    {
        (void)mem;
        MiniRISCVImpl* impl=t_active_impl;
        if(impl==NULL)
        {
            return 0;
        }
        if(address<MINIRISCV_RAM_BASE || (uint64_t)address+len>(uint64_t)MINIRISCV_RAM_BASE+impl->m_ram.size())
        {
            return 0;
        }
        memcpy(data,impl->m_ram.data()+(address-MINIRISCV_RAM_BASE),len);
        return len;
    }

    template<typename MEM>
    static size_t ram_write(const MEM* mem,uint32_t address,const void* data,size_t len)
    {
        (void)mem;
        MiniRISCVImpl* impl=t_active_impl;
        if(impl==NULL)
        {
            return 0;
        }
        if(address<MINIRISCV_RAM_BASE || (uint64_t)address+len>(uint64_t)MINIRISCV_RAM_BASE+impl->m_ram.size())
        {
            return 0;
        }
        memcpy(impl->m_ram.data()+(address-MINIRISCV_RAM_BASE),data,len);
        return len;
    }

    template<typename M>
    static bool console_put(M* machine,void* usr,uint8_t data)
    {
        (void)machine;
        (void)usr;
        MiniRISCVImpl* impl=t_active_impl;
        if(impl!=NULL && impl->m_owner!=NULL)
        {
            impl->m_owner->AppendOutput(&data,1);
        }
        return true;
    }

    template<typename M>
    static bool console_get(M* machine,void* usr,uint8_t* data)
    {
        (void)machine;
        (void)usr;
        MiniRISCVImpl* impl=t_active_impl;
        if(impl==NULL || impl->m_owner==NULL || data==NULL || !impl->m_owner->HasInput())
        {
            return false;
        }
        *data=impl->m_owner->GetInput();
        return true;
    }

    template<typename M>
    static bool console_has_data(M* machine,void* usr)
    {
        (void)machine;
        (void)usr;
        MiniRISCVImpl* impl=t_active_impl;
        if(impl==NULL || impl->m_owner==NULL)
        {
            return false;
        }
        return impl->m_owner->HasInput();
    }

    MiniRISCVGenericGui* m_owner;
    bool m_rv64;
    std::vector<uint8_t> m_machine_mem;
    std::vector<uint8_t> m_rom;
    std::vector<uint8_t> m_flash;
    std::vector<uint8_t> m_ram;
    void* m_machine;
    int (*m_step)(void*,uint32_t,size_t);
    void (*m_wfi_clear)(void*);
};

template<typename M>
static bool configure_machine(MiniRISCVImpl* impl)
{
    using ops=riscv_machine_ops<M>;
    impl->m_machine_mem.resize(ops::size());
    M* machine=ops::init(impl->m_machine_mem.data());
    if(machine==NULL)
    {
        return false;
    }

    machine->ram.base=MINIRISCV_RAM_BASE;
    machine->ram.size=(uint32_t)impl->m_ram.size();
    machine->ram.read=&MiniRISCVImpl::template ram_read<typename ops::memory_t>;
    machine->ram.write=&MiniRISCVImpl::template ram_write<typename ops::memory_t>;

    machine->rom.base=MINIRISCV_ROM_BASE;
    machine->rom.size=MINIRISCV_REGION_SIZE;
    machine->rom.read=&MiniRISCVImpl::template rom_read<typename ops::memory_t>;
    machine->rom.write=&MiniRISCVImpl::template rom_write<typename ops::memory_t>;

    if(!impl->m_flash.empty())
    {
        machine->flash.base=0x00000000UL;
        machine->flash.size=MINIRISCV_REGION_SIZE;
        machine->flash.read=&MiniRISCVImpl::template flash_read<typename ops::memory_t>;
        machine->flash.write=&MiniRISCVImpl::template flash_write<typename ops::memory_t>;
    }
    else
    {
        machine->flash.base=0;
        machine->flash.size=0;
        machine->flash.read=NULL;
        machine->flash.write=NULL;
    }

    machine->entry=0;
    machine->hart_id=0;
    machine->dtb_address=0;
    machine->console_put=&MiniRISCVImpl::template console_put<M>;
    machine->console_get=&MiniRISCVImpl::template console_get<M>;
    machine->console_has_data=&MiniRISCVImpl::template console_has_data<M>;

    if(!ops::check(machine))
    {
        return false;
    }

    impl->m_machine=machine;
    impl->m_step=[](void* machine,uint32_t elapsed_us,size_t count)->int
    {
        return ops::step((M*)machine,elapsed_us,count);
    };
    impl->m_wfi_clear=[](void* machine)->void
    {
        ops::wfi_clear((M*)machine);
    };
    return true;
}

static bool parse_size(const std::string& text,size_t* size)
{
    if(text.empty() || size==NULL)
    {
        return false;
    }
    char* end=NULL;
    unsigned long long value=strtoull(text.c_str(),&end,0);
    if(end==text.c_str())
    {
        return false;
    }
    while(*end==' ')
    {
        end++;
    }
    if(*end=='k' || *end=='K')
    {
        value*=1024ULL;
        end++;
    }
    else if(*end=='m' || *end=='M')
    {
        value*=1024ULL*1024ULL;
        end++;
    }
    else if(*end=='g' || *end=='G')
    {
        value*=1024ULL*1024ULL*1024ULL;
        end++;
    }
    if(*end!='\0')
    {
        return false;
    }
    *size=(size_t)value;
    return true;
}

static bool load_file(const std::string& path,std::vector<uint8_t>& data)
{
    std::ifstream file;
    file.open(path.c_str(),std::ios::in|std::ios::binary);
    if(!file.is_open())
    {
        return false;
    }
    file.seekg(0,std::ios::end);
    std::streamoff length=file.tellg();
    if(length<0)
    {
        return false;
    }
    file.seekg(0,std::ios::beg);
    data.resize((size_t)length);
    if(length>0)
    {
        file.read((char*)data.data(),length);
    }
    file.close();
    return true;
}

MiniRISCVGenericGui::MiniRISCVGenericGui(wxWindow* parent,wxWindowID id,
                                         const wxPoint& pos,const wxSize& size,
                                         long style,const wxString& name)
    : wxPanel(parent,id,pos,size,style,name),
      m_listbook(NULL),
      m_panel_settings(NULL),
      m_panel_serial(NULL),
      m_terminal(NULL),
      m_filePicker_rom(NULL),
      m_filePicker_flash(NULL),
      m_choice_arch(NULL),
      m_textCtrl_ram_size(NULL),
      m_button_start(NULL),
      m_button_stop(NULL),
      m_button_quit(NULL),
      m_running(false),
      m_stop_requested(false),
      m_finished(false),
      m_impl(new MiniRISCVImpl(this))
{
    wxBoxSizer* topSizer=new wxBoxSizer(wxVERTICAL);
    m_listbook=new wxListbook(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLB_LEFT);
    m_panel_settings=new wxPanel(m_listbook,wxID_ANY);
    m_panel_serial=new wxPanel(m_listbook,wxID_ANY);

    /*
     * 设置页
     */
    wxBoxSizer* settingsSizer=new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer* grid=new wxFlexGridSizer(0,2,8,8);
    grid->AddGrowableCol(1,1);

    grid->Add(new wxStaticText(m_panel_settings,wxID_ANY,_T("架构:")),0,wxALIGN_CENTER_VERTICAL);
    m_choice_arch=new wxChoice(m_panel_settings,wxID_ANY);
    m_choice_arch->Append(_T("rv32"));
    m_choice_arch->Append(_T("rv64"));
    m_choice_arch->SetSelection(0);
    grid->Add(m_choice_arch,0);

    grid->Add(new wxStaticText(m_panel_settings,wxID_ANY,_T("ROM文件:")),0,wxALIGN_CENTER_VERTICAL);
    m_filePicker_rom=new wxFilePickerCtrl(m_panel_settings,wxID_ANY,wxEmptyString,
            _T("选择ROM文件"),
            _T("镜像文件 (*.bin;*.rom)|*.bin;*.rom|所有文件 (*.*)|*.*"),
            wxDefaultPosition,wxDefaultSize,wxFLP_DEFAULT_STYLE);
    grid->Add(m_filePicker_rom,1,wxEXPAND);

    grid->Add(new wxStaticText(m_panel_settings,wxID_ANY,_T("Flash文件(可选):")),0,wxALIGN_CENTER_VERTICAL);
    m_filePicker_flash=new wxFilePickerCtrl(m_panel_settings,wxID_ANY,wxEmptyString,
            _T("选择Flash文件"),
            _T("镜像文件 (*.bin;*.rom)|*.bin;*.rom|所有文件 (*.*)|*.*"),
            wxDefaultPosition,wxDefaultSize,wxFLP_DEFAULT_STYLE);
    grid->Add(m_filePicker_flash,1,wxEXPAND);

    grid->Add(new wxStaticText(m_panel_settings,wxID_ANY,_T("RAM大小:")),0,wxALIGN_CENTER_VERTICAL);
    m_textCtrl_ram_size=new wxTextCtrl(m_panel_settings,wxID_ANY,_T("1M"));
    grid->Add(m_textCtrl_ram_size,1,wxEXPAND);

    settingsSizer->Add(grid,0,wxEXPAND|wxALL,10);
    m_panel_settings->SetSizer(settingsSizer);

    /*
     * 串口页
     */
    wxBoxSizer* serialSizer=new wxBoxSizer(wxVERTICAL);
    m_terminal=new HCPPTerminal(m_panel_serial,wxID_ANY,wxDefaultPosition);
    serialSizer->Add(m_terminal,1,wxEXPAND|wxALL,5);
    m_panel_serial->SetSizer(serialSizer);
    m_terminal->set_input_callback([this](wxString input)
    {
        PushInput(input.ToStdString());
    });

    m_listbook->AddPage(m_panel_settings,_T("设置"),true);
    m_listbook->AddPage(m_panel_serial,_T("串口"),false);
    topSizer->Add(m_listbook,1,wxEXPAND|wxALL,5);

    wxBoxSizer* buttonSizer=new wxBoxSizer(wxHORIZONTAL);
    m_button_start=new wxButton(this,wxID_ANY,_T("启动"));
    m_button_stop=new wxButton(this,wxID_ANY,_T("停止"));
    m_button_quit=new wxButton(this,wxID_ANY,_T("退出虚拟机"));
    m_button_stop->Enable(false);
    buttonSizer->Add(m_button_start,0,wxALL,5);
    buttonSizer->Add(m_button_stop,0,wxALL,5);
    buttonSizer->Add(m_button_quit,0,wxALL,5);
    topSizer->Add(buttonSizer,0);
    SetSizer(topSizer);

    m_button_start->Bind(wxEVT_BUTTON,&MiniRISCVGenericGui::OnButtonClick_Start,this);
    m_button_stop->Bind(wxEVT_BUTTON,&MiniRISCVGenericGui::OnButtonClick_Stop,this);
    m_button_quit->Bind(wxEVT_BUTTON,&MiniRISCVGenericGui::OnButtonClick_Quit,this);

    m_timer.SetOwner(this);
    Bind(wxEVT_TIMER,&MiniRISCVGenericGui::OnTimer,this);
    m_timer.Start(10);
}

MiniRISCVGenericGui::~MiniRISCVGenericGui()
{
    StopVM();
}

void MiniRISCVGenericGui::AppendOutput(const void* data,size_t size)
{
    if(data==NULL || size==0)
    {
        return;
    }
    std::lock_guard<std::mutex> lock(m_io_mutex);
    m_output.append((const char*)data,size);
}

void MiniRISCVGenericGui::PushInput(const std::string& data)
{
    std::lock_guard<std::mutex> lock(m_io_mutex);
    m_input.append(data);
}

bool MiniRISCVGenericGui::HasInput()
{
    std::lock_guard<std::mutex> lock(m_io_mutex);
    return !m_input.empty();
}

uint8_t MiniRISCVGenericGui::GetInput()
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

void MiniRISCVGenericGui::DrainOutput()
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
    if(m_terminal!=NULL && !data.empty())
    {
        m_terminal->DisplayCharsUnsafe(wxString::FromUTF8(data.data(),data.size()));
        m_terminal->Refresh(false);
    }
}

void MiniRISCVGenericGui::SetRunningUI(bool running)
{
    m_button_start->Enable(!running);
    m_button_stop->Enable(running);
    m_panel_settings->Enable(!running);
}

void MiniRISCVGenericGui::OnButtonClick_Start(wxCommandEvent& event)
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
    if(!LoadImages(error))
    {
        wxMessageBox(error,_T("启动失败"),wxOK|wxICON_ERROR,this);
        return;
    }

    m_running.store(true);
    SetRunningUI(true);
    m_listbook->SetSelection(1);
    m_terminal->SetFocus();

    m_thread=std::thread([this]()
    {
        RunVM();
        m_finished.store(true);
    });
}

void MiniRISCVGenericGui::OnButtonClick_Stop(wxCommandEvent& event)
{
    (void)event;
    m_stop_requested.store(true);
    m_button_stop->Enable(false);
}

void MiniRISCVGenericGui::OnButtonClick_Quit(wxCommandEvent& event)
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

void MiniRISCVGenericGui::OnTimer(wxTimerEvent& event)
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

void MiniRISCVGenericGui::StopVM()
{
    m_stop_requested.store(true);
    m_timer.Stop();
    if(m_thread.joinable())
    {
        m_thread.join();
    }
    m_running.store(false);
}

bool MiniRISCVGenericGui::LoadImages(wxString& error)
{
    std::string rom_name=m_filePicker_rom->GetPath().ToStdString();
    if(rom_name.empty())
    {
        error=_T("请选择ROM文件!");
        return false;
    }
    if(!load_file(rom_name,m_impl->m_rom))
    {
        error=_T("无法打开ROM文件:")+wxString::FromUTF8(rom_name.c_str());
        return false;
    }
    if(m_impl->m_rom.empty())
    {
        error=_T("ROM文件为空!");
        return false;
    }

    m_impl->m_flash.clear();
    std::string flash_name=m_filePicker_flash->GetPath().ToStdString();
    if(!flash_name.empty())
    {
        if(!load_file(flash_name,m_impl->m_flash))
        {
            error=_T("无法打开Flash文件:")+wxString::FromUTF8(flash_name.c_str());
            return false;
        }
        if(m_impl->m_flash.empty())
        {
            error=_T("Flash文件为空!");
            return false;
        }
    }

    size_t ram_size=0;
    if(!parse_size(m_textCtrl_ram_size->GetValue().ToStdString(),&ram_size))
    {
        error=_T("RAM大小格式错误!");
        return false;
    }
    if(ram_size<16UL*1024)
    {
        error=_T("RAM大小不能小于16KB!");
        return false;
    }
    if(ram_size>(size_t)256UL*1024*1024)
    {
        error=_T("RAM大小不能大于256MB!");
        return false;
    }
    m_impl->m_ram.assign(ram_size,0);

    bool rv64=(m_choice_arch->GetSelection()==1);
    m_impl->m_rv64=rv64;
    if(rv64)
    {
        if(!configure_machine<hs_risc_v_machine_rv64_t>(m_impl.get()))
        {
            error=_T("rv64机器初始化失败!");
            return false;
        }
    }
    else
    {
        if(!configure_machine<hs_risc_v_machine_rv32_t>(m_impl.get()))
        {
            error=_T("rv32机器初始化失败!");
            return false;
        }
    }

    return true;
}

void MiniRISCVGenericGui::RunVM()
{
    t_active_impl=m_impl.get();
    uint64_t last=get_time_us();
    while(!m_stop_requested.load())
    {
        uint64_t now=get_time_us();
        uint64_t elapsed=now-last;
        last=now;
        if(elapsed>UINT32_MAX)
        {
            elapsed=UINT32_MAX;
        }
        if(elapsed==0)
        {
            elapsed=1;
        }
        int code=0;
        if(m_impl->m_step!=NULL)
        {
            code=m_impl->m_step(m_impl->m_machine,(uint32_t)elapsed,10000);
        }
        if(code==1)
        {
            if(m_impl->m_wfi_clear!=NULL)
            {
                m_impl->m_wfi_clear(m_impl->m_machine);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        else if(code<0)
        {
            break;
        }
        else
        {
            std::this_thread::yield();
        }
    }
    t_active_impl=NULL;
}
