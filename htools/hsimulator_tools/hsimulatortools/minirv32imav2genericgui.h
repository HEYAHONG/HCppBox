#ifndef __MINIRV32IMAV2GENERICGUI_H__
#define __MINIRV32IMAV2GENERICGUI_H__

#include <wx/panel.h>
#include <wx/listbook.h>
#include <wx/button.h>
#include <wx/timer.h>
#include <wx/filepicker.h>
#include <wx/checkbox.h>
#include <wx/string.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "hcppterminal.h"

struct MiniRV32IMAImpl;

/*
 * HBox mini-rv32imav2模拟器页面
 * 参考hmini-rv32imav2实现，不包含openblt
 */
class MiniRV32IMAV2GenericGui: public wxPanel
{
    friend struct MiniRV32IMAImpl;
public:
    MiniRV32IMAV2GenericGui(wxWindow* parent,wxWindowID id=wxID_ANY,
                            const wxPoint& pos=wxDefaultPosition,
                            const wxSize& size=wxDefaultSize,
                            long style=wxTAB_TRAVERSAL,
                            const wxString& name=wxEmptyString);
    virtual ~MiniRV32IMAV2GenericGui();

private:
    void OnButtonClick_MiniRV32IMA_Start(wxCommandEvent& event);
    void OnButtonClick_MiniRV32IMA_Stop(wxCommandEvent& event);
    void OnButtonClick_MiniRV32IMA_Quit(wxCommandEvent& event);
    void OnTimer_MiniRV32IMA_MS_Timer(wxTimerEvent& event);

    bool LoadImageFile(wxString& error);
    void RunVM();
    void StopVM();
    void SetRunningUI(bool running);

    void AppendOutput(const void* data,size_t size);
    void PushInput(const std::string& data);
    bool HasInput();
    uint8_t GetInput();
    void DrainOutput();

    wxListbook* m_listbook_minirv32ima;
    wxPanel* m_panel_minirv32ima_settings;
    wxPanel* m_panel_minirv32ima_serialport;
    HCPPTerminal* m_HCPPTerminal_Serialport0;
    wxFilePickerCtrl* m_filePicker_minirv32ima_image;
    wxFilePickerCtrl* m_filePicker_minirv32ima_dtb;
    wxCheckBox* m_checkBox_minirv32ima_display_dtb;
    wxButton* m_button_minirv32ima_start;
    wxButton* m_button_minirv32ima_stop;
    wxButton* m_button_minirv32ima_quit;
    wxTimer m_timer_minirv32ima_ms_timer;

    std::thread m_thread;
    std::atomic<bool> m_running;
    std::atomic<bool> m_stop_requested;
    std::atomic<bool> m_finished;

    std::mutex m_io_mutex;
    std::string m_input;
    std::string m_output;

    std::unique_ptr<MiniRV32IMAImpl> m_impl;
};

#endif // __MINIRV32IMAV2GENERICGUI_H__
