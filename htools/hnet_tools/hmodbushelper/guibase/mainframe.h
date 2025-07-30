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
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
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
		wxMenu* m_menu_help;
		wxTimer m_ms_timer;

		// Virtual event handlers, override them in your derived class
		virtual void OnMenuSelection_Menu_Quit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelection_Menu_Logdialog( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMSTimer( wxTimerEvent& event ) { event.Skip(); }


	public:

		mainframe( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("hmodbushelper"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;

		~mainframe();

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

