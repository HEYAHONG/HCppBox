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

	wxMenuItem* m_menuItem_new_modbussession_tcp_gateway_simulator;
	m_menuItem_new_modbussession_tcp_gateway_simulator = new wxMenuItem( m_menu_session, wxID_ANY, wxString( _("新建Modbus TCP网关(模拟器)") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_session->Append( m_menuItem_new_modbussession_tcp_gateway_simulator );

	m_mainframe_menubar->Append( m_menu_session, _("会话") );

	m_menu_help = new wxMenu();
	wxMenuItem* m_menuItem_Modbus_Protocol;
	m_menuItem_Modbus_Protocol = new wxMenuItem( m_menu_help, wxID_ANY, wxString( _("Modbus应用协议") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_help->Append( m_menuItem_Modbus_Protocol );

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
	m_menu_session->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnMenuSelection_New_Modbus_Session_TCP_Gateway_Simulator ), this, m_menuItem_new_modbussession_tcp_gateway_simulator->GetId());
	m_menu_help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnMenuSelection_Modbus_Protocol ), this, m_menuItem_Modbus_Protocol->GetId());
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

	m_staticline10 = new wxStaticLine( m_panel_coils, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer6->Add( m_staticline10, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1 = new wxStaticText( m_panel_coils, wxID_ANY, _("起始地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer7->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_coils_addr_base = new wxTextCtrl( m_panel_coils, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_coils_addr_base->SetMinSize( wxSize( 60,-1 ) );

	bSizer7->Add( m_textCtrl_coils_addr_base, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText2 = new wxStaticText( m_panel_coils, wxID_ANY, _("数量："), wxDefaultPosition, wxDefaultSize, 0 );
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

	m_staticline11 = new wxStaticLine( m_panel_coils, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer6->Add( m_staticline11, 0, wxEXPAND | wxALL, 5 );


	bSizer5->Add( bSizer6, 1, wxEXPAND, 5 );


	m_panel_coils->SetSizer( bSizer5 );
	m_panel_coils->Layout();
	bSizer5->Fit( m_panel_coils );
	m_auinotebook_tcp_client->AddPage( m_panel_coils, _("线圈"), true, wxNullBitmap );
	m_panel_discrete_input = new wxPanel( m_auinotebook_tcp_client, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	m_grid_discrete_input = new wxGrid( m_panel_discrete_input, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid_discrete_input->CreateGrid( 65536, 1 );
	m_grid_discrete_input->EnableEditing( true );
	m_grid_discrete_input->EnableGridLines( true );
	m_grid_discrete_input->EnableDragGridSize( false );
	m_grid_discrete_input->SetMargins( 0, 0 );

	// Columns
	m_grid_discrete_input->EnableDragColMove( false );
	m_grid_discrete_input->EnableDragColSize( true );
	m_grid_discrete_input->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid_discrete_input->EnableDragRowSize( true );
	m_grid_discrete_input->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid_discrete_input->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer8->Add( m_grid_discrete_input, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl_discrete_input_log = new wxTextCtrl( m_panel_discrete_input, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizer9->Add( m_textCtrl_discrete_input_log, 1, wxALL|wxEXPAND, 5 );

	m_staticline8 = new wxStaticLine( m_panel_discrete_input, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer9->Add( m_staticline8, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText3 = new wxStaticText( m_panel_discrete_input, wxID_ANY, _("起始地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer10->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_discrete_input_addr_base = new wxTextCtrl( m_panel_discrete_input, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_discrete_input_addr_base->SetMinSize( wxSize( 60,-1 ) );

	bSizer10->Add( m_textCtrl_discrete_input_addr_base, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText4 = new wxStaticText( m_panel_discrete_input, wxID_ANY, _("数量："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer10->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_discrete_input_addr_length = new wxTextCtrl( m_panel_discrete_input, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_discrete_input_addr_length->SetMinSize( wxSize( 40,-1 ) );

	bSizer10->Add( m_textCtrl_discrete_input_addr_length, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_button_discrete_input_read = new wxButton( m_panel_discrete_input, wxID_ANY, _("读取"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_button_discrete_input_read, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer9->Add( bSizer10, 0, 0, 5 );

	m_staticline9 = new wxStaticLine( m_panel_discrete_input, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer9->Add( m_staticline9, 0, wxEXPAND | wxALL, 5 );


	bSizer8->Add( bSizer9, 1, wxEXPAND, 5 );


	m_panel_discrete_input->SetSizer( bSizer8 );
	m_panel_discrete_input->Layout();
	bSizer8->Fit( m_panel_discrete_input );
	m_auinotebook_tcp_client->AddPage( m_panel_discrete_input, _("离散输入"), false, wxNullBitmap );
	m_panel_holding_registers = new wxPanel( m_auinotebook_tcp_client, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );

	m_grid_holding_registers = new wxGrid( m_panel_holding_registers, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid_holding_registers->CreateGrid( 65536, 1 );
	m_grid_holding_registers->EnableEditing( true );
	m_grid_holding_registers->EnableGridLines( true );
	m_grid_holding_registers->EnableDragGridSize( false );
	m_grid_holding_registers->SetMargins( 0, 0 );

	// Columns
	m_grid_holding_registers->SetColSize( 0, 120 );
	m_grid_holding_registers->EnableDragColMove( false );
	m_grid_holding_registers->EnableDragColSize( false );
	m_grid_holding_registers->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid_holding_registers->EnableDragRowSize( true );
	m_grid_holding_registers->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid_holding_registers->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	bSizer11->Add( m_grid_holding_registers, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl_holding_registers_log = new wxTextCtrl( m_panel_holding_registers, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizer12->Add( m_textCtrl_holding_registers_log, 1, wxALL|wxEXPAND, 5 );

	m_staticline5 = new wxStaticLine( m_panel_holding_registers, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer12->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText5 = new wxStaticText( m_panel_holding_registers, wxID_ANY, _("起始地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer13->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_holding_registers_addr_base = new wxTextCtrl( m_panel_holding_registers, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_holding_registers_addr_base->SetMinSize( wxSize( 60,-1 ) );

	bSizer13->Add( m_textCtrl_holding_registers_addr_base, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText6 = new wxStaticText( m_panel_holding_registers, wxID_ANY, _("数量："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer13->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_holding_registers_addr_length = new wxTextCtrl( m_panel_holding_registers, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_holding_registers_addr_length->SetMinSize( wxSize( 40,-1 ) );

	bSizer13->Add( m_textCtrl_holding_registers_addr_length, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_button_holding_registers_read = new wxButton( m_panel_holding_registers, wxID_ANY, _("读取"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_button_holding_registers_read, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_button_holding_registers_write = new wxButton( m_panel_holding_registers, wxID_ANY, _("写入"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_button_holding_registers_write, 0, wxALL, 5 );

	m_button_holding_registers_write_single = new wxButton( m_panel_holding_registers, wxID_ANY, _("单个写入"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_button_holding_registers_write_single, 0, wxALL, 5 );


	bSizer12->Add( bSizer13, 0, 0, 5 );

	m_staticline2 = new wxStaticLine( m_panel_holding_registers, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer12->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText11 = new wxStaticText( m_panel_holding_registers, wxID_ANY, _("读地址:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer17->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_holding_registers_read_write_read_addr_base = new wxTextCtrl( m_panel_holding_registers, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_holding_registers_read_write_read_addr_base->SetMinSize( wxSize( 48,-1 ) );

	bSizer17->Add( m_textCtrl_holding_registers_read_write_read_addr_base, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText12 = new wxStaticText( m_panel_holding_registers, wxID_ANY, _("数量:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer17->Add( m_staticText12, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_holding_registers_read_write_read_addr_length = new wxTextCtrl( m_panel_holding_registers, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_holding_registers_read_write_read_addr_length->SetMinSize( wxSize( 40,-1 ) );

	bSizer17->Add( m_textCtrl_holding_registers_read_write_read_addr_length, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText13 = new wxStaticText( m_panel_holding_registers, wxID_ANY, _("写地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer17->Add( m_staticText13, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_holding_registers_read_write_write_addr_base = new wxTextCtrl( m_panel_holding_registers, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_holding_registers_read_write_write_addr_base->SetMinSize( wxSize( 48,-1 ) );

	bSizer17->Add( m_textCtrl_holding_registers_read_write_write_addr_base, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText14 = new wxStaticText( m_panel_holding_registers, wxID_ANY, _("数量："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer17->Add( m_staticText14, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_holding_registers_read_write_write_addr_length = new wxTextCtrl( m_panel_holding_registers, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_holding_registers_read_write_write_addr_length->SetMinSize( wxSize( 40,-1 ) );

	bSizer17->Add( m_textCtrl_holding_registers_read_write_write_addr_length, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_button_holding_registers_read_write = new wxButton( m_panel_holding_registers, wxID_ANY, _("写后读"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_button_holding_registers_read_write, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer12->Add( bSizer17, 0, 0, 5 );

	m_staticline3 = new wxStaticLine( m_panel_holding_registers, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer12->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText17 = new wxStaticText( m_panel_holding_registers, wxID_ANY, _("掩码写入地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	bSizer18->Add( m_staticText17, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_holding_registers_mask_write_addr = new wxTextCtrl( m_panel_holding_registers, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_holding_registers_mask_write_addr->SetMinSize( wxSize( 60,-1 ) );

	bSizer18->Add( m_textCtrl_holding_registers_mask_write_addr, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText15 = new wxStaticText( m_panel_holding_registers, wxID_ANY, _("And:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer18->Add( m_staticText15, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_holding_registers_mask_write_and_mask = new wxTextCtrl( m_panel_holding_registers, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_holding_registers_mask_write_and_mask->SetMinSize( wxSize( 60,-1 ) );

	bSizer18->Add( m_textCtrl_holding_registers_mask_write_and_mask, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText16 = new wxStaticText( m_panel_holding_registers, wxID_ANY, _("Or:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer18->Add( m_staticText16, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_holding_registers_mask_write_or_mask = new wxTextCtrl( m_panel_holding_registers, wxID_ANY, _("65535"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_holding_registers_mask_write_or_mask->SetMinSize( wxSize( 60,-1 ) );

	bSizer18->Add( m_textCtrl_holding_registers_mask_write_or_mask, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_button_holding_registers_mask_write = new wxButton( m_panel_holding_registers, wxID_ANY, _("掩码写入"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( m_button_holding_registers_mask_write, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer12->Add( bSizer18, 0, 0, 5 );

	m_staticline4 = new wxStaticLine( m_panel_holding_registers, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer12->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );


	bSizer11->Add( bSizer12, 1, wxEXPAND, 5 );


	m_panel_holding_registers->SetSizer( bSizer11 );
	m_panel_holding_registers->Layout();
	bSizer11->Fit( m_panel_holding_registers );
	m_auinotebook_tcp_client->AddPage( m_panel_holding_registers, _("保持寄存器"), false, wxNullBitmap );
	m_panel_input_registers = new wxPanel( m_auinotebook_tcp_client, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );

	m_grid_input_registers = new wxGrid( m_panel_input_registers, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid_input_registers->CreateGrid( 65535, 1 );
	m_grid_input_registers->EnableEditing( true );
	m_grid_input_registers->EnableGridLines( true );
	m_grid_input_registers->EnableDragGridSize( false );
	m_grid_input_registers->SetMargins( 0, 0 );

	// Columns
	m_grid_input_registers->SetColSize( 0, 120 );
	m_grid_input_registers->EnableDragColMove( false );
	m_grid_input_registers->EnableDragColSize( false );
	m_grid_input_registers->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid_input_registers->EnableDragRowSize( true );
	m_grid_input_registers->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid_input_registers->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	bSizer14->Add( m_grid_input_registers, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl_input_registers_log = new wxTextCtrl( m_panel_input_registers, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizer15->Add( m_textCtrl_input_registers_log, 1, wxALL|wxEXPAND, 5 );

	m_staticline6 = new wxStaticLine( m_panel_input_registers, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer15->Add( m_staticline6, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText7 = new wxStaticText( m_panel_input_registers, wxID_ANY, _("起始地址："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer16->Add( m_staticText7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_input_registers_addr_base = new wxTextCtrl( m_panel_input_registers, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_input_registers_addr_base->SetMinSize( wxSize( 60,-1 ) );

	bSizer16->Add( m_textCtrl_input_registers_addr_base, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText8 = new wxStaticText( m_panel_input_registers, wxID_ANY, _("数量:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer16->Add( m_staticText8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_input_registers_addr_length = new wxTextCtrl( m_panel_input_registers, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_input_registers_addr_length->SetMinSize( wxSize( 40,-1 ) );

	bSizer16->Add( m_textCtrl_input_registers_addr_length, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_button_input_registers_read = new wxButton( m_panel_input_registers, wxID_ANY, _("读取"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_button_input_registers_read, 0, wxALL, 5 );


	bSizer15->Add( bSizer16, 0, 0, 5 );

	m_staticline7 = new wxStaticLine( m_panel_input_registers, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer15->Add( m_staticline7, 0, wxEXPAND | wxALL, 5 );


	bSizer14->Add( bSizer15, 1, wxEXPAND, 5 );


	m_panel_input_registers->SetSizer( bSizer14 );
	m_panel_input_registers->Layout();
	bSizer14->Fit( m_panel_input_registers );
	m_auinotebook_tcp_client->AddPage( m_panel_input_registers, _("输入寄存器"), false, wxNullBitmap );

	bSizer3->Add( m_auinotebook_tcp_client, 1, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_textCtrl_ip = new wxTextCtrl( this, wxID_ANY, _("localhost:502"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_ip->SetMinSize( wxSize( 300,-1 ) );

	bSizer4->Add( m_textCtrl_ip, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_button_connect_disconnect = new wxButton( this, wxID_ANY, _("连接"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button_connect_disconnect, 0, wxALL, 5 );

	m_staticText9 = new wxStaticText( this, wxID_ANY, _("从机地址:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer4->Add( m_staticText9, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_salve_addr = new wxTextCtrl( this, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_salve_addr->SetMinSize( wxSize( 40,-1 ) );

	bSizer4->Add( m_textCtrl_salve_addr, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText_tid = new wxStaticText( this, wxID_ANY, _("Tid:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_tid->Wrap( -1 );
	bSizer4->Add( m_staticText_tid, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_Tid = new wxTextCtrl( this, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrl_Tid->SetMinSize( wxSize( 60,-1 ) );

	bSizer4->Add( m_textCtrl_Tid, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_checkBox_Tid_Inc = new wxCheckBox( this, wxID_ANY, _("Tid自增"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox_Tid_Inc->SetValue(true);
	bSizer4->Add( m_checkBox_Tid_Inc, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer3->Add( bSizer4, 0, 0, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();

	// Connect Events
	this->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( modbussessiontcpclientbase::OnSetFocus ) );
	m_grid_coils->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( modbussessiontcpclientbase::OnGridCellChange_Modbus_Coils ), NULL, this );
	m_button_coils_read->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Coils_Read ), NULL, this );
	m_button_coils_write->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Coils_Write ), NULL, this );
	m_button_coils_write_single->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Coils_Write_Single ), NULL, this );
	m_button_discrete_input_read->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Discrete_Input_Read ), NULL, this );
	m_grid_holding_registers->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( modbussessiontcpclientbase::OnGridCellChange_Modbus_Holding_Registers ), NULL, this );
	m_button_holding_registers_read->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Holding_Registers_Read ), NULL, this );
	m_button_holding_registers_write->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Holding_Registers_Write ), NULL, this );
	m_button_holding_registers_write_single->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Holding_Registers_Write_Single ), NULL, this );
	m_button_holding_registers_read_write->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Holding_Registers_Read_Write ), NULL, this );
	m_button_holding_registers_mask_write->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Holding_Registers_Mask_Write ), NULL, this );
	m_button_input_registers_read->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Input_Registers_Read ), NULL, this );
	m_button_connect_disconnect->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpclientbase::OnButtonClick_Connect_DisConnect ), NULL, this );
}

modbussessiontcpclientbase::~modbussessiontcpclientbase()
{
}

modbussessiontcpgatewaysimulatorbase::modbussessiontcpgatewaysimulatorbase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );

	m_grid_simulator_data = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid_simulator_data->CreateGrid( 65536, 4 );
	m_grid_simulator_data->EnableEditing( true );
	m_grid_simulator_data->EnableGridLines( true );
	m_grid_simulator_data->EnableDragGridSize( true );
	m_grid_simulator_data->SetMargins( 0, 0 );

	// Columns
	m_grid_simulator_data->EnableDragColMove( false );
	m_grid_simulator_data->EnableDragColSize( true );
	m_grid_simulator_data->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid_simulator_data->EnableDragRowSize( true );
	m_grid_simulator_data->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid_simulator_data->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer19->Add( m_grid_simulator_data, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );

	bSizer20->SetMinSize( wxSize( 150,-1 ) );
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText21 = new wxStaticText( this, wxID_ANY, _("仅本地："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	bSizer24->Add( m_staticText21, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_checkBox_localhost = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox_localhost->SetValue(true);
	bSizer24->Add( m_checkBox_localhost, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer20->Add( bSizer24, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText19 = new wxStaticText( this, wxID_ANY, _("端口:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	bSizer22->Add( m_staticText19, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_choice_portChoices[] = { _("64502"), _("63502"), _("62502"), _("61502"), _("60502"), _("59502"), _("58502"), _("57502"), _("56502"), _("55502"), _("54502"), _("53502"), _("52502"), _("51502"), _("50502"), _("502"), wxEmptyString, wxEmptyString, wxEmptyString };
	int m_choice_portNChoices = sizeof( m_choice_portChoices ) / sizeof( wxString );
	m_choice_port = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_portNChoices, m_choice_portChoices, 0 );
	m_choice_port->SetSelection( 16 );
	bSizer22->Add( m_choice_port, 1, wxALL|wxEXPAND, 5 );


	bSizer20->Add( bSizer22, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText18 = new wxStaticText( this, wxID_ANY, _("IPV4:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	bSizer21->Add( m_staticText18, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_button_ipv4_gateway_start_stop = new wxButton( this, wxID_ANY, _("启动"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_button_ipv4_gateway_start_stop, 1, wxALL|wxEXPAND, 5 );


	bSizer20->Add( bSizer21, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText20 = new wxStaticText( this, wxID_ANY, _("IPV6:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	bSizer23->Add( m_staticText20, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_button_ipv6_gateway_start_stop = new wxButton( this, wxID_ANY, _("启动"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( m_button_ipv6_gateway_start_stop, 1, wxALL|wxEXPAND, 5 );


	bSizer20->Add( bSizer23, 1, wxEXPAND, 5 );


	bSizer19->Add( bSizer20, 0, 0, 5 );


	this->SetSizer( bSizer19 );
	this->Layout();
	m_timer_simulator_update.SetOwner( this, m_timer_simulator_update.GetId() );
	m_timer_simulator_update.Start( 50 );


	// Connect Events
	m_grid_simulator_data->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( modbussessiontcpgatewaysimulatorbase::OnGridCellChange_Simulator_Data ), NULL, this );
	m_button_ipv4_gateway_start_stop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpgatewaysimulatorbase::OnButtonClick_IPV4_Gateway_Start_Stop ), NULL, this );
	m_button_ipv6_gateway_start_stop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( modbussessiontcpgatewaysimulatorbase::OnButtonClick_IPV6_Gateway_Start_Stop ), NULL, this );
	this->Connect( m_timer_simulator_update.GetId(), wxEVT_TIMER, wxTimerEventHandler( modbussessiontcpgatewaysimulatorbase::OnTimer_Simulator_Update ) );
}

modbussessiontcpgatewaysimulatorbase::~modbussessiontcpgatewaysimulatorbase()
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
