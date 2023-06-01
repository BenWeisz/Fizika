#pragma once

#include "util/XML.hpp"

enum GeometryMode {
    STATIC,
    DYNAMIC
};

struct DataBuildState {
    std::string modelFilePath;
    tinyxml2::XMLDocument* modelFileDoc;

    // Geometry State
    std::string geometryFilePath;
    GeometryMode geometryMode;

    // Material State
    std::string materialFilePath;
};