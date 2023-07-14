#pragma once

#include <string>

#include "tinyxml2.h"

inline std::string XML_SafeXMLAttribute(const std::string& name, const tinyxml2::XMLElement* element) {
    const char* attribStr = element->Attribute(name.c_str());
    if (attribStr == 0)
        return std::string("");

    return std::string(attribStr);
}