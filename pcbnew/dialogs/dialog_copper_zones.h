/* dialog_copper_zones.h */

#ifndef DIALOG_COPPER_ZONES
#define DIALOG_COPPER_ZONES

#include "dialog_copper_zones_base.h"

/* here is the derivated class from dialog_copper_zone_frame created by wxFormBuilder
 */
class dialog_copper_zone : public dialog_copper_zone_base
{
private:
    WinEDA_PcbFrame* m_Parent;
    wxConfig*        m_Config;      // Current config
    int m_OnExitCode;               /* exit code: ZONE_ABORT if no change,
                                     *  ZONE_OK if new values accepted
                                     *  ZONE_EXPORT_VALUES if values are exported to others zones
                                     */

    ZONE_SETTING*   m_Zone_Setting;
    bool            m_NetSortingByPadCount;   /* false = alphabetic sort.
                                               *  true = pad count sort.
                                               */
    long            m_NetFiltering;
    int             m_LayerId[LAYER_COUNT]; // Handle the real layer number from layer name position in m_LayerSelectionCtrl
    static wxString m_netNameShowFilter;    /* the filter to show nets (default * "*").
                                             *  static to keep this pattern for an entire pcbnew session
                                             */

public:
    dialog_copper_zone( WinEDA_PcbFrame* parent, ZONE_SETTING* zone_setting );
private:
    void initDialog();
    void OnButtonOkClick( wxCommandEvent& event );
    void OnButtonCancelClick( wxCommandEvent& event );
    bool AcceptOptions( bool aPromptForErrors, bool aUseExportableSetupOnly = false );
    void OnNetSortingOptionSelected( wxCommandEvent& event );
    void ExportSetupToOtherCopperZones( wxCommandEvent& event );
    void OnPadsInZoneClick( wxCommandEvent& event );
    void OnRunFiltersButtonClick( wxCommandEvent& event );
    void buildAvailableListOfNets();
    void initListNetsParams();
};
#endif      // #ifndef DIALOG_COPPER_ZONES