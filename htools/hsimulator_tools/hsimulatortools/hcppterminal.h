#ifndef HCPPTERMINAL_H
#define HCPPTERMINAL_H

#ifdef __cplusplus
#include "terminalwx.h"
#include "functional"

class HCPPTerminal: virtual public TerminalWx
{
    std::function<void(wxString)> m_input_callback;
public:
    HCPPTerminal(wxWindow* parent, wxWindowID id,const wxPoint& pos,int width=80, int height=24,const wxString& name=_T("HCPPTermainl"));
    virtual ~HCPPTerminal();
    void set_input_callback(std::function<void(wxString)> cb);
protected:
    virtual void OnUserInput(wxString input);

private:
};

#endif // __cplusplus

#endif // HCPPTERMINAL_H
