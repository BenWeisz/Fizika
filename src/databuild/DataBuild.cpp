#include "databuild/DataBuild.hpp"

DataBuild::DataBuild(const std::string& modelFilePath, bool& initSuccess) {
    initSuccess = InitDataBuild(modelFilePath);
}

DataBuild::~DataBuild() {
    // Clean up the DataBuildState
    delete mState->modelFileDoc;

    delete mState;
}

bool DataBuild::InitDataBuild(const std::string& modelFilePath) {
    // Initialize DataBuildState struct populating it with
    // geometry, material paths, and modelFile xml document

    // Allocate space for DataBuildState
    mState = new DataBuildState;

    // Load modelFile
    mState->modelFilePath = modelFilePath;
    mState->modelFileDoc = new tinyxml2::XMLDocument();
    mState->modelFileDoc->LoadFile(modelFilePath.c_str());

    if (mState->modelFileDoc->Error()) {
        std::cout << "ERROR: Failed to load xml file for target \"" << modelFilePath << "\"" << std::endl;
        mState->modelFileDoc->PrintError();
        return false;
    }

    // Load the geometry and material paths
    tinyxml2::XMLElement* rootNode = mState->modelFileDoc->RootElement();

    // Load Model file path and mode attribute
    tinyxml2::XMLElement* geometryNode = rootNode->FirstChildElement("Geometry");
    if (geometryNode == nullptr) {
        std::cout << "ERROR: Failed to find geometry tag in target \"" << modelFilePath << "\"" << std::endl;
        return false;
    }

    mState->geometryFilePath = SafeXMLAttribute("path", geometryNode);
    if (mState->geometryFilePath.empty()) {
        std::cout << "ERROR: Geometry tag does not specify valid attribute \"path\" for target \"" << modelFilePath << "\"" << std::endl;
        return false;
    }

    std::string geometryModeStr = SafeXMLAttribute("mode", geometryNode);
    if (geometryModeStr == "STATIC")
        mState->geometryMode = GeometryMode::STATIC;
    else if (geometryModeStr == "DYNAMIC")
        mState->geometryMode = GeometryMode::DYNAMIC;
    else {
        std::cout << "ERROR: Geometry tag does not specify valid attribute \"mode\" for target \"" << modelFilePath << "\"" << std::endl;
        std::cout << "-----> \"mode\" attribute should specify \"STATIC\" or \"DYNAMIC\"" << std::endl;
        return false;
    }

    // Load Material file path
    tinyxml2::XMLElement* materialNode = rootNode->FirstChildElement("Material");
    if (materialNode == nullptr) {
        std::cout << "ERROR: Failed to find material tag in target \"" << modelFilePath << "\"" << std::endl;
        return false;
    }

    mState->materialFilePath = SafeXMLAttribute("path", materialNode);
    if (mState->materialFilePath.empty()) {
        std::cout << "ERROR: Material tag does not specify valid attribute \"path\" for target \"" << modelFilePath << "\"" << std::endl;
        return false;
    }

    // Load the material file
    if (!LoadMaterialFile())
        return false;

    // Load the geometry file
    if (!LoadGeometryFile())
        return false;

    return true;
}

bool DataBuild::Run() {
    return true;
}

bool DataBuild::SaveData() const {
    return true;
}

bool DataBuild::LoadMaterialFile() {
    return true;
}

bool DataBuild::LoadGeometryFile() {
    return true;
}