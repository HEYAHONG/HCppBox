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
	this->SetMenuBar( m_menubar );

	mainpanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( mainpanel, wxAuiPaneInfo() .Center() .CloseButton( false ).Dock().Resizable().FloatingSize( wxDefaultSize ).BottomDockable( false ).Floatable( false ) );

	wxGridSizer* maingSizer;
	maingSizer = new wxGridSizer( 0, 1, 0, 0 );

	mainauinotebook = new wxAuiNotebook( mainpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TAB_MOVE|wxAUI_NB_TAB_SPLIT|wxAUI_NB_TOP );
	dotfontscanpanel = new wxPanel( mainauinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* dotfontscanbSizer;
	dotfontscanbSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* dotfontscanfontpickerbSizer;
	dotfontscanfontpickerbSizer = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1 = new wxStaticText( dotfontscanpanel, wxID_ANY, _("字体："), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText1->Wrap( -1 );
	dotfontscanfontpickerbSizer->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_fontPicker_dotfontscan = new wxFontPickerCtrl( dotfontscanpanel, wxID_ANY, wxFont( 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Segoe UI Variable") ), wxDefaultPosition, wxDefaultSize, wxFNTP_USEFONT_FOR_LABEL );
	m_fontPicker_dotfontscan->SetMaxPointSize( 100 );
	m_fontPicker_dotfontscan->SetMinSize( wxSize( 400,-1 ) );

	dotfontscanfontpickerbSizer->Add( m_fontPicker_dotfontscan, 0, wxALL|wxEXPAND, 5 );


	dotfontscanfontpickerbSizer->Add( 0, 0, 1, wxEXPAND, 5 );


	dotfontscanbSizer->Add( dotfontscanfontpickerbSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_button_start = new wxButton( dotfontscanpanel, wxID_ANY, _("开始"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button_start, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	dotfontscanbSizer->Add( bSizer5, 1, wxEXPAND, 5 );


	dotfontscanpanel->SetSizer( dotfontscanbSizer );
	dotfontscanpanel->Layout();
	dotfontscanbSizer->Fit( dotfontscanpanel );
	mainauinotebook->AddPage( dotfontscanpanel, _("dotfontscan"), false, wxNullBitmap );

	maingSizer->Add( mainauinotebook, 1, wxEXPAND | wxALL, 5 );


	maingSizer->Add( 0, 0, 1, wxEXPAND, 5 );


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
