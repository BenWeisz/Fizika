#pragma once

/*
 *   Controls the whole Data Build process.
 *
 *   Reads Data Build file, processes the model based on the
 *   pipeline operations that it is give and then packages the
 *   vertex and index data into packed byte files.
 *   Generates model.xml file and packs it into a zip file with
 *   binary model data, material file and textures
 */

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "util/types.hpp"
#include "util/strings.hpp"
#include "util/XML.hpp"
#include "util/log.hpp"

namespace DataBuild {

enum GeometryMode {
    STATIC,
    DYNAMIC
};

enum PrimitiveType {
    TRIANGLES,
    LINES
};

enum ShaderType {
    VERTEX,
    FRAGMENT
};

class DataBuild {
   private:
    // Model File State
    std::string mModelFilePath;
    tinyxml2::XMLDocument* mModelFileDoc;

    // Geometry File State
    std::vector<f32> mPositions;
    std::vector<f32> mUVs;

    std::vector<u32> mPositionIndices;
    std::vector<u32> mUVIndices;
    u32 mVertexArity;

    std::string mGeometryFilePath;
    GeometryMode mGeometryMode;
    PrimitiveType mPrimitiveType;

    // Material File State
    std::string mMaterialFilePath;
    std::vector<std::string> mMaterialTexturePaths;
    std::vector<std::pair<std::string, ShaderType>> mMaterialShaders;
    std::vector<std::pair<std::string, u32>> mMaterialAttributes;

   public:
    DataBuild(const std::string& path, bool& initSuccess);
    ~DataBuild();
    bool Run();
    bool SaveData() const;

   private:
    bool InitDataBuild();
    bool LoadMaterialFile();
    bool LoadGeometryFile();
    // bool LoadPipeline();
    bool LoadGeometryPrimitiveType(std::ifstream& geometryFile);
    bool LoadGeometryData(std::ifstream& geometryFile);
};

};  // namespace DataBuild