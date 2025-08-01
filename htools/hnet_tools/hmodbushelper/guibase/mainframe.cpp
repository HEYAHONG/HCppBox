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

	m_menu_session = new wxMenu();
	wxMenuItem* m_menuItem_new_modbussession_tcp_client;
	m_menuItem_new_modbussession_tcp_client = new wxMenuItem( m_menu_session, wxID_ANY, wxString( _("新建Modbus TCP客户端") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_session->Append( m_menuItem_new_modbussession_tcp_client );

	m_mainframe_menubar->Append( m_menu_session, _("会话") );

	m_menu_help = new wxMenu();
	wxMenuItem* m_menuItem_Help_About;
	m_menuItem_Help_About = new wxMenuItem( m_menu_help, wxID_ANY, wxString( _("关于") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_help->Append( m_menuItem_Help_About );

	m_mainframe_menubar->Append( m_menu_help, _("帮助") );

	this->SetMenuBar( m_mainframe_menubar );

	m_ms_timer.SetOwner( this, m_ms_timer.GetId() );
	m_ms_timer.Start( 1 );

	m_main_panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_main_panel, wxAuiPaneInfo() .Center() .CaptionVisible( false ).CloseButton( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).BottomDockable( false ).TopDockable( false ).LeftDockable( false ).RightDockable( false ).Floatable( false ) );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_main_auinotebook = new wxAuiNotebook( m_main_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE );

	bSizer2->Add( m_main_auinotebook, 1, wxEXPAND | wxALL, 5 );


	m_main_panel->SetSizer( bSizer2 );
	m_main_panel->Layout();
	bSizer2->Fit( m_main_panel );

	m_mgr.Update();
	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_ACTIVATE, wxActivateEventHandler( mainframe::OnActivate ) );
	m_menu_file->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnMenuSelection_Menu_Quit ), this, m_menuItem_quit->GetId());
	m_menu_log->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnMenuSelection_Menu_Logdialog ), this, m_menuItem_logdialog->GetId());
	m_menu_session->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnMenuSelection_New_Modbus_Session_TCP_Client ), this, m_menuItem_new_modbussession_tcp_client->GetId());
	m_menu_help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnAbout ), this, m_menuItem_Help_About->GetId());
	this->Connect( m_ms_timer.GetId(), wxEVT_TIMER, wxTimerEventHandler( mainframe::OnMSTimer ) );
}

mainframe::~mainframe()
{
	m_mgr.UnInit();

}

modbussessiontcpclientbase::modbussessiontcpclientbase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_auinotebook_tcp_client = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TAB_MOVE|wxAUI_NB_TAB_SPLIT|wxAUI_NB_TOP );
	m_panel_coils = new wxPanel( m_auinotebook_tcp_client, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_grid_coils = new wxGrid( m_panel_coils, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid_coils->CreateGrid( 65536, 1 );
	m_grid_coils->EnableEditing( true );
	m_grid_coils->EnableGridLines( true );
	m_grid_coils->EnableDragGridSize( false );
	m_grid_coils->SetMargins( 0, 0 );

	// Columns
	m_grid_coils->EnableDragColMove( false );
	m_grid_coils->EnableDragColSize( true );
	m_grid_coils->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid_coils->EnableDragRowSize( true );
	m_grid_coils->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid_coils->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer5->Add( m_grid_coils, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl_coils_log = new wxTextCtrl( m_panel_coils, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizer6->Add( m_textCtrl_coils_log, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1 = new wxStaticText( m_panel_coils, wxID_ANY, _("起始地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer7->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_coils_addr_base = new wxTextCtrl( m_panel_coils, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_coils_addr_base->SetMinSize( wxSize( 60,-1 ) );

	bSizer7->Add( m_textCtrl_coils_addr_base, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText2 = new wxStaticText( m_panel_coils, wxID_ANY, _("长度："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer7->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_coils_addr_length = new wxTextCtrl( m_panel_coils, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_coils_addr_length->SetMinSize( wxSize( 40,-1 ) );

	bSizer7->Add( m_textCtrl_coils_addr_length, 0, wxALL, 5 );

	m_button_coils_read = new wxButton( m_panel_coils, wxID_ANY, _(" 读取"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_button_coils_read, 0, wxALL, 5 );

	m_button_coils_write = new wxButton( m_panel_coils, wxID_ANY, _("写入"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_button_coils_write, 0, wxALL, 5 );

	m_button_coils_write_single = new wxButton( m_panel_coils, wxID_ANY, _("单个写入"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_button_coils_write_single, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer6->Add( bSizer7, 0, 0, 5 );


	bSizer5->Add( bSizer6, 1, wxEXPAND, 5 );


	m_panel_coils->SetSizer( bSizer5 );
	m_panel_coils->Layout();
	bSizer5->Fit( m_panel_coils );
	m_auinotebook_tcp_client->AddPage( m_panel_coils, _("线圈"), false, wxNullBitmap );

	bSizer3->Add( m_auinotebook_tcp_client, 1, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_textCtrl_ip = new wxTextCtrl( this, wxID_ANY, _("localhost:502"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_ip->SetMinSize( wxSize( 300,-1 ) );

	bSizer4->Add( m_textCtrl_ip, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_button_connect_disconnect = new wxButton( this, wxID_ANY, _("连接"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button_connect_disconnect, 0, wxALL, 5 );


	bSizer3->Add( bSizer4, 0, 0, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();

	// Connect Events
	this->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( modbussessiontcpclientbase::OnSetFocus ) );
	m_grid_coils->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( modbussessiontcpclientbase::OnGridCellChange_Modbus_Coils ), NULL, this );
	m_button_coils_read->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Coils_Read ), NULL, this );
	m_button_coils_write->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Coils_Write ), NULL, this );
	m_button_coils_write_single->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Coils_Write_Single ), NULL, this );
	m_button_connect_disconnect->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Connect_DisConnect ), NULL, this );
}

modbussessiontcpclientbase::~modbussessiontcpclientbase()
{
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
