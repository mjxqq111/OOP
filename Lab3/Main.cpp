#include <wx/wx.h>
#include "VehicleFrame.h"

// Main application class for the Vehicle Manager GUI
class VehicleApp : public wxApp {
public:
    // Called when the application starts
    // Creates and shows the main window
    virtual bool OnInit() override {
        VehicleFrame* frame = new VehicleFrame();
        frame->Show(true);
        return true;
    }
};

// Macro that generates the main entry point for wxWidgets application
wxIMPLEMENT_APP(VehicleApp);