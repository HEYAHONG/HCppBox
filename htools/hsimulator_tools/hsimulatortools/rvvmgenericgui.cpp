#include "rvvmgenericgui.h"
#include "wx/log.h"
#include "thread"
#include <wx/stdpaths.h>
#include <wx/fontutil.h>

extern "C"
{
    typedef struct gui_window_t gui_window_t;
    extern bool gui_window_init_auto(rvvm_machine_t* machine, uint32_t width, uint32_t height);
    extern i2c_bus_t* i2c_oc_init_auto(rvvm_machine_t* machine);
    extern rvvm_mmio_dev_t* ns16550a_init_auto(rvvm_machine_t* machine, struct chardev* chardev);
    extern rvvm_mmio_dev_t* ns16550a_init_term_auto(rvvm_machine_t* machine);
    extern bool ata_init_auto(rvvm_machine_t* machine, const char* image, bool rw);
    extern void riscv_clint_init_auto(rvvm_machine_t* machine);
    extern void riscv_imsic_init_auto(rvvm_machine_t* machine);
    extern rvvm_intc_t* riscv_aplic_init_auto(rvvm_machine_t* machine);
    extern rvvm_intc_t* riscv_plic_init_auto(rvvm_machine_t* machine);
    extern pci_bus_t* pci_bus_init_auto(rvvm_machine_t* machine);
    extern rvvm_mmio_dev_t* rtc_goldfish_init_auto(rvvm_machine_t* machine);
    extern rvvm_mmio_dev_t* syscon_init_auto(rvvm_machine_t* machine);
    typedef struct pci_dev pci_dev_t;
    extern pci_dev_t* nvme_init_auto(rvvm_machine_t* machine, const char* image_path, bool rw);
}



RVVMGenericGui::RVVMGenericGui(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):rvvmgenericbase(parent, id, pos,size,style, name),m_running_machine(NULL)
{
    {
        /*
         * 初始化串口0输出
         */
        wxFont font=m_richText_rvvm_generic_serialport0->GetFont();
        m_richText_rvvm_generic_serialport0->SetFont(*wxTheFontList->FindOrCreateFont(font.GetNativeFontInfo()->GetPointSize(),wxFONTFAMILY_TELETYPE,font.GetStyle(),font.GetWeight()));
        m_richText_rvvm_generic_serialport0->SetBackgroundColour(*wxBLACK);
        m_richText_rvvm_generic_serialport0->SetForegroundColour(*wxWHITE);
    }
}

RVVMGenericGui::~RVVMGenericGui()
{
    StopMachine(m_running_machine);
    m_running_machine=NULL;
}

void RVVMGenericGui::OnChar_RVVM_Generic_Serialport0( wxKeyEvent& event )
{
    wxChar uc=event.GetUnicodeKey();
    if(uc!=wxKEY_NONE)
    {
        m_machine_serialport[0].Input.Post((uint8_t)uc);
        m_machine_serialport[0].InputDataCount++;
    }
}

void RVVMGenericGui::OnKeyDown_RVVM_Generic_Serialport0( wxKeyEvent& event )
{
    {
        switch ( event.GetKeyCode() )
        {
        case WXK_LEFT:
        {

        }
        break;
        case WXK_RIGHT:
        {

        }
        break;
        case WXK_UP:
        {

        }
        break;
        case WXK_DOWN:
        {

        }
        break;
        default:
        {
            event.Skip();
        }
        break;
        }
    }
}

void RVVMGenericGui::OnButtonClick_RVVM_Generic_Start( wxCommandEvent& event )
{
    std::lock_guard<std::recursive_mutex> lock(m_vm_gui_lock);
    m_button_rvvm_generic_start->Enable(false);
    m_button_rvvm_generic_stop->Enable(true);
    InitMachineSerialport();
    m_running_machine=CreateMachine();
    if(m_running_machine!=NULL)
    {
        std::thread machine_thread([&]()
        {
            RunMachine(m_running_machine);
            m_running_machine=NULL;
        });
        machine_thread.detach();
        {
            //自动切换至串口页面
            {
                m_listbook_rvvm_generic->SetSelection(1);
                m_panel_rvvm_generic_serialport0->SetFocus();
            }
        }
    }
    else
    {
        wxLogMessage(_T("启动RVVM虚拟机失败!"));
        m_button_rvvm_generic_start->Enable(true);
        m_button_rvvm_generic_stop->Enable(false);
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
    rvvm_machine_t *machine=rvvm_create_machine(mem_mb*1024*1024,smp,isa);
    if(machine!=NULL)
    {
        //初始化基本外设
        riscv_clint_init_auto(machine);

        if(m_checkBox_rvvm_generic_disk_ata->IsChecked())
        {
            // Use RISC-V Advanced Interrupt Architecture (IMSIC + APLIC)
            riscv_imsic_init_auto(machine);
            riscv_aplic_init_auto(machine);
        }
        else
        {
            // Use SiFive PLIC
            riscv_plic_init_auto(machine);
        }

        pci_bus_init_auto(machine);
        i2c_oc_init_auto(machine);
        // usb_xhci_init(rvvm_get_pci_bus(machine));
        rtc_goldfish_init_auto(machine);
        syscon_init_auto(machine);

        for(size_t i=0; i<sizeof(m_machine_serialport)/sizeof(m_machine_serialport[0]); i++)
        {
            ns16550a_init_auto(machine,&m_machine_serialport[i].dev);
        }

        /*
         * 添加磁盘
         */
        if(!m_filePicker_rvvm_generic_disk_image->GetPath().IsEmpty())
        {
            std::string disk_image_path=m_filePicker_rvvm_generic_disk_image->GetPath().ToStdString();
            if(m_checkBox_rvvm_generic_disk_ata->IsChecked())
            {
                ata_init_auto(machine,disk_image_path.c_str(),true);
            }
            else
            {
                nvme_init_auto(machine,disk_image_path.c_str(),true);
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
        gui_window_init_auto(machine, 640, 480);
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
    m_button_rvvm_generic_start->Enable(false);
    m_button_rvvm_generic_stop->Enable(true);
    m_panel_rvvm_generic_settings->Enable(false);

    rvvm_start_machine(machine);

    // Returns on machine shutdown
    rvvm_run_eventloop();

    rvvm_free_machine(machine);

    m_button_rvvm_generic_start->Enable(true);
    m_button_rvvm_generic_stop->Enable(false);
    m_panel_rvvm_generic_settings->Enable(true);

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

static const uint32_t serialport_rx=0x01;
static const uint32_t serialport_tx=0x02;
void RVVMGenericGui::InitMachineSerialport()
{
    for(size_t i=0; i<sizeof(m_machine_serialport)/sizeof(m_machine_serialport[0]); i++)
    {
        {
            uint8_t msg=0;
            while(m_machine_serialport[i].Input.ReceiveTimeout(0,msg)==wxMSGQUEUE_NO_ERROR);
        }
        m_machine_serialport[i].InputDataCount=0;
        memset(&m_machine_serialport[i].dev,0,sizeof(m_machine_serialport[i].dev));
        m_machine_serialport[i].index=i;
        m_machine_serialport[i].parent=this;
        m_machine_serialport[i].dev.write=[](struct chardev* dev, const void* buf, size_t nbytes)->size_t
        {
            struct rvvm_serialport_t *serialport=(struct rvvm_serialport_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(dev,struct rvvm_serialport_t,dev);
            if(serialport!=NULL && serialport->parent!=NULL)
            {
                switch(serialport->index)
                {
                case 0:
                {
                    serialport->Output.Post(wxString::FromUTF8((const char *)buf,nbytes));
                }
                break;
                default:
                {

                }
                break;
                }
            }
            serialport->Flags=dev->poll(dev);
            return nbytes;
        };
        m_machine_serialport[i].dev.poll=[](struct chardev* dev)->uint32_t
        {
            struct rvvm_serialport_t *serialport=(struct rvvm_serialport_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(dev,struct rvvm_serialport_t,dev);
            if(serialport!=NULL && serialport->parent!=NULL)
            {
                return serialport_tx | (serialport->InputDataCount > 0 ? serialport_rx:0);
            }
            return serialport_tx;
        };
        m_machine_serialport[i].dev.read=[](struct chardev* dev, void* buf, size_t nbytes)->size_t
        {
            struct rvvm_serialport_t *serialport=(struct rvvm_serialport_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(dev,struct rvvm_serialport_t,dev);
            if(serialport==NULL && serialport->parent==NULL)
            {
                return 0;
            }
            size_t ret=0;
            while((nbytes--) > 0)
            {
                uint8_t msg=0;
                if(wxMSGQUEUE_NO_ERROR==serialport->Input.ReceiveTimeout(0,msg))
                {
                    ((uint8_t *)buf)[ret]=msg;
                    ret++;
                    serialport->InputDataCount--;
                }
            }
            serialport->Flags=dev->poll(dev);
            return ret;
        };
        m_machine_serialport[i].dev.update=[](struct chardev* dev)
        {
            struct rvvm_serialport_t *serialport=(struct rvvm_serialport_t *)GET_STRUCT_PTR_BY_MEMBER_PTR(dev,struct rvvm_serialport_t,dev);
            if(serialport==NULL && serialport->parent==NULL)
            {
                return;
            }
            if(dev->poll!=NULL && dev->notify!=NULL)
            {
                uint32_t flags=dev->poll(dev);
                uint32_t orig_flags=serialport->Flags;
                serialport->Flags=flags;
                flags&=(~orig_flags);
                if(flags)
                {
                    dev->notify(dev->io_dev,flags);
                }
            }
        };
        m_machine_serialport[i].Flags=m_machine_serialport[i].dev.poll(&m_machine_serialport[i].dev);
    }

}

void RVVMGenericGui::MachineSerialportLoop()
{
    for(size_t i=0; i<sizeof(m_machine_serialport)/sizeof(m_machine_serialport[0]); i++)
    {
        wxString data;
        wxString msg;
        while(wxMSGQUEUE_NO_ERROR==m_machine_serialport[i].Output.ReceiveTimeout(0,msg))
        {
            data+=msg;
        }
        if(!data.IsEmpty())
        {
            switch(i)
            {
            case 0:
            {
                wxString remain_data=data;
                /*
                 * 替换\r\n
                 */
                remain_data.Replace(_T("\r\n"),_T("\n"));
                while(!remain_data.IsEmpty())
                {
                    {
                        data.Clear();
                        int pos=wxNOT_FOUND;
                        if((pos=remain_data.Find('\b'))!=wxNOT_FOUND)
                        {
                            //处理退格
                            wxRichTextRange current_pos(m_richText_rvvm_generic_serialport0->GetLastPosition()-1,m_richText_rvvm_generic_serialport0->GetLastPosition());
                            m_richText_rvvm_generic_serialport0->Delete(current_pos);
                            if(pos==0)
                            {
                                remain_data=remain_data.substr(pos+1);
                                data.Clear();
                                continue;
                            }
                            else
                            {
                                data=remain_data.substr(0,pos);
                                remain_data=remain_data.substr(pos+1);
                            }
                        }

                        if(data.IsEmpty())
                        {
                            data=remain_data;
                            remain_data.Clear();
                        }
                    }
                    m_richText_rvvm_generic_serialport0->SetInsertionPointEnd();
                    m_richText_rvvm_generic_serialport0->BeginSuppressUndo();
                    m_richText_rvvm_generic_serialport0->BeginTextColour(m_richText_rvvm_generic_serialport0->GetForegroundColour());
                    m_richText_rvvm_generic_serialport0->WriteText(data);
                    m_richText_rvvm_generic_serialport0->EndTextColour();
                    m_richText_rvvm_generic_serialport0->EndSuppressUndo();
                    {
                        long pos=m_richText_rvvm_generic_serialport0->GetLastPosition();
                        if(pos > 0 && !m_richText_rvvm_generic_serialport0->IsPositionVisible(pos))
                        {
                            m_richText_rvvm_generic_serialport0->ShowPosition(pos);
                        }
                    }
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
}
