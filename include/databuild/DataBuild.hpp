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

#include "DataBuildState.hpp"
#include "Pipeline.hpp"

#include "util/types.hpp"
#include "util/strings.hpp"
#include "util/XML.hpp"
#include "util/log.hpp"

namespace DataBuild {

class DataBuild {
   private:
    // Model File State
    std::string mModelFilePath;
    tinyxml2::XMLDocument* mModelFileDoc;

    // Geometry File State
    f32* mPositions;
    u32 mNumPositions;
    f32* mUVs;
    u32 mNumUVs;

    u32* mPositionIndices;
    u32* mUVIndices;
    u32 mNumPrimitives;
    u32 mVertexArity;

    std::string mGeometryFilePath;
    GeometryMode mGeometryMode;
    PrimitiveType mPrimitiveType;

    // Material File State
    std::string mMaterialFilePath;
    std::vector<std::string> mMaterialTexturePaths;
    std::vector<std::pair<std::string, ShaderType>> mMaterialShaders;
    std::vector<std::pair<std::string, u32>> mMaterialAttributes;

    // Pipeline State
    std::vector<PipelineOperator> mPipelineOperators;

   public:
    DataBuild(const std::string& path, bool& initSuccess);
    ~DataBuild();
    bool Run();
    bool SaveData() const;

   private:
    bool InitDataBuild();
    bool LoadMaterialFile();
    bool LoadGeometryFile();
    bool LoadPipeline();
    bool LoadPipelineOperator(const PipelineOperatorType type, const tinyxml2::XMLElement* element, const int opNum);
    bool LoadGeometryPrimitiveType(std::ifstream& geometryFile);
    bool LoadGeometryData(std::ifstream& geometryFile);
};

};  // namespace DataBuild