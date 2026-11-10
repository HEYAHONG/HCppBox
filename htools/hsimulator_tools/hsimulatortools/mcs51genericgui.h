#ifndef __MCS51GENERICGUI_H__
#define __MCS51GENERICGUI_H__

#include <wx/panel.h>
#include <wx/listbook.h>
#include <wx/button.h>
#include <wx/timer.h>
#include <wx/filepicker.h>
#include <wx/string.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "hcppterminal.h"

class MCS51VM;

/*
 * HBox Mini-MCS51模拟器页面
 * 参考hmini-mcs51实现，不包含openblt
 */
class MCS51GenericGui: public wxPanel
{
    friend class MCS51VM;
public:
    MCS51GenericGui(wxWindow* parent,wxWindowID id=wxID_ANY,
                    const wxPoint& pos=wxDefaultPosition,
                    const wxSize& size=wxDefaultSize,
                    long style=wxTAB_TRAVERSAL,
                    const wxString& name=wxEmptyString);
    virtual ~MCS51GenericGui();

private:
    void OnButtonClick_MCS51_Start(wxCommandEvent& event);
    void OnButtonClick_MCS51_Stop(wxCommandEvent& event);
    void OnButtonClick_MCS51_Quit(wxCommandEvent& event);
    void OnTimer_MCS51_MS_Timer(wxTimerEvent& event);

    bool LoadProgram(wxString& error);
    void RunVM();
    void StopVM();
    void SetRunningUI(bool running);

    void AppendOutput(const void* data,size_t size);
    void PushInput(const std::string& data);
    bool HasInput();
    uint8_t GetInput();
    void DrainOutput();

    wxListbook* m_listbook_mcs51;
    wxPanel* m_panel_mcs51_settings;
    wxPanel* m_panel_mcs51_serialport;
    HCPPTerminal* m_HCPPTerminal_Serialport0;
    wxFilePickerCtrl* m_filePicker_mcs51_program;
    wxButton* m_button_mcs51_start;
    wxButton* m_button_mcs51_stop;
    wxButton* m_button_mcs51_quit;
    wxTimer m_timer_mcs51_ms_timer;

    std::thread m_thread;
    std::atomic<bool> m_running;
    std::atomic<bool> m_stop_requested;
    std::atomic<bool> m_finished;

    std::mutex m_io_mutex;
    std::string m_input;
    std::string m_output;

    std::unique_ptr<MCS51VM> m_vm;
};

#endif // __MCS51GENERICGUI_H__
