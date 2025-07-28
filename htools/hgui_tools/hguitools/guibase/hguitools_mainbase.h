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
#include <wx/stattext.h>
#include <wx/fontpicker.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/stc/stc.h>
#include <wx/aui/auibook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/statbmp.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class mainframe
///////////////////////////////////////////////////////////////////////////////
class mainframe : public wxFrame
{
	private:

	protected:
		enum
		{
			ID_MENU_FILE_QUIT = 6000,
			ID_MENU_HELP_ABOUT,
		};

		wxStatusBar* m_statusBar;
		wxMenuBar* m_menubar;
		wxMenu* m_menu_file;
		wxMenu* m_menu_help;
		wxPanel* mainpanel;
		wxAuiNotebook* mainauinotebook;
		wxPanel* dotfontscanpanel;
		wxPanel* dotfontscan_panel_op;
		wxStaticText* m_staticText1;
		wxFontPickerCtrl* m_fontPicker_dotfontscan;
		wxButton* m_button_start;
		wxButton* m_button_save;
		wxStyledTextCtrl* dotfontscan_scintilla_c_source;
		wxPanel* imageresourcegeneratepanel;
		wxStyledTextCtrl* imageresourcegenerate_scintilla_c_source;
		wxButton* imageresourcegenerate_button_load;
		wxButton* imageresourcegenerate_button_save;
		wxPanel* logspanel;
		wxAuiNotebook* logsauinotebook;
		wxPanel* logsmainpanel;
		wxTextCtrl* m_textCtrl_log;
		wxTimer m_timer_ms;

		// Virtual event handlers, override them in your derived class
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void dotfontscan_OnFontChanged( wxFontPickerEvent& event ) { event.Skip(); }
		virtual void dotdontscan_start_OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void dotdontscan_save_OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void imageresourcegenerate_load_OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void imageresourcegenerate_save_OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMSTimer( wxTimerEvent& event ) { event.Skip(); }


	public:

		mainframe( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("hguitools"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;

		~mainframe();

};

///////////////////////////////////////////////////////////////////////////////
/// Class imageresourcegenerate_dialog_preview
///////////////////////////////////////////////////////////////////////////////
class imageresourcegenerate_dialog_preview : public wxDialog
{
	private:

	protected:

	public:
		wxStaticBitmap* imageresourcegenerate_bitmap_preview;

		imageresourcegenerate_dialog_preview( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX );

		~imageresourcegenerate_dialog_preview();

};

