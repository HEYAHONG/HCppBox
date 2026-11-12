#ifndef __MINIRISVCGENERICGUI_H__
#define __MINIRISVCGENERICGUI_H__

#include <wx/panel.h>
#include <wx/listbook.h>
#include <wx/button.h>
#include <wx/timer.h>
#include <wx/filepicker.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/string.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "hcppterminal.h"

struct MiniRISCVImpl;

/*
 * HBox Mini-RISCV模拟器页面
 * 参考hmini-riscv实现，使用虚拟CPU模型。
 */
class MiniRISCVGenericGui: public wxPanel
{
    friend struct MiniRISCVImpl;
public:
    MiniRISCVGenericGui(wxWindow* parent,wxWindowID id=wxID_ANY,
                        const wxPoint& pos=wxDefaultPosition,
                        const wxSize& size=wxDefaultSize,
                        long style=wxTAB_TRAVERSAL,
                        const wxString& name=wxEmptyString);
    virtual ~MiniRISCVGenericGui();

private:
    void OnButtonClick_Start(wxCommandEvent& event);
    void OnButtonClick_Stop(wxCommandEvent& event);
    void OnButtonClick_Quit(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);

    bool LoadImages(wxString& error);
    void RunVM();
    void StopVM();
    void SetRunningUI(bool running);

    void AppendOutput(const void* data,size_t size);
    void PushInput(const std::string& data);
    bool HasInput();
    uint8_t GetInput();
    void DrainOutput();

    wxListbook* m_listbook;
    wxPanel* m_panel_settings;
    wxPanel* m_panel_serial;
    HCPPTerminal* m_terminal;
    wxFilePickerCtrl* m_filePicker_rom;
    wxFilePickerCtrl* m_filePicker_flash;
    wxChoice* m_choice_arch;
    wxTextCtrl* m_textCtrl_ram_size;
    wxButton* m_button_start;
    wxButton* m_button_stop;
    wxButton* m_button_quit;
    wxTimer m_timer;

    std::thread m_thread;
    std::atomic<bool> m_running;
    std::atomic<bool> m_stop_requested;
    std::atomic<bool> m_finished;

    std::mutex m_io_mutex;
    std::string m_input;
    std::string m_output;

    std::unique_ptr<MiniRISCVImpl> m_impl;
};

#endif // __MINIRISVCGENERICGUI_H__
