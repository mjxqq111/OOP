#include "VehicleFrame.h"
#include "VehicleDialog.h"
#include <wx/msgdlg.h>
#include <wx/filedlg.h>

// Event table definition
wxBEGIN_EVENT_TABLE(VehicleFrame, wxFrame)
EVT_BUTTON(ID_ADD_VEHICLE, VehicleFrame::OnAddVehicle)
EVT_BUTTON(ID_EDIT_VEHICLE, VehicleFrame::OnEditVehicle)
EVT_BUTTON(ID_DELETE_VEHICLE, VehicleFrame::OnDeleteVehicle)
EVT_BUTTON(ID_SAVE_FILE, VehicleFrame::OnSaveToFile)
EVT_BUTTON(ID_LOAD_FILE, VehicleFrame::OnLoadFromFile)
EVT_LIST_ITEM_SELECTED(wxID_ANY, VehicleFrame::OnSelectVehicle)
wxEND_EVENT_TABLE()

// Constructor
VehicleFrame::VehicleFrame()
    : wxFrame(nullptr, wxID_ANY, "Vehicle Manager", wxDefaultPosition, wxSize(800, 600)),
    selectedIndex(-1) {

    // Create a panel to hold all controls
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    // Create the vehicle list control with fixed size
    vehicleList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize(680, 400),
        wxLC_REPORT | wxLC_SINGLE_SEL);

    // Add columns to the list with adjusted widths
    vehicleList->AppendColumn("Index", wxLIST_FORMAT_LEFT, 50);
    vehicleList->AppendColumn("Type", wxLIST_FORMAT_LEFT, 120);
    vehicleList->AppendColumn("Max Speed (km/h)", wxLIST_FORMAT_LEFT, 110);
    vehicleList->AppendColumn("Length (m)", wxLIST_FORMAT_LEFT, 80);
    vehicleList->AppendColumn("Width (m)", wxLIST_FORMAT_LEFT, 80);
    vehicleList->AppendColumn("Height (m)", wxLIST_FORMAT_LEFT, 80);
    vehicleList->AppendColumn("Mass (kg)", wxLIST_FORMAT_LEFT, 90);
    vehicleList->AppendColumn("Capacity", wxLIST_FORMAT_LEFT, 70);

    // Create buttons with unique identifiers
    addButton = new wxButton(panel, ID_ADD_VEHICLE, "Add Vehicle", wxDefaultPosition, wxSize(120, 35));
    editButton = new wxButton(panel, ID_EDIT_VEHICLE, "Edit Vehicle", wxDefaultPosition, wxSize(120, 35));
    deleteButton = new wxButton(panel, ID_DELETE_VEHICLE, "Delete Vehicle", wxDefaultPosition, wxSize(120, 35));
    saveButton = new wxButton(panel, ID_SAVE_FILE, "Save to XML", wxDefaultPosition, wxSize(120, 35));
    loadButton = new wxButton(panel, ID_LOAD_FILE, "Load from XML", wxDefaultPosition, wxSize(120, 35));

    // Disable edit and delete buttons initially
    editButton->Enable(false);
    deleteButton->Enable(false);

    // Create a sizer for layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    // Add controls to sizers without expansion flags
    mainSizer->Add(vehicleList, 0, wxALIGN_CENTER | wxALL, 10);

    buttonSizer->Add(addButton, 0, wxALL, 5);
    buttonSizer->Add(editButton, 0, wxALL, 5);
    buttonSizer->Add(deleteButton, 0, wxALL, 5);
    buttonSizer->Add(saveButton, 0, wxALL, 5);
    buttonSizer->Add(loadButton, 0, wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

    panel->SetSizer(mainSizer);

    // Center the frame on screen
    Centre();

    // Refresh the vehicle list
    RefreshVehicleList();
}

// Destructor
VehicleFrame::~VehicleFrame() {}

// Handles Add Vehicle button click
void VehicleFrame::OnAddVehicle(wxCommandEvent& event) {
    wxString typeName = ShowTypeSelectionDialog();
    if (typeName.IsEmpty()) {
        return;
    }

    // Create a new vehicle of the selected type
    auto newVehicle = Vehicle::create(typeName.ToStdString());
    if (!newVehicle) {
        wxMessageBox("Failed to create vehicle", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Show edit dialog to set properties
    if (ShowEditDialog(typeName, newVehicle.get())) {
        manager.addVehicle(std::move(newVehicle));
        RefreshVehicleList();
    }
}

// Handles Edit Vehicle button click
void VehicleFrame::OnEditVehicle(wxCommandEvent& event) {
    if (selectedIndex < 0 || selectedIndex >= manager.getCount()) {
        wxMessageBox("Please select a vehicle to edit", "No Selection", wxOK | wxICON_WARNING);
        return;
    }

    // Get a clone of the selected vehicle
    auto vehicle = manager.getVehicleCopy(selectedIndex);
    if (!vehicle) {
        wxMessageBox("Failed to get vehicle", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxString typeName(vehicle->getTypeName());

    // Show edit dialog
    if (ShowEditDialog(typeName, vehicle.get())) {
        manager.editVehicle(selectedIndex, std::move(vehicle));
        RefreshVehicleList();
    }
}

// Handles Delete Vehicle button click
void VehicleFrame::OnDeleteVehicle(wxCommandEvent& event) {
    if (selectedIndex < 0 || selectedIndex >= manager.getCount()) {
        wxMessageBox("Please select a vehicle to delete", "No Selection", wxOK | wxICON_WARNING);
        return;
    }

    int result = wxMessageBox("Are you sure you want to delete this vehicle?",
        "Confirm Delete",
        wxYES_NO | wxICON_QUESTION);

    if (result == wxYES) {
        manager.removeVehicle(selectedIndex);
        selectedIndex = -1;
        RefreshVehicleList();
        editButton->Enable(false);
        deleteButton->Enable(false);
    }
}

// Handles Save to File button click
void VehicleFrame::OnSaveToFile(wxCommandEvent& event) {
    wxFileDialog saveDialog(this, "Save XML File", "", "vehicles.xml",
        "XML files (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveDialog.ShowModal() == wxID_OK) {
        std::string filename = saveDialog.GetPath().ToStdString();
        manager.saveToFile(filename);
        wxMessageBox("Vehicles saved successfully", "Success", wxOK | wxICON_INFORMATION);
    }
}

// Handles Load from File button click
void VehicleFrame::OnLoadFromFile(wxCommandEvent& event) {
    wxFileDialog openDialog(this, "Open XML File", "", "",
        "XML files (*.xml)|*.xml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openDialog.ShowModal() == wxID_OK) {
        std::string filename = openDialog.GetPath().ToStdString();
        manager.loadFromFile(filename);
        RefreshVehicleList();
        selectedIndex = -1;
        editButton->Enable(false);
        deleteButton->Enable(false);
    }
}

// Handles vehicle selection in the list
void VehicleFrame::OnSelectVehicle(wxListEvent& event) {
    selectedIndex = event.GetIndex();
    editButton->Enable(true);
    deleteButton->Enable(true);
}

// Updates the list display from the manager
void VehicleFrame::RefreshVehicleList() {
    vehicleList->DeleteAllItems();

    for (int i = 0; i < manager.getCount(); ++i) {
        auto vehicle = manager.getVehicleConst(i);
        if (!vehicle) continue;

        long itemIndex = vehicleList->InsertItem(i, wxString::Format("%d", i));

        vehicleList->SetItem(itemIndex, 1, vehicle->getTypeName());
        vehicleList->SetItem(itemIndex, 2, wxString::Format("%.1f", vehicle->getMaxSpeed()));
        vehicleList->SetItem(itemIndex, 3, wxString::Format("%.2f", vehicle->getLength()));
        vehicleList->SetItem(itemIndex, 4, wxString::Format("%.2f", vehicle->getWidth()));
        vehicleList->SetItem(itemIndex, 5, wxString::Format("%.2f", vehicle->getHeight()));
        vehicleList->SetItem(itemIndex, 6, wxString::Format("%.1f", vehicle->getMass()));
        vehicleList->SetItem(itemIndex, 7, wxString::Format("%d", vehicle->getCapacity()));
    }
}

// Shows a dialog for selecting vehicle type
wxString VehicleFrame::ShowTypeSelectionDialog() {
    auto types = Vehicle::getRegisteredTypes();

    wxArrayString typeArray;
    for (const auto& type : types) {
        typeArray.Add(type);
    }

    wxSingleChoiceDialog dialog(this, "Select vehicle type", "Add Vehicle", typeArray);

    if (dialog.ShowModal() == wxID_OK) {
        return dialog.GetStringSelection();
    }

    return wxEmptyString;
}

// Shows a dialog for editing vehicle properties
bool VehicleFrame::ShowEditDialog(const wxString& typeName, Vehicle* vehicle) {
    VehicleDialog dialog(this, typeName, vehicle);
    return dialog.ShowModal() == wxID_OK;
}