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

	m_menu_virtualmachine = new wxMenu();
	m_menu_rvvm = new wxMenu();
	wxMenuItem* m_menu_rvvmItem = new wxMenuItem( m_menu_virtualmachine, wxID_ANY, _("RVVM"), wxEmptyString, wxITEM_NORMAL, m_menu_rvvm );
	wxMenuItem* m_menuItem_rvvm_generic;
	m_menuItem_rvvm_generic = new wxMenuItem( m_menu_rvvm, wxID_ANY, wxString( _("RISCV通用") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_rvvm->Append( m_menuItem_rvvm_generic );

	m_menu_virtualmachine->Append( m_menu_rvvmItem );

	m_menubar->Append( m_menu_virtualmachine, _("虚拟机") );

	m_menu_help = new wxMenu();
	wxMenuItem* m_menu_help_about;
	m_menu_help_about = new wxMenuItem( m_menu_help, ID_MENU_HELP_ABOUT, wxString( _("关于") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_help->Append( m_menu_help_about );

	m_menubar->Append( m_menu_help, _("帮助") );

	this->SetMenuBar( m_menubar );

	mainpanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( mainpanel, wxAuiPaneInfo() .Center() .CaptionVisible( false ).CloseButton( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).BottomDockable( false ).Floatable( false ).MinSize( wxSize( -1,500 ) ) );

	wxGridSizer* maingSizer;
	maingSizer = new wxGridSizer( 0, 1, 0, 0 );

	mainauinotebook = new wxAuiNotebook( mainpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TAB_MOVE|wxAUI_NB_TAB_SPLIT|wxAUI_NB_TOP );

	maingSizer->Add( mainauinotebook, 1, wxEXPAND | wxALL, 5 );


	mainpanel->SetSizer( maingSizer );
	mainpanel->Layout();
	maingSizer->Fit( mainpanel );
	logspanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	logspanel->SetMinSize( wxSize( 600,100 ) );

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

	logsbSizer1->Add( logsauinotebook, 1, wxALL|wxEXPAND, 5 );


	logspanel->SetSizer( logsbSizer1 );
	logspanel->Layout();
	logsbSizer1->Fit( logspanel );
	m_timer_ms.SetOwner( this, m_timer_ms.GetId() );
	m_timer_ms.Start( 1 );


	m_mgr.Update();
	this->Centre( wxBOTH );

	// Connect Events
	m_menu_file->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnQuit ), this, m_menu_file_quit->GetId());
	m_menu_rvvm->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnMenuSelectionRVVMGeneric ), this, m_menuItem_rvvm_generic->GetId());
	m_menu_help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainframe::OnAbout ), this, m_menu_help_about->GetId());
	this->Connect( m_timer_ms.GetId(), wxEVT_TIMER, wxTimerEventHandler( mainframe::OnMSTimer ) );
}

mainframe::~mainframe()
{
	// Disconnect Events
	this->Disconnect( m_timer_ms.GetId(), wxEVT_TIMER, wxTimerEventHandler( mainframe::OnMSTimer ) );

	m_mgr.UnInit();

}

rvvmgenericbase::rvvmgenericbase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_listbook_rvvm_generic = new wxListbook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_LEFT );
	m_panel_rvvm_generic_settings = new wxPanel( m_listbook_rvvm_generic, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );


	bSizer7->Add( 50, 0, 0, 0, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );


	bSizer4->Add( 0, 50, 0, 0, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1 = new wxStaticText( m_panel_rvvm_generic_settings, wxID_ANY, _("CPU SMP:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_staticText1->Wrap( -1 );
	m_staticText1->SetMinSize( wxSize( 100,-1 ) );

	bSizer6->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_choice_rvvm_generic_smpChoices[] = { _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("16"), _("32") };
	int m_choice_rvvm_generic_smpNChoices = sizeof( m_choice_rvvm_generic_smpChoices ) / sizeof( wxString );
	m_choice_rvvm_generic_smp = new wxChoice( m_panel_rvvm_generic_settings, wxID_ANY, wxDefaultPosition, wxSize( 100,-1 ), m_choice_rvvm_generic_smpNChoices, m_choice_rvvm_generic_smpChoices, 0 );
	m_choice_rvvm_generic_smp->SetSelection( 0 );
	bSizer6->Add( m_choice_rvvm_generic_smp, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer4->Add( bSizer6, 0, 0, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText2 = new wxStaticText( m_panel_rvvm_generic_settings, wxID_ANY, _("内存(MiB):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	m_staticText2->SetMinSize( wxSize( 100,-1 ) );

	bSizer9->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_choice_rvvm_generic_memChoices[] = { _("256"), _("512"), _("1024"), _("2048"), _("4096"), _("8192"), _("16384"), _("32768") };
	int m_choice_rvvm_generic_memNChoices = sizeof( m_choice_rvvm_generic_memChoices ) / sizeof( wxString );
	m_choice_rvvm_generic_mem = new wxChoice( m_panel_rvvm_generic_settings, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_rvvm_generic_memNChoices, m_choice_rvvm_generic_memChoices, 0 );
	m_choice_rvvm_generic_mem->SetSelection( 3 );
	m_choice_rvvm_generic_mem->SetMinSize( wxSize( 100,-1 ) );

	bSizer9->Add( m_choice_rvvm_generic_mem, 0, wxALL, 5 );


	bSizer4->Add( bSizer9, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText4 = new wxStaticText( m_panel_rvvm_generic_settings, wxID_ANY, _("固件（BIOS):"), wxDefaultPosition, wxSize( 100,-1 ), 0 );
	m_staticText4->Wrap( -1 );
	bSizer11->Add( m_staticText4, 0, wxALL, 5 );

	m_filePicker_rvvm_firmware_image = new wxFilePickerCtrl( m_panel_rvvm_generic_settings, wxID_ANY, wxEmptyString, _("Select a file"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizer11->Add( m_filePicker_rvvm_firmware_image, 1, wxALL, 5 );

	m_hyperlink1 = new wxHyperlinkCtrl( m_panel_rvvm_generic_settings, wxID_ANY, _("固件下载"), wxT("https://github.com/LekKit/RVVM"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer11->Add( m_hyperlink1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer4->Add( bSizer11, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText5 = new wxStaticText( m_panel_rvvm_generic_settings, wxID_ANY, _("内核:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_staticText5->Wrap( -1 );
	m_staticText5->SetMinSize( wxSize( 100,-1 ) );

	bSizer12->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_filePicker_rvvm_generic_kernel = new wxFilePickerCtrl( m_panel_rvvm_generic_settings, wxID_ANY, wxEmptyString, _("Select a file"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizer12->Add( m_filePicker_rvvm_generic_kernel, 1, wxALL, 5 );


	bSizer4->Add( bSizer12, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText6 = new wxStaticText( m_panel_rvvm_generic_settings, wxID_ANY, _("内核命令行:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	m_staticText6->SetMinSize( wxSize( 100,-1 ) );

	bSizer13->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_rvvm_generic_kernel_cmdline = new wxTextCtrl( m_panel_rvvm_generic_settings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_textCtrl_rvvm_generic_kernel_cmdline, 1, wxALL, 5 );

	m_checkBox_rvvm_generic_kernel_cmdline_default = new wxCheckBox( m_panel_rvvm_generic_settings, wxID_ANY, _("追加默认值"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox_rvvm_generic_kernel_cmdline_default->SetValue(true);
	bSizer13->Add( m_checkBox_rvvm_generic_kernel_cmdline_default, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer4->Add( bSizer13, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText3 = new wxStaticText( m_panel_rvvm_generic_settings, wxID_ANY, _("磁盘:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetMinSize( wxSize( 100,-1 ) );

	bSizer10->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_filePicker_rvvm_generic_disk_image = new wxFilePickerCtrl( m_panel_rvvm_generic_settings, wxID_ANY, wxEmptyString, _("Select a file"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizer10->Add( m_filePicker_rvvm_generic_disk_image, 1, wxALL, 5 );

	m_checkBox_rvvm_generic_disk_ata = new wxCheckBox( m_panel_rvvm_generic_settings, wxID_ANY, _("ATA"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_checkBox_rvvm_generic_disk_ata, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer4->Add( bSizer10, 0, wxEXPAND, 5 );


	bSizer7->Add( bSizer4, 1, wxEXPAND, 5 );


	m_panel_rvvm_generic_settings->SetSizer( bSizer7 );
	m_panel_rvvm_generic_settings->Layout();
	bSizer7->Fit( m_panel_rvvm_generic_settings );
	m_listbook_rvvm_generic->AddPage( m_panel_rvvm_generic_settings, _("基本"), true );
	m_panel_rvvm_generic_serialport = new wxPanel( m_listbook_rvvm_generic, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxVERTICAL );

	m_auinotebook_rvvm_generic_serialport = new wxAuiNotebook( m_panel_rvvm_generic_serialport, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_BOTTOM|wxAUI_NB_TAB_MOVE );
	m_panel_rvvm_generic_serialport0 = new wxPanel( m_auinotebook_rvvm_generic_serialport, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxVERTICAL );

	m_richText_rvvm_generic_serialport0 = new wxRichTextCtrl( m_panel_rvvm_generic_serialport0, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	bSizer111->Add( m_richText_rvvm_generic_serialport0, 1, wxEXPAND | wxALL, 5 );


	m_panel_rvvm_generic_serialport0->SetSizer( bSizer111 );
	m_panel_rvvm_generic_serialport0->Layout();
	bSizer111->Fit( m_panel_rvvm_generic_serialport0 );
	m_auinotebook_rvvm_generic_serialport->AddPage( m_panel_rvvm_generic_serialport0, _("串口0"), false, wxNullBitmap );

	bSizer101->Add( m_auinotebook_rvvm_generic_serialport, 1, wxEXPAND | wxALL, 5 );


	m_panel_rvvm_generic_serialport->SetSizer( bSizer101 );
	m_panel_rvvm_generic_serialport->Layout();
	bSizer101->Fit( m_panel_rvvm_generic_serialport );
	m_listbook_rvvm_generic->AddPage( m_panel_rvvm_generic_serialport, _("串口"), false );
	#ifdef __WXGTK__ // Small icon style not supported in GTK
	wxListView* m_listbook_rvvm_genericListView = m_listbook_rvvm_generic->GetListView();
	long m_listbook_rvvm_genericFlags = m_listbook_rvvm_genericListView->GetWindowStyleFlag();
	if( m_listbook_rvvm_genericFlags & wxLC_SMALL_ICON )
	{
		m_listbook_rvvm_genericFlags = ( m_listbook_rvvm_genericFlags & ~wxLC_SMALL_ICON ) | wxLC_ICON;
	}
	m_listbook_rvvm_genericListView->SetWindowStyleFlag( m_listbook_rvvm_genericFlags );
	#endif

	bSizer2->Add( m_listbook_rvvm_generic, 1, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_button_rvvm_generic_start = new wxButton( this, wxID_ANY, _("启动"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button_rvvm_generic_start, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_button_rvvm_generic_stop = new wxButton( this, wxID_ANY, _(" 停止"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button_rvvm_generic_stop->Enable( false );

	bSizer5->Add( m_button_rvvm_generic_stop, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_button_rvvm_generic_quit = new wxButton( this, wxID_ANY, _(" 退出虚拟机"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button_rvvm_generic_quit, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer2->Add( bSizer5, 0, 0, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();
	m_timer_rvvm_ms_timer.SetOwner( this, m_timer_rvvm_ms_timer.GetId() );
	m_timer_rvvm_ms_timer.Start( 10 );


	// Connect Events
	m_richText_rvvm_generic_serialport0->Connect( wxEVT_CHAR, wxKeyEventHandler( rvvmgenericbase::OnChar_RVVM_Generic_Serialport0 ), NULL, this );
	m_richText_rvvm_generic_serialport0->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( rvvmgenericbase::OnKeyDown_RVVM_Generic_Serialport0 ), NULL, this );
	m_button_rvvm_generic_start->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rvvmgenericbase::OnButtonClick_RVVM_Generic_Start ), NULL, this );
	m_button_rvvm_generic_stop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rvvmgenericbase::OnButtonClick_RVVM_Generic_Stop ), NULL, this );
	m_button_rvvm_generic_quit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rvvmgenericbase::OnButtonClick_RVVM_Generic_Quit ), NULL, this );
	this->Connect( m_timer_rvvm_ms_timer.GetId(), wxEVT_TIMER, wxTimerEventHandler( rvvmgenericbase::OnTimer_RVVM_MS_Timer ) );
}

rvvmgenericbase::~rvvmgenericbase()
{
	// Disconnect Events
	m_richText_rvvm_generic_serialport0->Disconnect( wxEVT_CHAR, wxKeyEventHandler( rvvmgenericbase::OnChar_RVVM_Generic_Serialport0 ), NULL, this );
	m_richText_rvvm_generic_serialport0->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( rvvmgenericbase::OnKeyDown_RVVM_Generic_Serialport0 ), NULL, this );
	m_button_rvvm_generic_start->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rvvmgenericbase::OnButtonClick_RVVM_Generic_Start ), NULL, this );
	m_button_rvvm_generic_stop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rvvmgenericbase::OnButtonClick_RVVM_Generic_Stop ), NULL, this );
	m_button_rvvm_generic_quit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rvvmgenericbase::OnButtonClick_RVVM_Generic_Quit ), NULL, this );
	this->Disconnect( m_timer_rvvm_ms_timer.GetId(), wxEVT_TIMER, wxTimerEventHandler( rvvmgenericbase::OnTimer_RVVM_MS_Timer ) );

}
