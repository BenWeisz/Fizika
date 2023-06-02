#pragma once

#include <vector>
#include <algorithm>

#include "util/XML.hpp"
#include "util/types.hpp"

namespace DataBuild {

enum GeometryMode {
    STATIC,
    DYNAMIC
};

enum ShaderType {
    VERTEX,
    FRAGMENT
};

struct DataBuildState {
    std::string modelFilePath;
    tinyxml2::XMLDocument* modelFileDoc;

    // Geometry State
    std::string geometryFilePath;
    GeometryMode geometryMode;

    // Material State
    std::string materialFilePath;
    std::vector<std::pair<std::string, ShaderType>> materialShaders;
    std::vector<std::pair<std::string, u32>> materialAttributes;
};

};  // namespace DataBuild