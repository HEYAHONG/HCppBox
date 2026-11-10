#include "rvvmgenericgui.h"
#include <rvvm/rvvm_board.h>
#include "wx/log.h"
#include "thread"
#include <wx/stdpaths.h>
#include <wx/fontutil.h>
#include <algorithm>
#include <cstring>

extern "C"
{
    /*
     * 串口设备初始化接口未在公开头文件中声明，此处按新的RVVM内部接口声明。
     * chardev结构定义见rvvmgenericgui.h（与RVVM内部chardev_t布局一致）。
     */
    extern rvvm_reg_dev_t* rvvm_ns16550a_init(rvvm_machine_t* machine, struct chardev* chardev,
                                              rvvm_addr_t addr, uint32_t attr,
                                              rvvm_irq_dev_t* irq_dev, rvvm_irq_t irq);
    /*
     * RVVM当前源码中I2C控制器初始化仍是旧接口名i2c_oc_init_auto()
     * (公开头文件中的rvvm_i2c_ocores_init()尚未实现)，故继续使用旧接口。
     */
    typedef struct rvvm_i2c_bus i2c_bus_t;
    extern i2c_bus_t* i2c_oc_init_auto(rvvm_machine_t* machine);
}



static const uint32_t serialport_rx=0x01;
static const uint32_t serialport_tx=0x02;

RVVMGenericGui::RVVMGenericGui(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):rvvmgenericbase(parent, id, pos,size,style, name),m_running_machine(NULL),m_machine_finished(false)
{
    m_HCPPTerminal_Serialport0->set_input_callback([this](wxString input)
    {
        std::string input_array=input.ToStdString();
        if(input_array.empty())
        {
            return;
        }
        struct rvvm_serialport_t &serialport=m_machine_serialport[0];
        std::lock_guard<std::mutex> lock(serialport.buffer_mutex);
        serialport.Input.append(input_array);
        serialport.InputDataCount.store(serialport.Input.size());
        /*
         * 立即通知UART有新数据，而不是等待RVVM的周期轮询(约60Hz)，
         * 从而消除串口输入最大约16ms的额外延迟。
         */
        if(serialport.NotifyReady.load() && serialport.dev.notify!=NULL && serialport.dev.io_dev!=NULL)
        {
            uint32_t flags=serialport_tx | (serialport.InputDataCount.load()>0?serialport_rx:0);
            uint32_t new_flags=flags & (~serialport.Flags.load());
            serialport.Flags.store(flags);
            if(new_flags)
            {
                serialport.dev.notify(serialport.dev.io_dev,new_flags);
            }
        }
    });
}

RVVMGenericGui::~RVVMGenericGui()
{
    StopMachine(m_running_machine);
    m_running_machine=NULL;
}

void RVVMGenericGui::OnChoice_RVVM_Generic_Isa( wxCommandEvent& event )
{
    if(m_choice_rvvm_generic_isa->GetString(m_choice_rvvm_generic_isa->GetSelection())==wxString(_T("rv32")))
    {
        //32位系统选择内存为512M
        m_choice_rvvm_generic_mem->SetSelection(1);
    }
    else
    {
        //64位系统选择内存为2G
        m_choice_rvvm_generic_mem->SetSelection(3);
    }
}

void RVVMGenericGui::OnButtonClick_RVVM_Generic_Start( wxCommandEvent& event )
{
    std::lock_guard<std::recursive_mutex> lock(m_vm_gui_lock);
    m_button_rvvm_generic_start->Enable(false);
    m_button_rvvm_generic_stop->Enable(true);
    m_panel_rvvm_generic_settings->Enable(false);
    m_machine_finished.store(false);
    InitMachineSerialport();
    m_running_machine=CreateMachine();
    if(m_running_machine!=NULL)
    {
        //注意：机器线程中不能再直接操作wxWidgets控件
        std::thread machine_thread([this,machine=m_running_machine]()
        {
            RunMachine(machine);
        });
        machine_thread.detach();
        //自动切换至串口页面并将焦点交给终端，避免用户直接在设置页输入而无响应
        {
            m_listbook_rvvm_generic->SetSelection(1);
            m_panel_rvvm_generic_serialport0->SetFocus();
            m_HCPPTerminal_Serialport0->SetFocus();
        }
#ifdef __WXGTK__
        {
            wxLogMessage(_T("请转到串口页查看输出或者进行输入!"));
        }
#endif
    }
    else
    {
        wxLogMessage(_T("启动RVVM虚拟机失败!"));
        m_machine_finished.store(false);
        m_button_rvvm_generic_start->Enable(true);
        m_button_rvvm_generic_stop->Enable(false);
        m_panel_rvvm_generic_settings->Enable(true);
    }
}
void RVVMGenericGui::OnButtonClick_RVVM_Generic_Stop( wxCommandEvent& event )
{
    StopMachine(m_running_machine);
    std::lock_guard<std::recursive_mutex> lock(m_vm_gui_lock);
    m_running_machine=NULL;
    m_button_rvvm_generic_start->Enable(true);
    m_button_rvvm_generic_stop->Enable(false);
}
void RVVMGenericGui::OnButtonClick_RVVM_Generic_Quit( wxCommandEvent& event )
{
    OnButtonClick_RVVM_Generic_Stop(event);
    std::lock_guard<std::recursive_mutex> lock(m_vm_gui_lock);
    if(GetParent()!=NULL && (dynamic_cast<wxAuiNotebook*>(GetParent()))!=nullptr)
    {
        int index=((wxAuiNotebook*)GetParent())->GetPageIndex(this);
        if(index!=wxNOT_FOUND)
        {
            ((wxAuiNotebook*)GetParent())->DeletePage(index);
        }
    }
}

void RVVMGenericGui::OnTimer_RVVM_MS_Timer( wxTimerEvent& event )
{
    if(m_machine_finished.exchange(false))
    {
        std::lock_guard<std::recursive_mutex> lock(m_vm_gui_lock);
        m_running_machine=NULL;
        m_button_rvvm_generic_start->Enable(true);
        m_button_rvvm_generic_stop->Enable(false);
        m_panel_rvvm_generic_settings->Enable(true);
    }
    MachineSerialportLoop();
}

void RVVMGenericGui::LoadDefaultMachineSettings()
{

}

rvvm_machine_t *RVVMGenericGui::CreateMachine(const char *isa)
{
    LoadDefaultMachineSettings();
    unsigned long int smp=1;
    m_choice_rvvm_generic_smp->GetString(m_choice_rvvm_generic_smp->GetSelection()).ToULong(&smp);
    if(smp < 1)
    {
        smp=1;
    }
    std::string isa_string;
    if(isa!=NULL)
    {
        isa_string=isa;
    }
    else
    {
        isa_string=m_choice_rvvm_generic_isa->GetString(m_choice_rvvm_generic_isa->GetSelection()).ToStdString();
    }
    unsigned long int mem_mb=256;
    m_choice_rvvm_generic_mem->GetString(m_choice_rvvm_generic_mem->GetSelection()).ToULong(&mem_mb);
    if(mem_mb < 256)
    {
        mem_mb=256;
    }
    std::string firmware=m_filePicker_rvvm_firmware_image->GetPath().ToStdString();
    if(firmware.empty())
    {
        wxLogMessage(_T("RVVM虚拟机必须指定固件（BIOS）!"));
        return NULL;
    }
    rvvm_machine_t *machine=rvvm_create_machine(mem_mb*1024*1024,smp,isa_string.c_str());
    if(machine!=NULL)
    {
        /*
         * 初始化基本外设(CLINT、PLIC/AIA、PCIe、Syscon、RTC)
         * 磁盘为ATA时使用RISC-V AIA(IMSIC+APLIC)，否则使用SiFive PLIC
         */
        bool use_aia=m_checkBox_rvvm_generic_disk_ata->IsChecked();
        if(!rvvm_board_riscv_virt_init(machine,use_aia))
        {
            wxLogMessage(_T("初始化RVVM基本外设失败!"));
            rvvm_free_machine(machine);
            return NULL;
        }

        i2c_oc_init_auto(machine);
        // usb_xhci_init(rvvm_get_pci_bus(machine));

        for(size_t i=0; i<sizeof(m_machine_serialport)/sizeof(m_machine_serialport[0]); i++)
        {
            if(rvvm_ns16550a_init(machine,&m_machine_serialport[i].dev,0x10000000UL,0,NULL,0)!=NULL)
            {
                std::lock_guard<std::mutex> lock(m_machine_serialport[i].buffer_mutex);
                m_machine_serialport[i].NotifyReady.store(true);
            }
        }

        /*
         * 添加磁盘
         */
        if(!m_filePicker_rvvm_generic_disk_image->GetPath().IsEmpty())
        {
            std::string disk_image_path=m_filePicker_rvvm_generic_disk_image->GetPath().ToStdString();
            if(m_checkBox_rvvm_generic_disk_ata->IsChecked())
            {
                rvvm_ata_init_auto(machine,disk_image_path.c_str());
            }
            else
            {
                rvvm_nvme_init_auto(machine,disk_image_path.c_str());
            }
        }

        /*
         * 加载固件
         */
        rvvm_load_firmware(machine,firmware.c_str());


        {
            /*
             * 加载(Linux)内核
             */
            std::string kernel_path=m_filePicker_rvvm_generic_kernel->GetPath().ToStdString();
            if(kernel_path.length()>0)
            {
                rvvm_load_kernel(machine,kernel_path.c_str());
                std::string kernel_cmdline=m_textCtrl_rvvm_generic_kernel_cmdline->GetValue().ToStdString();
                if(kernel_cmdline.length()>0)
                {
                    rvvm_set_cmdline(machine,kernel_cmdline.c_str());
                }
                if(m_checkBox_rvvm_generic_kernel_cmdline_default->IsChecked())
                {
                    if(m_checkBox_rvvm_generic_disk_ata->IsChecked())
                    {
                        /*
                         * 直接采用磁盘作为根文件系统(磁盘镜像不采用分区)
                         */
                        rvvm_append_cmdline(machine,"root=/dev/sda rw");
                    }
                    else
                    {
                        /*
                         * 直接采用磁盘作为根文件系统（磁盘镜像不采用分区）
                         */
                        rvvm_append_cmdline(machine,"root=/dev/nvme0n1 rootflags=discard rw");
                    }
                }
            }
        }

        /*
         * 初始化GUI,默认在GTK下使用(Windows下会产生冲突)
         */
#ifdef __WXGTK__
        //gui_window_init_auto(machine, 640, 480);
#endif

    }
    return machine;
}

void RVVMGenericGui::RunMachine(rvvm_machine_t *machine)
{
    if(machine==NULL)
    {
        return;
    }
    std::lock_guard<std::recursive_mutex> lock(m_vm_gui_lock);

    rvvm_start_machine(machine);

    // Returns on machine shutdown
    rvvm_run_eventloop();

    /*
     * 释放虚拟机前先断开串口通知，避免用户在停止过程中输入时
     * 访问已经释放的UART上下文。
     */
    for(size_t i=0; i<sizeof(m_machine_serialport)/sizeof(m_machine_serialport[0]); i++)
    {
        std::lock_guard<std::mutex> serialport_lock(m_machine_serialport[i].buffer_mutex);
        m_machine_serialport[i].NotifyReady.store(false);
        m_machine_serialport[i].dev.notify=NULL;
        m_machine_serialport[i].dev.io_dev=NULL;
        m_machine_serialport[i].Flags.store(0);
    }

    rvvm_free_machine(machine);

    /*
     * wxWidgets控件只能在GUI线程更新，这里只置位，
     * 由OnTimer_RVVM_MS_Timer在GUI线程恢复界面状态。
     */
    m_machine_finished.store(true);
}

void RVVMGenericGui::StopMachine(rvvm_machine_t *machine)
{
    if(machine==NULL)
    {
        return;
    }
    {
        rvvm_reset_machine(machine,false);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::recursive_mutex> lock(m_vm_gui_lock);
}

void RVVMGenericGui::InitMachineSerialport()
{
    for(size_t i=0; i<sizeof(m_machine_serialport)/sizeof(m_machine_serialport[0]); i++)
    {
        struct rvvm_serialport_t &serialport=m_machine_serialport[i];
        std::lock_guard<std::mutex> lock(serialport.buffer_mutex);
        serialport.Input.clear();
        serialport.Output.clear();
        serialport.InputDataCount.store(0);
        serialport.NotifyReady.store(false);
        memset(&serialport.dev,0,sizeof(serialport.dev));
        serialport.index=i;
        serialport.parent=this;
        serialport.dev.write=[](struct chardev* dev, const void* buf, size_t nbytes)->size_t
        {
            struct rvvm_serialport_t *serialport=(struct rvvm_serialport_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(dev,struct rvvm_serialport_t,dev);
            if(serialport==NULL || serialport->parent==NULL)
            {
                return nbytes;
            }
            /*
             * 只追加到输出缓冲，由GUI定时器批量取走并刷新，
             * 避免逐字节构造wxString及Post消息队列。
             */
            std::lock_guard<std::mutex> lock(serialport->buffer_mutex);
            serialport->Output.append((const char *)buf,nbytes);
            return nbytes;
        };
        serialport.dev.poll=[](struct chardev* dev)->uint32_t
        {
            struct rvvm_serialport_t *serialport=(struct rvvm_serialport_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(dev,struct rvvm_serialport_t,dev);
            if(serialport!=NULL && serialport->parent!=NULL)
            {
                return serialport_tx | (serialport->InputDataCount.load() > 0 ? serialport_rx:0);
            }
            return serialport_tx;
        };
        serialport.dev.read=[](struct chardev* dev, void* buf, size_t nbytes)->size_t
        {
            struct rvvm_serialport_t *serialport=(struct rvvm_serialport_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(dev,struct rvvm_serialport_t,dev);
            if(serialport==NULL || serialport->parent==NULL)
            {
                return 0;
            }
            std::lock_guard<std::mutex> lock(serialport->buffer_mutex);
            if(nbytes==0 || serialport->Input.empty())
            {
                return 0;
            }
            size_t ret=std::min(nbytes,serialport->Input.size());
            memcpy(buf,serialport->Input.data(),ret);
            serialport->Input.erase(0,ret);
            serialport->InputDataCount.store(serialport->Input.size());
            serialport->Flags.store(serialport_tx | (serialport->Input.empty()?0:serialport_rx));
            return ret;
        };
        serialport.dev.update=[](struct chardev* dev)
        {
            struct rvvm_serialport_t *serialport=(struct rvvm_serialport_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(dev,struct rvvm_serialport_t,dev);
            if(serialport==NULL || serialport->parent==NULL)
            {
                return;
            }
            std::lock_guard<std::mutex> lock(serialport->buffer_mutex);
            uint32_t flags=serialport_tx | (serialport->InputDataCount.load()>0?serialport_rx:0);
            uint32_t orig_flags=serialport->Flags.load();
            serialport->Flags.store(flags);
            flags&=(~orig_flags);
            if(flags && serialport->NotifyReady.load() && dev->notify!=NULL && dev->io_dev!=NULL)
            {
                dev->notify(dev->io_dev,flags);
            }
        };
        serialport.Flags.store(serialport_tx);
    }

}

void RVVMGenericGui::MachineSerialportLoop()
{
    for(size_t i=0; i<sizeof(m_machine_serialport)/sizeof(m_machine_serialport[0]); i++)
    {
        struct rvvm_serialport_t &serialport=m_machine_serialport[i];
        std::string data;
        {
            std::lock_guard<std::mutex> lock(serialport.buffer_mutex);
            if(serialport.Output.empty())
            {
                continue;
            }
            /*
             * 限制单次刷新的数据量，避免大量串口输出长时间占用GUI线程，
             * 从而导致键盘输入事件得不到及时处理。
             */
            const size_t max_chunk=64*1024;
            if(serialport.Output.size()>max_chunk)
            {
                data.assign(serialport.Output,0,max_chunk);
                serialport.Output.erase(0,max_chunk);
            }
            else
            {
                data.swap(serialport.Output);
            }
        }
        if(!data.empty())
        {
            switch(i)
            {
            case 0:
            {
                /*
                 * MachineSerialportLoop()运行在GUI线程，直接同步更新终端字符缓冲，
                 * 然后手动请求重绘。不能只调用DisplayChars()，它只是投递事件，
                 * 在页面不切换时不会产生Paint事件，内容不会实时显示。
                 */
                wxString text=wxString::FromUTF8(data.data(),data.size());
                m_HCPPTerminal_Serialport0->DisplayCharsUnsafe(text);
                m_HCPPTerminal_Serialport0->Refresh(false);
            }
            break;
            default:
            {

            }
            break;
            }
        }
    }
}
