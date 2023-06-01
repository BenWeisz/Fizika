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
#include <vector>
#include <unordered_map>

#include "util/types.hpp"
#include "util/strings.hpp"
#include "util/XML.hpp"
#include "databuild/DataBuildState.hpp"

class DataBuild {
   private:
    DataBuildState* mState;

   public:
    DataBuild(const std::string& path, bool& initSuccess);
    ~DataBuild();
    bool Run();
    bool SaveData() const;

   private:
    bool InitDataBuild(const std::string& modelFilePath);
    bool LoadMaterialFile();
    bool LoadGeometryFile();
    // bool LoadPipeline();
};