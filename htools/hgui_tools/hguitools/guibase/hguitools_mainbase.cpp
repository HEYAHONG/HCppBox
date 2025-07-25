///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-HYH_USED-0-gdf26f269)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "guibase.h"

#include "hguitools_mainbase.h"

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
	dotfontscanpanel = new wxPanel( mainauinotebook, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* dotfontscanbSizer;
	dotfontscanbSizer = new wxBoxSizer( wxVERTICAL );

	dotfontscan_panel_op = new wxPanel( dotfontscanpanel, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	dotfontscan_panel_op->SetMaxSize( wxSize( -1,100 ) );

	wxBoxSizer* dotfontscan_panel_op_bSizer;
	dotfontscan_panel_op_bSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* dotfontscanfontpickerbSizer;
	dotfontscanfontpickerbSizer = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1 = new wxStaticText( dotfontscan_panel_op, wxID_ANY, _("字体："), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText1->Wrap( -1 );
	dotfontscanfontpickerbSizer->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_fontPicker_dotfontscan = new wxFontPickerCtrl( dotfontscan_panel_op, wxID_ANY, wxFont( 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Segoe UI Variable") ), wxDefaultPosition, wxDefaultSize, wxFNTP_USE_TEXTCTRL );
	m_fontPicker_dotfontscan->SetMaxPointSize( 100 );
	dotfontscanfontpickerbSizer->Add( m_fontPicker_dotfontscan, 0, wxALL|wxEXPAND, 5 );


	dotfontscan_panel_op_bSizer->Add( dotfontscanfontpickerbSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* dotfontscanbuttonbSizer;
	dotfontscanbuttonbSizer = new wxBoxSizer( wxHORIZONTAL );

	m_button_start = new wxButton( dotfontscan_panel_op, wxID_ANY, _("开始"), wxDefaultPosition, wxDefaultSize, 0 );
	dotfontscanbuttonbSizer->Add( m_button_start, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	dotfontscan_panel_op_bSizer->Add( dotfontscanbuttonbSizer, 1, wxEXPAND, 5 );


	dotfontscan_panel_op->SetSizer( dotfontscan_panel_op_bSizer );
	dotfontscan_panel_op->Layout();
	dotfontscan_panel_op_bSizer->Fit( dotfontscan_panel_op );
	dotfontscanbSizer->Add( dotfontscan_panel_op, 1, wxALL, 5 );

	dotfontscan_scintilla_c_source = new wxStyledTextCtrl( dotfontscanpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );
	dotfontscan_scintilla_c_source->SetUseTabs( true );
	dotfontscan_scintilla_c_source->SetTabWidth( 4 );
	dotfontscan_scintilla_c_source->SetIndent( 4 );
	dotfontscan_scintilla_c_source->SetTabIndents( true );
	dotfontscan_scintilla_c_source->SetBackSpaceUnIndents( true );
	dotfontscan_scintilla_c_source->SetViewEOL( false );
	dotfontscan_scintilla_c_source->SetViewWhiteSpace( false );
	dotfontscan_scintilla_c_source->SetMarginWidth( 2, 0 );
	dotfontscan_scintilla_c_source->SetIndentationGuides( true );
	dotfontscan_scintilla_c_source->SetReadOnly( false );
	dotfontscan_scintilla_c_source->SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
	dotfontscan_scintilla_c_source->SetMarginMask( 1, wxSTC_MASK_FOLDERS );
	dotfontscan_scintilla_c_source->SetMarginWidth( 1, 16);
	dotfontscan_scintilla_c_source->SetMarginSensitive( 1, true );
	dotfontscan_scintilla_c_source->SetProperty( wxT("fold"), wxT("1") );
	dotfontscan_scintilla_c_source->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
	dotfontscan_scintilla_c_source->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	dotfontscan_scintilla_c_source->SetMarginWidth( 0, dotfontscan_scintilla_c_source->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	dotfontscan_scintilla_c_source->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	dotfontscan_scintilla_c_source->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	dotfontscan_scintilla_c_source->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	dotfontscan_scintilla_c_source->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	dotfontscan_scintilla_c_source->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	dotfontscan_scintilla_c_source->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	dotfontscan_scintilla_c_source->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	dotfontscan_scintilla_c_source->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	dotfontscan_scintilla_c_source->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	dotfontscan_scintilla_c_source->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	dotfontscan_scintilla_c_source->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	dotfontscan_scintilla_c_source->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	dotfontscan_scintilla_c_source->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	dotfontscan_scintilla_c_source->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	dotfontscan_scintilla_c_source->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	dotfontscan_scintilla_c_source->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	dotfontscan_scintilla_c_source->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	dotfontscanbSizer->Add( dotfontscan_scintilla_c_source, 1, wxEXPAND | wxALL, 5 );


	dotfontscanpanel->SetSizer( dotfontscanbSizer );
	dotfontscanpanel->Layout();
	dotfontscanbSizer->Fit( dotfontscanpanel );
	mainauinotebook->AddPage( dotfontscanpanel, _("dotfontscan"), false, wxNullBitmap );

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
	logsmaingSizer->Add( m_textCtrl_log, 0, wxALL|wxEXPAND, 5 );


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
	m_fontPicker_dotfontscan->Connect( wxEVT_COMMAND_FONTPICKER_CHANGED, wxFontPickerEventHandler( mainframe::dotfontscan_OnFontChanged ), NULL, this );
	m_button_start->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( mainframe::dotdontscan_start_OnButtonClick ), NULL, this );
	this->Connect( m_timer_ms.GetId(), wxEVT_TIMER, wxTimerEventHandler( mainframe::OnMSTimer ) );
}

mainframe::~mainframe()
{
	// Disconnect Events
	m_fontPicker_dotfontscan->Disconnect( wxEVT_COMMAND_FONTPICKER_CHANGED, wxFontPickerEventHandler( mainframe::dotfontscan_OnFontChanged ), NULL, this );
	m_button_start->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( mainframe::dotdontscan_start_OnButtonClick ), NULL, this );
	this->Disconnect( m_timer_ms.GetId(), wxEVT_TIMER, wxTimerEventHandler( mainframe::OnMSTimer ) );

	m_mgr.UnInit();

}
