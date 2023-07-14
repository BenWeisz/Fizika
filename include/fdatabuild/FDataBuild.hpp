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
#include <cmath>

#include "FDataBuildState.hpp"
#include "modifiers/GeomerySegmentModifier.hpp"

#include "util/types.hpp"
#include "util/strings.hpp"
#include "util/XML.hpp"
#include "util/log.hpp"
#include "util/math.hpp"

namespace FDataBuild {

class FDataBuild {
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
    u32 mPrimitiveArity;

    std::string mGeometryFilePath;
    GeometryMode mGeometryMode;
    PrimitiveType mPrimitiveType;

    // Material File State
    std::string mMaterialFilePath;
    std::vector<std::string> mMaterialTexturePaths;
    std::vector<std::pair<std::string, ShaderType>> mMaterialShaders;
    std::vector<std::pair<std::string, u32>> mMaterialAttributes;

    // Modifiers State
    Modifier* mModifiers;
    u32 mNumModifiers;

    // Output state
    bool mShouldOuputUVs;
    bool mShouldOuputNormals;

    u32 mNumOuputVertices;

   public:
    FDataBuild(const std::string& path, bool& initSuccess);
    ~FDataBuild();
    bool Run();
    bool SaveData();

   private:
    bool InitFDataBuild();
    bool LoadMaterialFile();
    bool LoadGeometryFile();
    bool LoadModifiers();
    bool LoadModifier(const ModifierType type, const tinyxml2::XMLElement* element, const u32 modNum, const u32 modIndex);
    bool LoadGeometryPrimitiveType(std::ifstream& geometryFile);
    bool LoadGeometryData(std::ifstream& geometryFile);
    f32* ComputeNormals() const;
};

};  // namespace FDataBuild