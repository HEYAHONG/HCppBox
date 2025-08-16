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
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
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
		virtual void OnMenuSelection_New_Modbus_Session_TCP_Gateway_Simulator( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelection_Modbus_Protocol( wxCommandEvent& event ) { event.Skip(); }
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
		wxStaticLine* m_staticline10;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrl_coils_addr_base;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrl_coils_addr_length;
		wxButton* m_button_coils_read;
		wxButton* m_button_coils_write;
		wxButton* m_button_coils_write_single;
		wxStaticLine* m_staticline11;
		wxPanel* m_panel_discrete_input;
		wxGrid* m_grid_discrete_input;
		wxTextCtrl* m_textCtrl_discrete_input_log;
		wxStaticLine* m_staticline8;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrl_discrete_input_addr_base;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_textCtrl_discrete_input_addr_length;
		wxButton* m_button_discrete_input_read;
		wxStaticLine* m_staticline9;
		wxPanel* m_panel_holding_registers;
		wxGrid* m_grid_holding_registers;
		wxTextCtrl* m_textCtrl_holding_registers_log;
		wxStaticLine* m_staticline5;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_textCtrl_holding_registers_addr_base;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_textCtrl_holding_registers_addr_length;
		wxButton* m_button_holding_registers_read;
		wxButton* m_button_holding_registers_write;
		wxButton* m_button_holding_registers_write_single;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText11;
		wxTextCtrl* m_textCtrl_holding_registers_read_write_read_addr_base;
		wxStaticText* m_staticText12;
		wxTextCtrl* m_textCtrl_holding_registers_read_write_read_addr_length;
		wxStaticText* m_staticText13;
		wxTextCtrl* m_textCtrl_holding_registers_read_write_write_addr_base;
		wxStaticText* m_staticText14;
		wxTextCtrl* m_textCtrl_holding_registers_read_write_write_addr_length;
		wxButton* m_button_holding_registers_read_write;
		wxStaticLine* m_staticline3;
		wxStaticText* m_staticText17;
		wxTextCtrl* m_textCtrl_holding_registers_mask_write_addr;
		wxStaticText* m_staticText15;
		wxTextCtrl* m_textCtrl_holding_registers_mask_write_and_mask;
		wxStaticText* m_staticText16;
		wxTextCtrl* m_textCtrl_holding_registers_mask_write_or_mask;
		wxButton* m_button_holding_registers_mask_write;
		wxStaticLine* m_staticline4;
		wxPanel* m_panel_input_registers;
		wxGrid* m_grid_input_registers;
		wxTextCtrl* m_textCtrl_input_registers_log;
		wxStaticLine* m_staticline6;
		wxStaticText* m_staticText7;
		wxTextCtrl* m_textCtrl_input_registers_addr_base;
		wxStaticText* m_staticText8;
		wxTextCtrl* m_textCtrl_input_registers_addr_length;
		wxButton* m_button_input_registers_read;
		wxStaticLine* m_staticline7;
		wxTextCtrl* m_textCtrl_ip;
		wxButton* m_button_connect_disconnect;
		wxStaticText* m_staticText9;
		wxTextCtrl* m_textCtrl_salve_addr;
		wxStaticText* m_staticText_tid;
		wxTextCtrl* m_textCtrl_Tid;
		wxCheckBox* m_checkBox_Tid_Inc;

		// Virtual event handlers, override them in your derived class
		virtual void OnSetFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnGridCellChange_Modbus_Coils( wxGridEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Coils_Read( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Coils_Write( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Coils_Write_Single( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Discrete_Input_Read( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGridCellChange_Modbus_Holding_Registers( wxGridEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Holding_Registers_Read( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Holding_Registers_Write( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Holding_Registers_Write_Single( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Holding_Registers_Read_Write( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Holding_Registers_Mask_Write( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Input_Registers_Read( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_Connect_DisConnect( wxCommandEvent& event ) { event.Skip(); }


	public:

		modbussessiontcpclientbase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 750,500 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~modbussessiontcpclientbase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class modbussessiontcpgatewaysimulatorbase
///////////////////////////////////////////////////////////////////////////////
class modbussessiontcpgatewaysimulatorbase : public wxPanel
{
	private:

	protected:
		wxGrid* m_grid_simulator_data;
		wxStaticText* m_staticText21;
		wxCheckBox* m_checkBox_localhost;
		wxStaticText* m_staticText19;
		wxChoice* m_choice_port;
		wxStaticText* m_staticText18;
		wxButton* m_button_ipv4_gateway_start_stop;
		wxStaticText* m_staticText20;
		wxButton* m_button_ipv6_gateway_start_stop;
		wxTimer m_timer_simulator_update;

		// Virtual event handlers, override them in your derived class
		virtual void OnGridCellChange_Simulator_Data( wxGridEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_IPV4_Gateway_Start_Stop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick_IPV6_Gateway_Start_Stop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTimer_Simulator_Update( wxTimerEvent& event ) { event.Skip(); }


	public:

		modbussessiontcpgatewaysimulatorbase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 750,500 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~modbussessiontcpgatewaysimulatorbase();

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

