#include "rvvmgenericgui.h"
#include "wx/log.h"
#include "thread"
#include <wx/stdpaths.h>

RVVMGenericGui::RVVMGenericGui(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):rvvmgenericbase(parent, id, pos,size,style, name),m_running_machine(NULL)
{

}

RVVMGenericGui::~RVVMGenericGui()
{

}

void RVVMGenericGui::OnButtonClick_RVVM_Generic_Start( wxCommandEvent& event )
{
    std::lock_guard<std::recursive_mutex> lock(m_vm_gui_lock);
    m_button_rvvm_generic_start->Enable(false);
    m_button_rvvm_generic_stop->Enable(true);
    m_running_machine=CreateMachine();
    if(m_running_machine!=NULL)
    {
        std::thread machine_thread([&]()
        {
            RunMachine(m_running_machine);
        });
        machine_thread.detach();
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
    if(m_running_machine!=NULL)
    {
        rvvm_pause_machine(m_running_machine);
    }
    std::lock_guard<std::recursive_mutex> lock(m_vm_gui_lock);
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
    long int mem_mb=m_slider_rvvm_generic_mem->GetValue();
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

        //标准输出作为串口终端输出
        /*
         * TODO:GUI程序会打开一个无用的console
         */
        ns16550a_init_term_auto(machine);

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

        /*
         * 初始化GUI
         */
        gui_window_init_auto(machine, 640, 480);
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

    rvvm_start_machine(machine);

    // Returns on machine shutdown
    rvvm_run_eventloop();

    rvvm_free_machine(machine);

    m_button_rvvm_generic_start->Enable(true);
    m_button_rvvm_generic_stop->Enable(false);

}
