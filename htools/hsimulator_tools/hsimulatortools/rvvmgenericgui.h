#ifndef __RVVMGENERICGUI_H__
#define __RVVMGENERICGUI_H__

#include "guibase/hsimulatortools_mainbase.h"
#include "HCPPGui.h"
#include "mutex"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
#include <rvvm/rvvm.h>

typedef struct chardev  rvvm_device_chardev_t;
#ifndef chardev_t
struct chardev
{
    // IO Dev -> Chardev calls
    uint32_t (*poll)(struct chardev* dev);
    size_t (*read)(struct chardev* dev, void* buf, size_t nbytes);
    size_t (*write)(struct chardev* dev, const void* buf, size_t nbytes);

    // Chardev -> IO Device notifications (IRQ)
    void   (*notify)(void* io_dev, uint32_t flags);

    // Common RVVM API features
    void   (*update)(struct chardev* dev);
    void   (*remove)(struct chardev* dev);

    void* data;
    void* io_dev;
};
#endif

#ifdef __cplusplus
}
#endif // __cplusplus


#ifdef __cplusplus
#include <wx/msgqueue.h>
class RVVMGenericGui:public rvvmgenericbase
{
    std::recursive_mutex m_vm_gui_lock;
public:
    RVVMGenericGui(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 750,500 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString);
    virtual ~RVVMGenericGui();

protected:
    virtual void OnChar_RVVM_Generic_Serialport0( wxKeyEvent& event );
    virtual void OnKeyDown_RVVM_Generic_Serialport0( wxKeyEvent& event );
    virtual void OnButtonClick_RVVM_Generic_Start( wxCommandEvent& event );
    virtual void OnButtonClick_RVVM_Generic_Stop( wxCommandEvent& event );
    virtual void OnButtonClick_RVVM_Generic_Quit( wxCommandEvent& event );
    virtual void OnTimer_RVVM_MS_Timer( wxTimerEvent& event );

private:
    virtual void LoadDefaultMachineSettings();
    virtual rvvm_machine_t *CreateMachine(const char *isa=NULL);
    virtual void RunMachine(rvvm_machine_t *machine);
    virtual void StopMachine(rvvm_machine_t *machine);
    rvvm_machine_t * m_running_machine;
    struct rvvm_serialport_t
    {
        rvvm_device_chardev_t dev;
        size_t index;
        RVVMGenericGui *parent;
        wxMessageQueue<wxString> Output;
        wxMessageQueue<uint8_t> Input;
        std::atomic<size_t> InputDataCount;
        std::atomic<uint32_t> Flags;
    } m_machine_serialport[1];
    virtual void InitMachineSerialport();
    virtual void MachineSerialportLoop();

};
#endif // __cplusplus

#endif // __RVVMGENERICGUI_H__
