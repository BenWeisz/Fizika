#include "databuild/DataBuild.hpp"

namespace DataBuild {

DataBuild::DataBuild(const std::string& modelFilePath, bool& initSuccess) : mState(nullptr) {
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
    if (rootNode == nullptr) {
        std::cout << "ERROR: Failed to find Model tag in target \"" << modelFilePath << "\"" << std::endl;
        return false;
    }

    // Load Model file path and mode attribute
    tinyxml2::XMLElement* geometryNode = rootNode->FirstChildElement("Geometry");
    if (geometryNode == nullptr) {
        std::cout << "ERROR: Failed to find Geometry tag in target \"" << modelFilePath << "\"" << std::endl;
        return false;
    }

    mState->geometryFilePath = SafeXMLAttribute("path", geometryNode);
    if (mState->geometryFilePath.empty()) {
        std::cout << "ERROR: Geometry tag does not specify valid attribute \"path\" for target \"" << modelFilePath << "\"" << std::endl;
        return false;
    }

    if (!std::filesystem::exists(mState->geometryFilePath)) {
        std::cout << "ERROR: Geometry tag's \"path\" attribute specifies a file which does not exist: \"" << mState->geometryFilePath << "\"" << std::endl;
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
        std::cout << "ERROR: Failed to find Material tag in target \"" << modelFilePath << "\"" << std::endl;
        return false;
    }

    mState->materialFilePath = SafeXMLAttribute("path", materialNode);
    if (mState->materialFilePath.empty()) {
        std::cout << "ERROR: Material tag does not specify valid attribute \"path\" for target \"" << modelFilePath << "\"" << std::endl;
        return false;
    }

    if (!std::filesystem::exists(mState->materialFilePath)) {
        std::cout << "ERROR: Material tag's \"path\" attribute specifies a file which does not exist: \"" << mState->materialFilePath << "\"" << std::endl;
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
    if (!mState->materialFilePath.ends_with(".mat")) {
        std::cout << "ERROR: Material file must have extension \".mat\"" << std::endl;
        return false;
    }

    // Load the material file
    tinyxml2::XMLDocument materialDoc;
    materialDoc.LoadFile(mState->materialFilePath.c_str());

    if (materialDoc.Error()) {
        std::cout << "ERROR: Failed to load material file " << mState->materialFilePath << std::endl;
        materialDoc.PrintError();
        return false;
    }

    // Set up the root node
    tinyxml2::XMLElement* rootNode = materialDoc.RootElement();
    if (rootNode == nullptr) {
        std::cout << "ERROR: Failed to find Material tag in material file \"" << mState->materialFilePath << "\"" << std::endl;
        return false;
    }

    // Load the shader tag
    tinyxml2::XMLElement* shaderNode = rootNode->FirstChildElement("Shader");
    if (shaderNode == nullptr) {
        std::cout << "ERROR: Failed to find Shader tag in material file \"" << mState->materialFilePath << "\"" << std::endl;
        return false;
    }

    // Load shader source tags
    tinyxml2::XMLElement* element = shaderNode->FirstChildElement("ShaderSource");
    if (element == nullptr) {
        std::cout << "ERROR: Failed to find ShaderSource's in material file \"" << mState->materialFilePath << "\"" << std::endl;
        return false;
    }

    bool foundVertexShader = false;
    bool foundFragmentShader = false;
    for (; element != nullptr; element = element->NextSiblingElement("ShaderSource")) {
        std::string type = SafeXMLAttribute("type", element);
        if (type.empty() || (type != "VERTEX" && type != "FRAGMENT")) {
            std::cout << "ERROR: ShaderSource tag does not specify valid attribute \"type\" in material file \"" << mState->materialFilePath << "\"" << std::endl;
            std::cout << "-----> \"type\" attribute should specify \"VERTEX\" or \"FRAGMENT\"" << std::endl;
            return false;
        }

        std::string path = SafeXMLAttribute("path", element);
        if (path.empty()) {
            std::cout << "ERROR: ShaderSource tag does not specify valid attribute \"path\" in material file \"" << mState->materialFilePath << "\"" << std::endl;
            return false;
        }

        // Check if file exists
        if (!std::filesystem::exists(path)) {
            std::cout << "ERROR: ShaderSource tag's \"path\" attribute specifies a file which does not exist: \"" << path << "\"" << std::endl;
            return false;
        }

        foundVertexShader = foundVertexShader || type == "VERTEX";
        foundFragmentShader = foundFragmentShader || type == "FRAGMENT";

        // Register the shader source
        ShaderType shaderType;
        if (type == "VERTEX")
            shaderType = ShaderType::VERTEX;
        else if (type == "FRAGMENT")
            shaderType = ShaderType::FRAGMENT;

        mState->materialShaders.emplace_back(path, shaderType);
    }

    // Ensure there are at least one of each of the basic shaders
    if (!foundVertexShader || !foundFragmentShader) {
        std::cout << "ERROR: Failed to find at least one VERTEX and FRAGMENT shader in material file \"" << mState->materialFilePath << "\"" << std::endl;
        return false;
    }

    // Load the Layout tag
    tinyxml2::XMLElement* layoutNode = rootNode->FirstChildElement("Layout");
    if (layoutNode == nullptr) {
        std::cout << "ERROR: Failed to find Layout tag in material file \"" << mState->materialFilePath << "\"" << std::endl;
        return false;
    }

    element = layoutNode->FirstChildElement("LayoutElement");
    if (element == nullptr) {
        std::cout << "ERROR: Failed to find LayoutElement's in material file \"" << mState->materialFilePath << "\"" << std::endl;
        return false;
    }
    for (; element != nullptr; element = element->NextSiblingElement("LayoutElement")) {
        std::string name = SafeXMLAttribute("name", element);
        if (name.empty()) {
            std::cout << "ERROR: LayoutElement tag does not specify valid attribute \"name\" in material file \"" << mState->materialFilePath << "\"" << std::endl;
            return false;
        }

        std::string attribSizeStr = SafeXMLAttribute("attribSize", element);
        if (attribSizeStr.empty()) {
            std::cout << "ERROR: LayoutElement tag does not specify valid attribute \"attribSize\" in material file \"" << mState->materialFilePath << "\"" << std::endl;
            return false;
        }

        u32 attribSize;
        try {
            attribSize = std::stoi(attribSizeStr);
        } catch (std::invalid_argument const& ex) {
            std::cout << "ERROR: LayoutElement tag does not specify valid attribute \"attribSize\" in material file \"" << mState->materialFilePath << "\"" << std::endl;
            return false;
        }

        if (attribSize < 1 || attribSize > 4) {
            std::cout << "ERROR: LayoutElement tag does not specify valid attribute \"attribSize\" in material file \"" << mState->materialFilePath << "\"" << std::endl;
            std::cout << "-----> \"attribSize\" attribute should specify value between 1 and 4" << std::endl;
            return false;
        }

        mState->materialAttributes.emplace_back(name, attribSize);
    }

    return true;
}

bool DataBuild::LoadGeometryFile() {
    return true;
}

};  // namespace DataBuild