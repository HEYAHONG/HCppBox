#ifndef __RVVMGENERICGUI_H__
#define __RVVMGENERICGUI_H__

#include "guibase/hsimulatortools_mainbase.h"
#include "HCPPGui.h"
#include "mutex"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include "rvvmlib.h"
#include "rvvm/rvvm.h"
#include "rvvm/gdbstub.h"
#include "rvvm/rvvm_isolation.h"
#include "rvvm/rvvm_user.h"
#include "rvvm/rvvmlib.h"
#include "rvvm/utils.h"

#include "rvvm/devices/ata.h"
#include "rvvm/devices/gui_window.h"
#include "rvvm/devices/i2c-oc.h"
#include "rvvm/devices/ns16550a.h"
#include "rvvm/devices/nvme.h"
#include "rvvm/devices/pci-bus.h"
#include "rvvm/devices/pci-vfio.h"
#include "rvvm/devices/riscv-aclint.h"
#include "rvvm/devices/riscv-aplic.h"
#include "rvvm/devices/riscv-imsic.h"
#include "rvvm/devices/riscv-plic.h"
#include "rvvm/devices/rtc-goldfish.h"
#include "rvvm/devices/rtl8169.h"
#include "rvvm/devices/sound-hda.h"
#include "rvvm/devices/syscon.h"
#include "rvvm/devices/usb-xhci.h"

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus
class RVVMGenericGui:public rvvmgenericbase
{
    std::recursive_mutex m_vm_gui_lock;
public:
    RVVMGenericGui(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 750,500 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
    virtual ~RVVMGenericGui();

protected:
    virtual void OnButtonClick_RVVM_Generic_Start( wxCommandEvent& event );
    virtual void OnButtonClick_RVVM_Generic_Stop( wxCommandEvent& event );
    virtual void OnButtonClick_RVVM_Generic_Quit( wxCommandEvent& event );

private:
    virtual void LoadDefaultMachineSettings();
    virtual rvvm_machine_t *CreateMachine(const char *isa=NULL);
    virtual void RunMachine(rvvm_machine_t *machine);
    rvvm_machine_t * m_running_machine;

};
#endif // __cplusplus

#endif // __RVVMGENERICGUI_H__
