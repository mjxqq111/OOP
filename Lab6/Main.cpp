#include <wx/wx.h>
#include "EditorFrame.h"

// wxWidgets application class
class EditorApp : public wxApp {
public:

    // Creating and showing main window
    virtual bool OnInit() override {
        EditorFrame* frame = new EditorFrame();
        frame->Show(true);
        return true;
    }
};

// Application entry point
// OLD:
// wxIMPLEMENT_APP(EditorApp);
// NEW:
int main(int argc, char* argv[]) {
    wxApp::SetInstance(new EditorApp());
    wxEntryStart(argc, argv);
    wxTheApp->OnInit();
    wxTheApp->OnRun();
    wxEntryCleanup();
    return 0;
}