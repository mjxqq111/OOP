#include <wx/wx.h>
#include "EditorFrame.h"

// wxWidgets application class
class EditorApp : public wxApp {
public:
    virtual bool OnInit() override {
        EditorFrame* frame = new EditorFrame();
        frame->Show(true);
        return true;
    }
};

// Application entry point
wxIMPLEMENT_APP(EditorApp);