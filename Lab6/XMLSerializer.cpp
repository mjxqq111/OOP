#include "XMLSerializer.h"
#include "ShapeFactory.h"
#include <wx/xml/xml.h>
#include <wx/textfile.h>
#include <wx/stream.h>

#include <wx/mstream.h>
#include <wx/sstream.h>

// Serializes figures to XML string
wxString XMLSerializer::serialize(const std::vector<std::shared_ptr<fig::Figure>>& figures) {
    wxXmlDocument doc;
    wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, "Canvas");
    doc.SetRoot(root);

    for (const auto& fig : figures) {
        wxXmlNode* figNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Figure");
        figNode->AddAttribute("Type", fig->getTypeName());

        wxXmlNode* startNode = new wxXmlNode(wxXML_ELEMENT_NODE, "Start");
        Point start = fig->getStart();
        startNode->AddAttribute("x", wxString::Format("%.2f", start.x));
        startNode->AddAttribute("y", wxString::Format("%.2f", start.y));
        figNode->AddChild(startNode);

        wxXmlNode* endNode = new wxXmlNode(wxXML_ELEMENT_NODE, "End");
        Point end = fig->getEnd();
        endNode->AddAttribute("x", wxString::Format("%.2f", end.x));
        endNode->AddAttribute("y", wxString::Format("%.2f", end.y));
        figNode->AddChild(endNode);

        root->AddChild(figNode);
    }

    wxStringOutputStream xml;
    if (doc.Save(xml)) {
        return xml.GetString();
    }

    return "";
}

// Deserialize XML string to figures
bool XMLSerializer::deserialize(const wxString& xmlContent,
    std::vector<std::shared_ptr<fig::Figure>>& outFigures) {
    if (xmlContent.IsEmpty()) {
        return false;
    }

    wxStringInputStream stream(xmlContent);
    wxXmlDocument doc;

    if (!doc.Load(stream)) {
        return false;
    }

    wxXmlNode* root = doc.GetRoot();
    if (!root || root->GetName() != "Canvas") {
        return false;
    }

    outFigures.clear();

    wxXmlNode* figNode = root->GetChildren();
    while (figNode) {
        if (figNode->GetName() == "Figure") {
            wxString type = figNode->GetAttribute("Type");

            Point start(0, 0), end(0, 0);

            wxXmlNode* child = figNode->GetChildren();
            while (child) {
                if (child->GetName() == "Start") {
                    child->GetAttribute("x", "0").ToDouble(&start.x);
                    child->GetAttribute("y", "0").ToDouble(&start.y);
                }
                else if (child->GetName() == "End") {
                    child->GetAttribute("x", "0").ToDouble(&end.x);
                    child->GetAttribute("y", "0").ToDouble(&end.y);
                }
                child = child->GetNext();
            }

            // Create figure using factory
            auto figure = ShapeFactory::createShape(type);
            if (figure) {
                figure->setStart(start);
                figure->setEnd(end);
                outFigures.push_back(figure);
            }
        }
        figNode = figNode->GetNext();
    }

    return true;
}