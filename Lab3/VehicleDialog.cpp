#include "VehicleDialog.h"
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>

// Event table for the dialog
wxBEGIN_EVENT_TABLE(VehicleDialog, wxDialog)
EVT_BUTTON(wxID_OK, VehicleDialog::OnOK)
EVT_BUTTON(wxID_CANCEL, VehicleDialog::OnCancel)
wxEND_EVENT_TABLE()

// Constructor that takes the vehicle to edit
VehicleDialog::VehicleDialog(wxWindow* parent, const wxString& typeName, Vehicle* vehicle)
    : wxDialog(parent, wxID_ANY, "Edit " + typeName, wxDefaultPosition, wxSize(400, 500)),
    vehicle(vehicle) {

    CreatePropertyControls();
    LoadVehicleData();
}

// Virtual destructor
VehicleDialog::~VehicleDialog() {}

// Creates property controls dynamically based on vehicle type
void VehicleDialog::CreatePropertyControls() {
    mainSizer = new wxBoxSizer(wxVERTICAL);
    gridSizer = new wxFlexGridSizer(2, 10, 10);

    // Get editable properties from the vehicle using virtual method
    auto properties = vehicle->getEditableProperties();

    // Create a control for each property
    for (const auto& prop : properties) {
        // Add property label
        gridSizer->Add(new wxStaticText(this, wxID_ANY, prop.name + ":"), 0, wxALIGN_RIGHT);

        // Create appropriate control based on property type
        if (prop.type == "int") {
            wxSpinCtrl* ctrl = new wxSpinCtrl(this, wxID_ANY, prop.value,
                wxDefaultPosition, wxDefaultSize,
                wxSP_ARROW_KEYS, 0, 1000000, std::stoi(prop.value));
            propertyControls.push_back(ctrl);
            gridSizer->Add(ctrl, 1, wxEXPAND);
            propertyNames.push_back(prop.name);
        }
        else {
            wxTextCtrl* ctrl = new wxTextCtrl(this, wxID_ANY, prop.value);
            propertyControls.push_back(ctrl);
            gridSizer->Add(ctrl, 1, wxEXPAND);
            propertyNames.push_back(prop.name);
        }
    }

    mainSizer->Add(gridSizer, 1, wxALL | wxEXPAND, 20);

    // Create OK and Cancel buttons
    buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* okButton = new wxButton(this, wxID_OK, "OK");
    wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");

    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);
    mainSizer->Fit(this);
}

// Loads current vehicle data into the controls
void VehicleDialog::LoadVehicleData() {
    auto properties = vehicle->getEditableProperties();
    for (size_t i = 0; i < properties.size(); ++i) {
        if (wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(propertyControls[i])) {
            textCtrl->SetValue(properties[i].value);
        }
        else if (wxSpinCtrl* spinCtrl = dynamic_cast<wxSpinCtrl*>(propertyControls[i])) {
            spinCtrl->SetValue(std::stoi(properties[i].value));
        }
    }
}

// Saves control data back to the vehicle
void VehicleDialog::SaveVehicleData() {
    for (size_t i = 0; i < propertyNames.size(); ++i) {
        std::string value;
        if (wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(propertyControls[i])) {
            value = textCtrl->GetValue().ToStdString();
        }
        else if (wxSpinCtrl* spinCtrl = dynamic_cast<wxSpinCtrl*>(propertyControls[i])) {
            value = std::to_string(spinCtrl->GetValue());
        }
        vehicle->setProperty(propertyNames[i], value);
    }
}

// Handles OK button click and saves changes
void VehicleDialog::OnOK(wxCommandEvent& event) {
    SaveVehicleData();
    EndModal(wxID_OK);
}

// Handles Cancel button click and discards changes
void VehicleDialog::OnCancel(wxCommandEvent& event) {
    EndModal(wxID_CANCEL);
}