///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-HYH_USED-0-gdf26f269)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/timer.h>
#include <wx/aui/auibook.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/grid.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class mainframe
///////////////////////////////////////////////////////////////////////////////
class mainframe : public wxFrame
{
	private:

	protected:
		wxStatusBar* m_mainframe_statusBar;
		wxMenuBar* m_mainframe_menubar;
		wxMenu* m_menu_file;
		wxMenu* m_menu_log;
		wxMenu* m_menu_session;
		wxMenu* m_menu_help;
		wxTimer m_ms_timer;
		wxPanel* m_main_panel;
		wxAuiNotebook* m_main_auinotebook;

		// Virtual event handlers, override them in your derived class
		virtual void OnActivate( wxActivateEvent& event ) { event.Skip(); }
		virtual void OnMenuSelection_Menu_Quit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelection_Menu_Logdialog( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelection_New_Modbus_Session_TCP_Client( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMSTimer( wxTimerEvent& event ) { event.Skip(); }


	public:

		mainframe( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("hmodbushelper"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;

		~mainframe();

};

///////////////////////////////////////////////////////////////////////////////
/// Class modbussessiontcpclientbase
///////////////////////////////////////////////////////////////////////////////
class modbussessiontcpclientbase : public wxPanel
{
	private:

	protected:
		wxAuiNotebook* m_auinotebook_tcp_client;
		wxPanel* m_panel_coils;
		wxGrid* m_grid_coils;
		wxTextCtrl* m_textCtrl_coils_log;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrl_coils_addr_base;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrl_coils_addr_length;
		wxButton* m_button_coils_read;
		wxButton* m_button_coils_write;
		wxButton* m_button_coils_write_single;
		wxTextCtrl* m_textCtrl_ip;
		wxButton* m_button_connect_disconnect;

		// Virtual event handlers, override them in your derived class
		virtual void OnSetFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnGridCellChange_Modbus_Coils( wxGridEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Coils_Read( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Coils_Write( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Coils_Write_Single( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Connect_DisConnect( wxCommandEvent& event ) { event.Skip(); }


	public:

		modbussessiontcpclientbase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 750,500 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~modbussessiontcpclientbase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class logdialog
///////////////////////////////////////////////////////////////////////////////
class logdialog : public wxDialog
{
	private:

	protected:

	public:
		wxTextCtrl* m_logdialog_textCtrl;

		logdialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER );

		~logdialog();

};

