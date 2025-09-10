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
#include <wx/aui/auibook.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/filepicker.h>
#include <wx/hyperlink.h>
#include <wx/checkbox.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/button.h>

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
		wxMenu* m_menu_virtualmachine;
		wxMenu* m_menu_rvvm;
		wxMenu* m_menu_help;
		wxPanel* mainpanel;
		wxAuiNotebook* mainauinotebook;
		wxPanel* logspanel;
		wxAuiNotebook* logsauinotebook;
		wxPanel* logsmainpanel;
		wxTextCtrl* m_textCtrl_log;
		wxTimer m_timer_ms;

		// Virtual event handlers, override them in your derived class
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionRVVMGeneric( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMSTimer( wxTimerEvent& event ) { event.Skip(); }


	public:

		mainframe( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("hsimulatortools"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;

		~mainframe();

};

///////////////////////////////////////////////////////////////////////////////
/// Class rvvmgenericbase
///////////////////////////////////////////////////////////////////////////////
class rvvmgenericbase : public wxPanel
{
	private:

	protected:
		wxListbook* m_listbook_rvvm_generic;
		wxPanel* m_panel_rvvm_generic_settings;
		wxStaticText* m_staticText1;
		wxChoice* m_choice_rvvm_generic_smp;
		wxStaticText* m_staticText2;
		wxChoice* m_choice_rvvm_generic_mem;
		wxStaticText* m_staticText4;
		wxFilePickerCtrl* m_filePicker_rvvm_firmware_image;
		wxHyperlinkCtrl* m_hyperlink1;
		wxStaticText* m_staticText3;
		wxFilePickerCtrl* m_filePicker_rvvm_generic_disk_image;
		wxCheckBox* m_checkBox_rvvm_generic_disk_ata;
		wxPanel* m_panel_rvvm_generic_serialport;
		wxAuiNotebook* m_auinotebook_rvvm_generic_serialport;
		wxPanel* m_panel_rvvm_generic_serialport0;
		wxRichTextCtrl* m_richText_rvvm_generic_serialport0;
		wxButton* m_button_rvvm_generic_start;
		wxButton* m_button_rvvm_generic_stop;
		wxButton* m_button_rvvm_generic_quit;
		wxTimer m_timer_rvvm_ms_timer;

		// Virtual event handlers, override them in your derived class
		virtual void OnKeyDown_RVVM_Generic_Serialport0( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_RVVM_Generic_Start( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_RVVM_Generic_Stop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_RVVM_Generic_Quit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTimer_RVVM_MS_Timer( wxTimerEvent& event ) { event.Skip(); }


	public:

		rvvmgenericbase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 750,500 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~rvvmgenericbase();

};

