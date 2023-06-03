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
    std::string mModelFilePath;
    tinyxml2::XMLDocument* mModelFileDoc;

    // Geometry State
    std::string mGeometryFilePath;
    GeometryMode mGeometryMode;

    // Material State
    std::string mMaterialFilePath;
    std::vector<std::pair<std::string, ShaderType>> mMaterialShaders;
    std::vector<std::pair<std::string, u32>> mMaterialAttributes;
    std::vector<std::string> mMaterialTexturePaths;
};

};  // namespace DataBuild