#include "databuild/DataBuild.hpp"

namespace DataBuild {

DataBuild::DataBuild(const std::string& modelFilePath, bool& initSuccess)
    : mModelFilePath(modelFilePath), mModelFileDoc(nullptr), mPositions(nullptr), mNumPositions(0), mUVs(nullptr), mNumUVs(0), mPositionIndices(nullptr), mUVIndices(nullptr), mNumPrimitives(0), mVertexArity(0) {
    initSuccess = InitDataBuild();
}

DataBuild::~DataBuild() {
    // Clean up the DataBuildState
    if (mPositions) delete[] mPositions;
    if (mUVs) delete[] mUVs;
    if (mPositionIndices) delete[] mPositionIndices;
    if (mUVIndices) delete[] mUVIndices;

    delete mModelFileDoc;
}

bool DataBuild::InitDataBuild() {
    // Initialize DataBuildState struct populating it with
    // geometry, material paths, and modelFile xml document

    // Load modelFile
    mModelFileDoc = new tinyxml2::XMLDocument();
    mModelFileDoc->LoadFile(mModelFilePath.c_str());

    if (mModelFileDoc->Error()) {
        LogError("Failed to load xml file for target \"", mModelFilePath, "\"");
        mModelFileDoc->PrintError();
        return false;
    }

    // Load the geometry and material paths
    tinyxml2::XMLElement* rootNode = mModelFileDoc->RootElement();
    if (rootNode == nullptr) {
        LogError("Failed to find Model tag in target \"", mModelFilePath, "\"");
        return false;
    }

    // Load Model file path and mode attribute
    tinyxml2::XMLElement* geometryNode = rootNode->FirstChildElement("Geometry");
    if (geometryNode == nullptr) {
        LogError("Failed to find Geometry tag in target \"", mModelFilePath, "\"");
        return false;
    }

    mGeometryFilePath = SafeXMLAttribute("path", geometryNode);
    if (mGeometryFilePath.empty()) {
        LogError("Geometry tag does not specify valid attribute \"path\" for target \"", mModelFilePath, "\"");
        return false;
    }

    if (!std::filesystem::exists(mGeometryFilePath)) {
        LogError("Geometry tag's \"path\" attribute specifies a file which does not exist: \"", mGeometryFilePath, "\"");
        return false;
    }

    std::string geometryModeStr = SafeXMLAttribute("mode", geometryNode);
    if (geometryModeStr == "STATIC")
        mGeometryMode = GeometryMode::STATIC;
    else if (geometryModeStr == "DYNAMIC")
        mGeometryMode = GeometryMode::DYNAMIC;
    else {
        LogError("Geometry tag does not specify valid attribute \"mode\" for target \"", mModelFilePath, "\"");
        std::cout << "-----> \"mode\" attribute should specify \"STATIC\" or \"DYNAMIC\"" << std::endl;
        return false;
    }

    // Load Material file path
    tinyxml2::XMLElement* materialNode = rootNode->FirstChildElement("Material");
    if (materialNode == nullptr) {
        LogError("Failed to find Material tag in target \"", mModelFilePath, "\"");
        return false;
    }

    mMaterialFilePath = SafeXMLAttribute("path", materialNode);
    if (mMaterialFilePath.empty()) {
        LogError("Material tag does not specify valid attribute \"path\" for target \"", mModelFilePath, "\"");
        return false;
    }

    if (!std::filesystem::exists(mMaterialFilePath)) {
        LogError("Material tag's \"path\" attribute specifies a file which does not exist: \"", mMaterialFilePath, "\"");
        return false;
    }

    // Load the material file
    if (!LoadMaterialFile())
        return false;

    // Load the geometry file
    if (!LoadGeometryFile())
        return false;

    if (!LoadPipeline()) {
        return false;
    }

    return true;
}

bool DataBuild::LoadMaterialFile() {
    if (!mMaterialFilePath.ends_with(".mat")) {
        LogError("Material file must have extension \".mat\"");
        return false;
    }

    // Load the material file
    tinyxml2::XMLDocument materialDoc;
    materialDoc.LoadFile(mMaterialFilePath.c_str());

    if (materialDoc.Error()) {
        LogError("Failed to load material file ", mMaterialFilePath);
        materialDoc.PrintError();
        return false;
    }

    // Set up the root node
    tinyxml2::XMLElement* rootNode = materialDoc.RootElement();
    if (rootNode == nullptr) {
        LogError("Failed to find Material tag in material file \"", mMaterialFilePath, "\"");
        return false;
    }

    // Load the shader tag
    tinyxml2::XMLElement* shaderNode = rootNode->FirstChildElement("Shader");
    if (shaderNode == nullptr) {
        LogError("Failed to find Shader tag in material file \"", mMaterialFilePath, "\"");
        return false;
    }

    // Load shader source tags
    tinyxml2::XMLElement* element = shaderNode->FirstChildElement("ShaderSource");
    if (element == nullptr) {
        LogError("Failed to find ShaderSource's in material file \"", mMaterialFilePath, "\"");
        return false;
    }

    bool foundVertexShader = false;
    bool foundFragmentShader = false;
    for (; element != nullptr; element = element->NextSiblingElement("ShaderSource")) {
        std::string type = SafeXMLAttribute("type", element);
        if (type.empty() || (type != "VERTEX" && type != "FRAGMENT")) {
            LogError("ShaderSource tag does not specify valid attribute \"type\" in material file \"", mMaterialFilePath, "\"");
            std::cout << "-----> \"type\" attribute should specify \"VERTEX\" or \"FRAGMENT\"" << std::endl;
            return false;
        }

        std::string path = SafeXMLAttribute("path", element);
        if (path.empty()) {
            LogError("ShaderSource tag does not specify valid attribute \"path\" in material file \"", mMaterialFilePath, "\"");
            return false;
        }

        // Check if file exists
        if (!std::filesystem::exists(path)) {
            LogError("ShaderSource tag's \"path\" attribute specifies a file which does not exist: \"", path, "\"");
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

        mMaterialShaders.emplace_back(path, shaderType);
    }

    // Ensure there are at least one of each of the basic shaders
    if (!foundVertexShader || !foundFragmentShader) {
        LogError("Failed to find at least one VERTEX and FRAGMENT shader in material file \"", mMaterialFilePath, "\"");
        return false;
    }

    // Load the Layout tag
    tinyxml2::XMLElement* layoutNode = rootNode->FirstChildElement("Layout");
    if (layoutNode == nullptr) {
        LogError("Failed to find Layout tag in material file \"", mMaterialFilePath, "\"");
        return false;
    }

    element = layoutNode->FirstChildElement("LayoutElement");
    if (element == nullptr) {
        LogError("Failed to find LayoutElement's in material file \"", mMaterialFilePath, "\"");
        return false;
    }

    for (; element != nullptr; element = element->NextSiblingElement("LayoutElement")) {
        std::string name = SafeXMLAttribute("name", element);
        if (name.empty()) {
            LogError("LayoutElement tag does not specify valid attribute \"name\" in material file \"", mMaterialFilePath, "\"");
            return false;
        }

        std::string attribSizeStr = SafeXMLAttribute("attribSize", element);
        if (attribSizeStr.empty()) {
            LogError("LayoutElement tag does not specify valid attribute \"attribSize\" in material file \"", mMaterialFilePath, "\"");
            return false;
        }

        u32 attribSize;
        try {
            attribSize = std::stoi(attribSizeStr);
        } catch (std::invalid_argument const& ex) {
            LogError("LayoutElement tag does not specify valid attribute \"attribSize\" in material file \"", mMaterialFilePath, "\"");
            return false;
        }

        if (attribSize < 1 || attribSize > 4) {
            LogError("LayoutElement tag does not specify valid attribute \"attribSize\" in material file \"", mMaterialFilePath, "\"");
            std::cout << "-----> \"attribSize\" attribute should specify value between 1 and 4" << std::endl;
            return false;
        }

        mMaterialAttributes.emplace_back(name, attribSize);
    }

    // Load the Textures tag
    tinyxml2::XMLElement* texturesNode = rootNode->FirstChildElement("Textures");
    if (texturesNode == nullptr) {
        LogError("Failed to find Textures tag in material file \"", mMaterialFilePath, "\"");
        return false;
    }

    element = texturesNode->FirstChildElement("Texture");
    for (; element != nullptr; element = element->NextSiblingElement("Texture")) {
        std::string path = SafeXMLAttribute("path", element);
        if (path.empty()) {
            LogError("Texture tag does not specify valid attribute \"path\" in material file \"", mMaterialFilePath, "\"");
            return false;
        }

        if (!std::filesystem::exists(path)) {
            LogError("Texture tag's \"path\" attribute specifies a file which does not exist: \"", path, "\"");
            return false;
        }

        mMaterialTexturePaths.push_back(path);
    }

    return true;
}

bool DataBuild::LoadGeometryFile() {
    if (!mGeometryFilePath.ends_with(".obj")) {
        LogError("Geometry file must have extension \".obj\"");
        return false;
    }

    if (!std::filesystem::exists(mGeometryFilePath)) {
        LogError("Geometry file does not exists ", mGeometryFilePath);
        return false;
    }

    std::ifstream geometryFile(mGeometryFilePath);
    if (geometryFile.fail()) {
        LogError("Failed to open Geometry file: ", mGeometryFilePath);
        geometryFile.close();
        return false;
    }

    // Load the primitive type from the file
    bool success = LoadGeometryPrimitiveType(geometryFile);
    if (!success) {
        geometryFile.close();
        return false;
    }

    // Load the geometry data from the geometry file
    success = LoadGeometryData(geometryFile);
    if (!success) {
        LogError("Failed to load geometry data from file: ", mGeometryFilePath);
        geometryFile.close();
        return false;
    }

    geometryFile.close();
    return true;
}

bool DataBuild::LoadPipeline() {
    // Load the model and pipeline tag
    tinyxml2::XMLElement* rootNode = mModelFileDoc->RootElement();
    if (rootNode == nullptr) {
        LogError("Failed to find Model tag in target \"", mModelFilePath, "\"");
        return false;
    }

    // Load Model file path and mode attribute
    tinyxml2::XMLElement* pipelineNode = rootNode->FirstChildElement("Pipeline");
    if (pipelineNode == nullptr) {
        LogError("Failed to find Pipeline tag in target \"", mModelFilePath, "\"");
        return false;
    }

    int i = 1;
    for (tinyxml2::XMLElement* element = pipelineNode->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
        std::string tag = element->Name();

        if (tag == "Scale") {
            if (!LoadPipelineOperator(PipelineOperatorType::SCALE, element, i))
                return false;
        } else if (tag == "Translate") {
            if (!LoadPipelineOperator(PipelineOperatorType::TRANSLATE, element, i))
                return false;
        } else if (tag == "Rotate") {
            if (!LoadPipelineOperator(PipelineOperatorType::ROTATE, element, i))
                return false;
        } else {
            LogWarn("Found unsuported pipeline operation \"", tag, "\" in pipeline");
        }

        i++;
    }

    return true;
}

bool DataBuild::LoadPipelineOperator(const PipelineOperatorType type, const tinyxml2::XMLElement* element, const int opNum) {
    f32 v0, v1, v2;

    std::string attrib0;
    std::string attrib1;
    std::string attrib2;

    std::string operatorName = type == PipelineOperatorType::SCALE ? "Scale" : (type == PipelineOperatorType::TRANSLATE ? "Translate" : "Rotate");

    // Determine the attributes to look for
    if (type == PipelineOperatorType::SCALE || type == PipelineOperatorType::TRANSLATE) {
        attrib0 = "x";
        attrib1 = "y";
        attrib2 = "z";
    } else if (type == PipelineOperatorType::ROTATE) {
        attrib0 = "x1";
        attrib1 = "z";
        attrib2 = "x2";
    }

    std::string v0Str = SafeXMLAttribute(attrib0, element);
    if (v0Str.empty()) {
        LogError("Failed to find attribute \"", attrib0, "\" for pipeline operator: ", operatorName, " (Op #", opNum, ")");
        return false;
    }

    std::string v1Str = SafeXMLAttribute(attrib1, element);
    if (v1Str.empty()) {
        LogError("Failed to find attribute \"", attrib1, "\" for pipeline operator: ", operatorName, " (Op #", opNum, ")");
        return false;
    }

    std::string v2Str = SafeXMLAttribute(attrib2, element);
    if (v2Str.empty()) {
        LogError("Failed to find attribute \"", attrib2, "\" for pipeline operator: ", operatorName, " (Op #", opNum, ")");
        return false;
    }

    // We're not going to handle conversion errors :)
    v0 = std::stof(v0Str);
    v1 = std::stof(v1Str);
    v2 = std::stof(v2Str);

    mPipelineOperators.push_back({type, v0, v1, v2});
    return true;
}

bool DataBuild::LoadGeometryPrimitiveType(std::ifstream& geometryFile) {
    // Rewind file pointer
    geometryFile.clear();
    geometryFile.seekg(0, std::ios::beg);

    // Helper variables
    std::string line;
    std::string trashStr;

    int foundTriangle = 0;
    int foundLine = 0;
    while (!geometryFile.eof()) {
        std::getline(geometryFile, line);
        if (line.compare(0, 2, "f ") == 0 && foundTriangle != 1)
            foundTriangle = 1;
        else if (line.compare(0, 2, "l ") == 0 && foundLine != 1)
            foundLine = 1;
    }

    if (foundTriangle + foundLine != 1) {
        LogError("The Geometry file: ", mGeometryFilePath, " must contain only one primitive type");
        std::cout << "-----> Specify either only \"l\" or \"f\" primitives" << std::endl;
        return false;
    }

    if (foundTriangle)
        mPrimitiveType = PrimitiveType::TRIANGLES;
    else if (foundLine)
        mPrimitiveType = PrimitiveType::LINES;

    // Rewind file pointer
    geometryFile.clear();
    geometryFile.seekg(0, std::ios::beg);

    return true;
}

bool DataBuild::LoadGeometryData(std::ifstream& geometryFile) {
    // Rewind file pointer
    geometryFile.clear();
    geometryFile.seekg(0, std::ios::beg);

    std::string line;
    std::string trashStr;

    // Figure out the arity of the data
    mVertexArity = mPrimitiveType == PrimitiveType::TRIANGLES ? 3 : 2;
    std::string primitiveAttribute = mVertexArity == 3 ? "f " : "l ";

    // Figure out which of the default attributes we might need to load
    bool loadPositions = false;
    bool loadUVs = false;
    for (const auto& p : mMaterialAttributes) {
        if (p.first == "iPosition" && p.second == 3) {
            loadPositions = true;
        } else if (p.first == "iUV" && p.second == 2) {
            loadUVs = true;
        }
    }

    // Throw error if there are no positions
    if (!loadPositions) {
        LogError("Material: Could not find required default shader attribute \"iPosition\"");
        return false;
    }

    // Calculate the amount of space we're going to need
    while (!geometryFile.eof()) {
        std::getline(geometryFile, line);
        std::istringstream iss(line.c_str());
        if (line.compare(0, 2, "v ") == 0)
            mNumPositions++;
        else if (line.compare(0, 3, "vt ") == 0)
            mNumUVs++;
        else if (line.compare(0, 2, primitiveAttribute) == 0) {
            mNumPrimitives++;
        }
    }

    // Rewind file pointer
    geometryFile.clear();
    geometryFile.seekg(0, std::ios::beg);

    // Allocate enough memory to store the vertex and index data;
    mPositions = new f32[mNumPositions * 3];
    mUVs = new f32[mNumUVs * 2];

    mPositionIndices = new u32[mNumPrimitives * mVertexArity];
    mUVIndices = new u32[mNumPrimitives * mVertexArity];

    u32 lineNumber = 1;
    u32 positionIndex = 0;
    u32 uvIndex = 0;
    u32 primtiveIndex = 0;
    while (!geometryFile.eof()) {
        std::getline(geometryFile, line);
        std::istringstream iss(line.c_str());
        if (line.compare(0, 2, "v ") == 0) {
            iss >> trashStr;
            f32 position;
            for (int i = 0; i < 3; i++) {
                iss >> position;
                mPositions[positionIndex++] = position;
            }
        } else if (line.compare(0, 3, "vt ") == 0 && loadUVs) {
            iss >> trashStr;
            f32 uv;
            for (int i = 0; i < 2; i++) {
                iss >> uv;
                mUVs[uvIndex++] = uv;
            }
        } else if (line.compare(0, 2, primitiveAttribute) == 0) {
            iss >> trashStr;

            std::string token;
            for (int i = 0; i < mVertexArity; i++) {
                iss >> token;

                // Error check for position
                if (token.compare(0, 1, "/") == 0) {
                    LogError("Failed to read geometry file (Line ", lineNumber, "). Token \"", token, "\" does not contain a position vertex for attribute \"iPosition\"");
                    return false;
                }

                // Find start and end of position
                int startPos = Strings_GetStartOfNumber(token, 0);
                if (startPos == -1) {
                    LogError("Failed to read geometry file (Line ", lineNumber, "). Token \"", token, "\" is an invalid vertex");
                    return false;
                }
                int endPos = Strings_GetEndOfNumber(token, startPos);
                mPositionIndices[(primtiveIndex * mVertexArity) + i] = std::stoi(token.substr(startPos, endPos - startPos)) - 1;

                if (loadUVs) {
                    // Find start and end of texture uv
                    startPos = Strings_GetStartOfNumber(token, endPos);
                    if (startPos == -1) {
                        LogError("Failed to read geometry file (Line ", lineNumber, "). Token \"", token, "\" is an invalid vertex in file:");
                        return false;
                    } else if (startPos - endPos > 1) {
                        LogError("Failed to read geometry file (Line ", lineNumber, "). Token \"", token, "\" does not contain a uv vertex for attribute \"iUV\":");
                        return false;
                    }
                    endPos = Strings_GetEndOfNumber(token, startPos);
                    mUVIndices[(primtiveIndex * mVertexArity) + i] = std::stoi(token.substr(startPos, endPos - startPos)) - 1;
                }
            }

            primtiveIndex++;
        }

        lineNumber++;
    }

    // Rewind file pointer
    geometryFile.clear();
    geometryFile.seekg(0, std::ios::beg);

    return true;
}

bool DataBuild::Run() {
    return true;
}

bool DataBuild::SaveData() const {
    // if (mGeometryMode == GeometryMode::STATIC) {
    //     std::vector<u32> indexData;
    //     std::vector<f32> vertexData;

    //     // Create a unique index for position, uv index pairs
    //     std::unordered_map<std::string, u32> indexMap;
    //     u32 currIndex = 0;
    //     for (u32 i = 0; i < mNumPrimitives * mVertexArity, i++) {
    //         u32 positionIndex = mPositionIndices[i];
    //         u32 uvIndex = mUVIndices[i];

    //         std::string key = std::to_string(positionIndex) + "/" + std::to_string(uvIndex);
    //         if (!indexMap.contains(key)) {
    //             // If the key didn't exist yet add it
    //             indexMap[key] = currIndex++;

    //             // Construct the corresponding vertex and add it to the vertex data
    //             // Add positional data
    //             vertexData.push_back(mPositions[(positionIndex * 3)]);
    //             vertexData.push_back(mPositions[(positionIndex * 3) + 1]);
    //             vertexData.push_back(mPositions[(positionIndex * 3) + 2]);

    //             // Add uv data
    //             // We're going to want to optimize this so that if uv isnt requested we dont use it here or even load it
    //         }

    //         // Add the vertex index to the list of indices
    //         indexData.push_back(indexMap[key]);
    //     }
    // }

    return true;
}

};  // namespace DataBuild