#pragma once

#include <wx/wx.h>
#include <vector>
#include <string>
#include "Vehicle.h"

// Dialog for editing vehicle properties
class VehicleDialog : public wxDialog {
public:
    // Constructor that takes the vehicle to edit
    VehicleDialog(wxWindow* parent, const wxString& typeName, Vehicle* vehicle);

    // Virtual destructor
    virtual ~VehicleDialog();

private:
    // Handles OK button click and saves changes
    void OnOK(wxCommandEvent& event);

    // Handles Cancel button click and discards changes
    void OnCancel(wxCommandEvent& event);

    // Creates property controls dynamically based on vehicle type
    void CreatePropertyControls();

    // Loads current vehicle data into the controls
    void LoadVehicleData();

    // Saves control data back to the vehicle
    void SaveVehicleData();

    Vehicle* vehicle;                                    // Pointer to the vehicle being edited
    std::vector<wxWindow*> propertyControls;            // List of control widgets
    std::vector<std::string> propertyNames;             // Names of properties for each control

    wxBoxSizer* mainSizer;                              // Main layout sizer
    wxFlexGridSizer* gridSizer;                         // Grid sizer for property layout
    wxBoxSizer* buttonSizer;                            // Sizer for OK/Cancel buttons

    wxDECLARE_EVENT_TABLE();
};