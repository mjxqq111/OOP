#pragma once

#include <wx/wx.h>
#include <vector>
#include <memory>
#include "Figure.hpp"

class XMLSerializer {
public:
    // Serialize figures to XML string
    static wxString serialize(const std::vector<std::shared_ptr<fig::Figure>>& figures);

    // Deserialize XML string to figures
    static bool deserialize(const wxString& xmlContent,
        std::vector<std::shared_ptr<fig::Figure>>& outFigures);
};