#include "hcppterminal.h"

HCPPTerminal::HCPPTerminal(wxWindow* parent, wxWindowID id,const wxPoint& pos,int width, int height,const wxString& name):TerminalWx(parent, id,pos,width, height,name)
{
    //ctor
}

void HCPPTerminal::set_input_callback(std::function<void(wxString)> cb)
{
    m_input_callback=cb;
}

void HCPPTerminal::OnUserInput(wxString input)
{
    auto cb=m_input_callback;
    if(cb!=NULL)
    {
        cb(input);
    }
}

HCPPTerminal::~HCPPTerminal()
{
    //dtor
}
