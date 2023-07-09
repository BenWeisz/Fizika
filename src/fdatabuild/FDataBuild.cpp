#include "fdatabuild/FDataBuild.hpp"

namespace FDataBuild {

FDataBuild::FDataBuild(const std::string& modelFilePath, bool& initSuccess)
    : mModelFilePath(modelFilePath), mModelFileDoc(nullptr), mPositions(nullptr), mNumPositions(0), mUVs(nullptr), mNumUVs(0), mPositionIndices(nullptr), mUVIndices(nullptr), mNumPrimitives(0), mPrimitiveArity(0), mModifiers(nullptr), mNumModifiers(0) {
    initSuccess = InitFDataBuild();
}

FDataBuild::~FDataBuild() {
    // Clean up the FDataBuildState
    if (mPositions) delete[] mPositions;
    if (mUVs) delete[] mUVs;
    if (mPositionIndices) delete[] mPositionIndices;
    if (mUVIndices) delete[] mUVIndices;
    if (mModifiers) delete[] mModifiers;

    delete mModelFileDoc;
}

bool FDataBuild::InitFDataBuild() {
    // Initialize FDataBuildState struct populating it with
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

    if (!LoadModifiers()) {
        return false;
    }

    return true;
}

bool FDataBuild::LoadMaterialFile() {
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

bool FDataBuild::LoadGeometryFile() {
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

bool FDataBuild::LoadModifiers() {
    // Load the model and modifier tag
    tinyxml2::XMLElement* rootNode = mModelFileDoc->RootElement();
    if (rootNode == nullptr) {
        LogError("Failed to find Model tag in target \"", mModelFilePath, "\"");
        return false;
    }

    // Load Model file path and mode attribute
    tinyxml2::XMLElement* modifiersNode = rootNode->FirstChildElement("Modifiers");
    if (modifiersNode == nullptr) {
        LogError("Failed to find Modifiers tag in target \"", mModelFilePath, "\"");
        return false;
    }

    mNumModifiers = 0;
    for (tinyxml2::XMLElement* element = modifiersNode->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
        std::string tag = element->Name();
        if (tag == "Scale" || tag == "Translate" || tag == "Rotate") {
            mNumModifiers++;
        } else {
            LogWarn("Found unsuported modifier \"", tag, "\"");
        }
    }

    // Allocate memory for the modifiers
    mModifiers = new Modifier[mNumModifiers];

    u32 modNum = 1;
    u32 modIndex = 0;
    for (tinyxml2::XMLElement* element = modifiersNode->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
        std::string tag = element->Name();

        if (tag == "Scale") {
            if (!LoadModifier(ModifierType::SCALE, element, modNum, modIndex))
                return false;
        } else if (tag == "Translate") {
            if (!LoadModifier(ModifierType::TRANSLATE, element, modNum, modIndex))
                return false;
        } else if (tag == "Rotate") {
            if (!LoadModifier(ModifierType::ROTATE, element, modNum, modIndex))
                return false;
        } else {
            // This is like only incremementing modIndex for valid modifiers
            modIndex--;
        }

        modNum++;
        modIndex++;
    }

    return true;
}

bool FDataBuild::LoadModifier(const ModifierType type, const tinyxml2::XMLElement* element, const u32 modNum, const u32 modIndex) {
    f32 v0, v1, v2;

    std::string attrib0;
    std::string attrib1;
    std::string attrib2;

    std::string modifierName = type == ModifierType::SCALE ? "Scale" : (type == ModifierType::TRANSLATE ? "Translate" : "Rotate");

    // Determine the attributes to look for
    if (type == ModifierType::SCALE || type == ModifierType::TRANSLATE) {
        attrib0 = "x";
        attrib1 = "y";
        attrib2 = "z";
    } else if (type == ModifierType::ROTATE) {
        attrib0 = "x1";
        attrib1 = "z";
        attrib2 = "x2";
    }

    std::string v0Str = SafeXMLAttribute(attrib0, element);
    if (v0Str.empty()) {
        LogError("Failed to find attribute \"", attrib0, "\" for modifier: ", modifierName, " (Op #", modNum, ")");
        return false;
    }

    std::string v1Str = SafeXMLAttribute(attrib1, element);
    if (v1Str.empty()) {
        LogError("Failed to find attribute \"", attrib1, "\" for modifier: ", modifierName, " (Op #", modNum, ")");
        return false;
    }

    std::string v2Str = SafeXMLAttribute(attrib2, element);
    if (v2Str.empty()) {
        LogError("Failed to find attribute \"", attrib2, "\" for modifier: ", modifierName, " (Op #", modNum, ")");
        return false;
    }

    // We're not going to handle conversion errors :)
    v0 = std::stof(v0Str);
    v1 = std::stof(v1Str);
    v2 = std::stof(v2Str);

    mModifiers[modIndex].mModifierType = type;
    mModifiers[modIndex].v0 = v0;
    mModifiers[modIndex].v1 = v1;
    mModifiers[modIndex].v2 = v2;

    return true;
}

bool FDataBuild::LoadGeometryPrimitiveType(std::ifstream& geometryFile) {
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

bool FDataBuild::LoadGeometryData(std::ifstream& geometryFile) {
    // Rewind file pointer
    geometryFile.clear();
    geometryFile.seekg(0, std::ios::beg);

    std::string line;
    std::string trashStr;

    // Figure out the arity of the data
    mPrimitiveArity = mPrimitiveType == PrimitiveType::TRIANGLES ? 3 : 2;
    std::string primitiveAttribute = mPrimitiveArity == 3 ? "f " : "l ";

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

    mPositionIndices = new u32[mNumPrimitives * mPrimitiveArity];
    mUVIndices = new u32[mNumPrimitives * mPrimitiveArity];

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
            for (int i = 0; i < mPrimitiveArity; i++) {
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
                mPositionIndices[(primtiveIndex * mPrimitiveArity) + i] = std::stoi(token.substr(startPos, endPos - startPos)) - 1;

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
                    mUVIndices[(primtiveIndex * mPrimitiveArity) + i] = std::stoi(token.substr(startPos, endPos - startPos)) - 1;
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

bool FDataBuild::Run() {
    i32 geometrySegmentStart = -1;
    Modifier* modifier;

    i32 i = 0;
    while (i < mNumModifiers) {
        modifier = &(mModifiers[i]);
        // Bundle up all the geometry based operators
        if (modifier->mModifierType == ModifierType::SCALE ||
            modifier->mModifierType == ModifierType::TRANSLATE ||
            modifier->mModifierType == ModifierType::ROTATE) {
            if (geometrySegmentStart == -1)
                geometrySegmentStart = i;

            i++;
        }
        // If the geometry segment has ended execute it
        else {
            if (geometrySegmentStart != -1) {
                // Execute the geometry segment
                GeomerySegmentModifier geometrySegmentModifier(&(mModifiers[geometrySegmentStart]), i - geometrySegmentStart);
                geometrySegmentModifier.Run(mPositions, mNumPositions);

                // Reset the geometry segment start
                geometrySegmentStart = -1;
            }

            i++;
        }
    }

    // Check if any segments ended
    if (geometrySegmentStart != -1) {
        // Execute the geometry segment
        GeomerySegmentModifier geometrySegmentModifier(&(mModifiers[geometrySegmentStart]), i - geometrySegmentStart);
        geometrySegmentModifier.Run(mPositions, mNumPositions);
    }

    return true;
}

bool FDataBuild::SaveData() const {
    f32* vertexNormals = ComputeNormals();
    for (int i = 0; i < 12; i++)
        std::cout << vertexNormals[i] << " ";
    std::cout << std::endl;

    delete[] vertexNormals;

    // if (mGeometryMode == GeometryMode::STATIC) {
    //     std::vector<u32> indexData;
    //     std::vector<f32> vertexData;

    //     // Create a unique index for position, uv index pairs
    //     std::unordered_map<std::string, u32> indexMap;
    //     u32 currIndex = 0;
    //     for (u32 i = 0; i < mNumPrimitives * mPrimitiveArity, i++) {
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

// Compute the per vertex normals of the model
f32* FDataBuild::ComputeNormals() const {
    // Only triangles are supported
    if (mPrimitiveArity != 3)
        return nullptr;

    f32* vertexNormals = new f32[mNumPositions * 3];
    for (u32 i = 0; i < mNumPositions * 3; i++)
        vertexNormals[i] = 0.0;

    u32* facesPerVertex = new u32[mNumPositions];
    for (u32 i = 0; i < mNumPositions; i++)
        facesPerVertex[i] = 0;

    f32 v0x, v0y, v0z, v1x, v1y, v1z, v2x, v2y, v2z;
    f32 d0x, d0y, d0z, d1x, d1y, d1z;
    f32 nx, ny, nz;
    f32 norm;

    u32 primitiveOffset;
    u32 v0Offset;
    u32 v1Offset;
    u32 v2Offset;
    for (u32 i = 0; i < mNumPrimitives; i++) {
        primitiveOffset = i * 3;
        v0Offset = mPositionIndices[primitiveOffset] * 3;
        v1Offset = mPositionIndices[primitiveOffset + 1] * 3;
        v2Offset = mPositionIndices[primitiveOffset + 2] * 3;

        v0x = mPositions[v0Offset];
        v0y = mPositions[v0Offset + 1];
        v0z = mPositions[v0Offset + 2];

        v1x = mPositions[v1Offset];
        v1y = mPositions[v1Offset + 1];
        v1z = mPositions[v1Offset + 2];

        v2x = mPositions[v2Offset];
        v2y = mPositions[v2Offset + 1];
        v2z = mPositions[v2Offset + 2];

        // Compute the displacement vectors
        d0x = v1x - v0x;
        d0y = v1y - v0y;
        d0z = v1z - v0z;

        d1x = v2x - v0x;
        d1y = v2y - v0y;
        d1z = v2z - v0z;

        // Compute the normalized normal
        nx = (d0y * d1z) - (d0z * d1y);
        ny = (d0z * d1x) - (d0x * d1z);
        nz = (d0x * d1y) - (d0y * d1x);
        norm = sqrt(nx * nx + ny * ny + nz * nz);
        nx /= norm;
        ny /= norm;
        nz /= norm;

        // Add the normal to v0's normals
        vertexNormals[v0Offset] += nx;
        vertexNormals[v0Offset + 1] += ny;
        vertexNormals[v0Offset + 2] += nz;

        // Add the normal to v1's normals
        vertexNormals[v1Offset] += nx;
        vertexNormals[v1Offset + 1] += ny;
        vertexNormals[v1Offset + 2] += nz;

        // Add the normal to v2's normals
        vertexNormals[v2Offset] += nx;
        vertexNormals[v2Offset + 1] += ny;
        vertexNormals[v2Offset + 2] += nz;

        // Increment the faces per vertex counts
        facesPerVertex[mPositionIndices[primitiveOffset]]++;
        facesPerVertex[mPositionIndices[primitiveOffset + 1]]++;
        facesPerVertex[mPositionIndices[primitiveOffset + 2]]++;
    }

    // Second pass to average out normals
    for (u32 i = 0; i < mNumPositions; i++) {
        nx = vertexNormals[(i * 3) + 0] / facesPerVertex[i];
        ny = vertexNormals[(i * 3) + 1] / facesPerVertex[i];
        nz = vertexNormals[(i * 3) + 2] / facesPerVertex[i];

        norm = sqrt(nx * nx + ny * ny + nz * nz);
        nx /= norm;
        ny /= norm;
        nz /= norm;

        vertexNormals[(i * 3) + 0] = nx;
        vertexNormals[(i * 3) + 1] = ny;
        vertexNormals[(i * 3) + 2] = nz;
    }

    // Relinquish faces per vertex count
    delete[] facesPerVertex;

    return vertexNormals;
}

};  // namespace FDataBuild