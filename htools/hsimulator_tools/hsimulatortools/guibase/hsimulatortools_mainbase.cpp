///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-HYH_USED-0-gdf26f269)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "guibase.h"

#include "hsimulatortools_mainbase.h"

///////////////////////////////////////////////////////////////////////////

mainframe::mainframe( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 600,800 ), wxDefaultSize );
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_ALLOW_FLOATING|wxAUI_MGR_DEFAULT|wxAUI_MGR_LIVE_RESIZE);

	m_statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	m_statusBar->SetMinSize( wxSize( -1,20 ) );

	m_menubar = new wxMenuBar( 0 );
	m_menu_file = new wxMenu();
	wxMenuItem* m_menu_file_quit;
	m_menu_file_quit = new wxMenuItem( m_menu_file, ID_MENU_FILE_QUIT, wxString( _("退出") ) + wxT('\t') + wxT("CTRL+Q"), wxEmptyString, wxITEM_NORMAL );
	m_menu_file->Append( m_menu_file_quit );

	m_menubar->Append( m_menu_file, _("文件") );

	m_menu_help = new wxMenu();
	wxMenuItem* m_menu_help_about;
	m_menu_help_about = new wxMenuItem( m_menu_help, ID_MENU_HELP_ABOUT, wxString( _("关于") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_help->Append( m_menu_help_about );

	m_menubar->Append( m_menu_help, _("帮助") );

	this->SetMenuBar( m_menubar );

	mainpanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( mainpanel, wxAuiPaneInfo() .Center() .CloseButton( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).BottomDockable( false ).Floatable( false ).MinSize( wxSize( -1,500 ) ) );

	wxGridSizer* maingSizer;
	maingSizer = new wxGridSizer( 0, 1, 0, 0 );

	mainauinotebook = new wxAuiNotebook( mainpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TAB_MOVE|wxAUI_NB_TAB_SPLIT|wxAUI_NB_TOP );

	maingSizer->Add( mainauinotebook, 1, wxEXPAND | wxALL, 5 );


	mainpanel->SetSizer( maingSizer );
	mainpanel->Layout();
	maingSizer->Fit( mainpanel );
	logspanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	logspanel->SetMinSize( wxSize( 600,100 ) );
	logspanel->SetMaxSize( wxSize( -1,250 ) );

	m_mgr.AddPane( logspanel, wxAuiPaneInfo() .Name( wxT("logs") ).Center() .CaptionVisible( false ).CloseButton( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).TopDockable( false ).Floatable( false ).BestSize( wxSize( 600,200 ) ) );

	wxBoxSizer* logsbSizer1;
	logsbSizer1 = new wxBoxSizer( wxVERTICAL );

	logsauinotebook = new wxAuiNotebook( logspanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TAB_MOVE|wxAUI_NB_TAB_SPLIT|wxAUI_NB_TOP );
	logsmainpanel = new wxPanel( logsauinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridSizer* logsmaingSizer;
	logsmaingSizer = new wxGridSizer( 0, 1, 0, 0 );

	m_textCtrl_log = new wxTextCtrl( logsmainpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_AUTO_URL|wxTE_MULTILINE|wxTE_READONLY );
	logsmaingSizer->Add( m_textCtrl_log, 1, wxALL|wxEXPAND, 5 );


	logsmainpanel->SetSizer( logsmaingSizer );
	logsmainpanel->Layout();
	logsmaingSizer->Fit( logsmainpanel );
	logsauinotebook->AddPage( logsmainpanel, _("日志"), false, wxNullBitmap );

	logsbSizer1->Add( logsauinotebook, 1, wxEXPAND | wxALL, 5 );


	logspanel->SetSizer( logsbSizer1 );
	logspanel->Layout();
	logsbSizer1->Fit( logspanel );
	m_timer_ms.SetOwner( this, m_timer_ms.GetId() );
	m_timer_ms.Start( 1 );


	m_mgr.Update();
	this->Centre( wxBOTH );

	// Connect Events
	m_menu_file->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnQuit ), this, m_menu_file_quit->GetId());
	m_menu_help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnAbout ), this, m_menu_help_about->GetId());
	this->Connect( m_timer_ms.GetId(), wxEVT_TIMER, wxTimerEventHandler( mainframe::OnMSTimer ) );
}

mainframe::~mainframe()
{
	// Disconnect Events
	this->Disconnect( m_timer_ms.GetId(), wxEVT_TIMER, wxTimerEventHandler( mainframe::OnMSTimer ) );

	m_mgr.UnInit();

}
