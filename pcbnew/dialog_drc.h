/////////////////////////////////////////////////////////////////////////////
// Name:        dialog_drc.h
// Purpose:     
// Author:      jean-pierre Charras
// Modified by: 
// Created:     27/02/2006 20:42:00
// RCS-ID:      
// Copyright:   License GNU
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 27/02/2006 20:42:00

#ifndef _DIALOG_DRC_H_
#define _DIALOG_DRC_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "dialog_drc.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/valgen.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DIALOG 10000
#define ID_TEXTCTRL1 10002
#define ID_CHECKBOX_PAD2PAD 10009
#define ID_CHECKBOX 10008
#define ID_CHECKBOX_TEST_ZONES 10007
#define ID_CHECKBOX_CREATE_FILE 10012
#define ID_DRC_RUN 10003
#define ID_STOP_CONTROL_DRC 10004
#define ID_ERASE_DRC_MARKERS 10005
#define ID_LIST_UNCONNECTED_PADS 10006
#define ID_BUTTON_BROWSE_RPT_FILE 10011
#define ID_TEXTCTRL_GET_RPT_FILENAME 10010
#define ID_TEXTCTRL 10001
#define SYMBOL_WINEDA_DRCFRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_WINEDA_DRCFRAME_TITLE _("DRC Control")
#define SYMBOL_WINEDA_DRCFRAME_IDNAME ID_DIALOG
#define SYMBOL_WINEDA_DRCFRAME_SIZE wxSize(400, 300)
#define SYMBOL_WINEDA_DRCFRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * WinEDA_DrcFrame class declaration
 */

class WinEDA_DrcFrame: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WinEDA_DrcFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WinEDA_DrcFrame( );
    WinEDA_DrcFrame( WinEDA_PcbFrame* parent, wxDC * panelDC,
		wxWindowID id = SYMBOL_WINEDA_DRCFRAME_IDNAME, const wxString& caption = SYMBOL_WINEDA_DRCFRAME_TITLE, const wxPoint& pos = SYMBOL_WINEDA_DRCFRAME_POSITION, const wxSize& size = SYMBOL_WINEDA_DRCFRAME_SIZE, long style = SYMBOL_WINEDA_DRCFRAME_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WINEDA_DRCFRAME_IDNAME, const wxString& caption = SYMBOL_WINEDA_DRCFRAME_TITLE, const wxPoint& pos = SYMBOL_WINEDA_DRCFRAME_POSITION, const wxSize& size = SYMBOL_WINEDA_DRCFRAME_SIZE, long style = SYMBOL_WINEDA_DRCFRAME_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin WinEDA_DrcFrame event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_DIALOG
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DRC_RUN
    void OnDrcRunClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STOP_CONTROL_DRC
    void OnStopControlDrcClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ERASE_DRC_MARKERS
    void OnEraseDrcMarkersClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_LIST_UNCONNECTED_PADS
    void OnListUnconnectedPadsClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CLOSE
    void OnCloseClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_BROWSE_RPT_FILE
    void OnButtonBrowseRptFileClick( wxCommandEvent& event );

////@end WinEDA_DrcFrame event handler declarations

////@begin WinEDA_DrcFrame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WinEDA_DrcFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

	void TestDrc(wxCommandEvent & event);
	void DelDRCMarkers(wxCommandEvent & event);
	void ListUnconnectedPads(wxCommandEvent & event);

////@begin WinEDA_DrcFrame member variables
    wxBoxSizer* m_MainSizer;
    wxBoxSizer* m_CommandSizer;
    wxStaticText* m_ClearenceTitle;
    wxTextCtrl* m_SetClearance;
    wxCheckBox* m_Pad2PadTestCtrl;
    wxCheckBox* m_UnconnectedTestCtrl;
    wxCheckBox* m_ZonesTestCtrl;
    wxCheckBox* m_CreateRptCtrl;
    wxTextCtrl* m_RptFilenameCtrl;
    wxTextCtrl* m_logWindow;
////@end WinEDA_DrcFrame member variables
	WinEDA_PcbFrame * m_Parent;
	wxDC * m_DC;
	int m_UnconnectedCount;
};

#endif
    // _DIALOG_DRC_H_
