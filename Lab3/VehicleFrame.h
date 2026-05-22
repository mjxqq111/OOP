#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "VehicleManager.h"

// Control identifiers for buttons
enum {
    ID_ADD_VEHICLE = 1000,
    ID_EDIT_VEHICLE,
    ID_DELETE_VEHICLE,
    ID_SAVE_FILE,
    ID_LOAD_FILE
};

// Main application frame
class VehicleFrame : public wxFrame {
public:
    // Constructor creates the main window and initializes controls
    VehicleFrame();

    // Virtual destructor
    virtual ~VehicleFrame();

private:
    // GUI event handlers
    void OnAddVehicle(wxCommandEvent& event);   // For adding a vehicle
    void OnEditVehicle(wxCommandEvent& event);  // For editing a vehicle
    void OnDeleteVehicle(wxCommandEvent& event);    // For deleting a vehicle
    void OnSaveToFile(wxCommandEvent& event);   // For saving vehicles to file (serialization)
    void OnLoadFromFile(wxCommandEvent& event); // For loading vehicles from files (deserilization)
    void OnSelectVehicle(wxListEvent& event);   // For selecting a vehicle

    // Updates the list display from the manager
    void RefreshVehicleList();

    // Shows a dialog for selecting vehicle type
    wxString ShowTypeSelectionDialog();

    // Shows a dialog for editing vehicle properties
    bool ShowEditDialog(const wxString& typeName, Vehicle* vehicle);

    // Vehicle manager instance
    VehicleManager manager;

    // GUI controls
    wxListCtrl* vehicleList;
    wxButton* addButton;
    wxButton* editButton;
    wxButton* deleteButton;
    wxButton* saveButton;
    wxButton* loadButton;

    // Currently selected vehicle index
    int selectedIndex;

    // Event table
    wxDECLARE_EVENT_TABLE();
};