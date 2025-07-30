///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-HYH_USED-0-gdf26f269)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "guibase.h"

#include "mainframe.h"

///////////////////////////////////////////////////////////////////////////

mainframe::mainframe( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 800,600 ), wxDefaultSize );
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);

	m_mainframe_statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	m_mainframe_menubar = new wxMenuBar( 0 );
	m_menu_file = new wxMenu();
	wxMenuItem* m_menuItem_quit;
	m_menuItem_quit = new wxMenuItem( m_menu_file, wxID_ANY, wxString( _("退出") ) + wxT('\t') + wxT("CTRL+Q"), wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menuItem_quit );

	m_mainframe_menubar->Append( m_menu_file, _("文件") );

	m_menu_log = new wxMenu();
	wxMenuItem* m_menuItem_logdialog;
	m_menuItem_logdialog = new wxMenuItem( m_menu_log, wxID_ANY, wxString( _("日志窗口") ) , wxEmptyString, wxITEM_CHECK );
	m_menu_log->Append( m_menuItem_logdialog );

	m_mainframe_menubar->Append( m_menu_log, _("日志") );

	m_menu_help = new wxMenu();
	wxMenuItem* m_menuItem_Help_About;
	m_menuItem_Help_About = new wxMenuItem( m_menu_help, wxID_ANY, wxString( _("关于") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_help->Append( m_menuItem_Help_About );

	m_mainframe_menubar->Append( m_menu_help, _("帮助") );

	this->SetMenuBar( m_mainframe_menubar );

	m_ms_timer.SetOwner( this, m_ms_timer.GetId() );
	m_ms_timer.Start( 1 );


	m_mgr.Update();
	this->Centre( wxBOTH );

	// Connect Events
	m_menu_file->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnMenuSelection_Menu_Quit ), this, m_menuItem_quit->GetId());
	m_menu_log->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnMenuSelection_Menu_Logdialog ), this, m_menuItem_logdialog->GetId());
	m_menu_help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnAbout ), this, m_menuItem_Help_About->GetId());
	this->Connect( m_ms_timer.GetId(), wxEVT_TIMER, wxTimerEventHandler( mainframe::OnMSTimer ) );
}

mainframe::~mainframe()
{
	m_mgr.UnInit();

}

logdialog::logdialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 480,320 ), wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_logdialog_textCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY );
	bSizer1->Add( m_logdialog_textCtrl, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );

	this->Centre( wxBOTH );
}

logdialog::~logdialog()
{
}
