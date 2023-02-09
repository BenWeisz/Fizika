#include "../include/graphics/Mesh.hpp"

// Helper string functions
// Find the position of the first character from start that is not a number character
int findEndOfNumber(const std::string& str, const int start) {
    std::istringstream iss(str.c_str());
    char token;

    int i = 0;
    while (!iss.eof()) {
        iss >> token;
        if (i > start) {
            int value = token - '0';
            if (value < 0 || value > 9)
                return i;
        }
        i++;
    }

    return str.length();
}

// Find the first character from start that is a number character
int findStartOfNumber(const std::string& str, const int start) {
    std::istringstream iss(str.c_str());
    char token;

    int i = 0;
    while (!iss.eof()) {
        iss >> token;
        if (i > start) {
            if (token != '/')
                return i;
        }
        i++;
    }

    return str.length();
}

Mesh::Mesh(const std::string& path) : mAttributeSettings(AttributeSettings::LOAD_POSITIONS) {
    // Initialize the mesh
    InitMesh(path);
}

Mesh::Mesh(const std::string& path, const AttributeSettings attribSettings) : mAttributeSettings((AttributeSettings)(AttributeSettings::LOAD_POSITIONS | attribSettings)) {
    // Initialize the mesh
    InitMesh(path);
}

Mesh::~Mesh() {
    delete mVertexArray;
    delete mVertexBuffer;
}

void Mesh::Bind() const {
    mVertexArray->Bind();
}

void Mesh::Unbind() const {
    mVertexArray->Unbind();
}

// Update the buffer based on the changes to the the
// positions, normals and uvs
void Mesh::Update() {
    if (mAttributeSettings & AttributeSettings::COMPUTE_NORMALS_VERTEX) {
        ComputeVertexNormals();
    }

    // Compute the buffer data vector
    auto packedMesh = PackMesh();

    mVertexBuffer->Bind();
    mVertexBuffer->Update(packedMesh.first);
    mVertexBuffer->Unbind();
}

VertexBuffer* Mesh::GetVertexBuffer() const {
    return mVertexBuffer;
}

VertexArray* Mesh::GetVertexArray() const {
    return mVertexArray;
}

IndexBuffer* Mesh::GetIndexBuffer() const {
    return mIndexBuffer;
}

void Mesh::InitMesh(const std::string& path) {
    // Load the mesh from file
    LoadFromFile(path);

    // Post processing
    if (mAttributeSettings & AttributeSettings::COMPUTE_NORMALS_VERTEX) {
        mNormals = Eigen::MatrixXd::Zero(mPositions.rows(), mPositions.cols());
        ComputeVertexNormals();
    }

    // Load the vertex data into the vertex buffer
    auto packedMesh = PackMesh();

    mVertexBuffer = new VertexBuffer(packedMesh.first);
    mIndexBuffer = new IndexBuffer(packedMesh.second);

    // Create the texture
    mVertexArray = new VertexArray();
}

// Operates on a dumbed down version of .obj where the
// position, texture and normal index must match for each vertex
void Mesh::LoadFromFile(const std::string& path) {
    std::ifstream file(path.c_str());

    // Simple error checking
    if (file.fail()) {
        std::cout << "ERROR: Could not load .obj file: " << path << std::endl;
        return;
    }

    // Helper variables
    std::string line;
    std::string trashStr;

    int foundTriangle = 0;
    int foundLine = 0;
    while (!file.eof()) {
        std::getline(file, line);
        if (line.compare(0, 2, "f ") == 0 && foundTriangle != 1)
            foundTriangle = 1;
        else if (line.compare(0, 2, "l ") == 0 && foundLine != 1)
            foundLine = 1;
    }

    if (foundTriangle + foundLine != 1) {
        std::cout << "ERROR: The file: " << path << " must contain only one primitive type" << std::endl;
        return;
    }

    int primitiveDegree;
    if (foundTriangle) {
        primitiveDegree = 3;
        mPrimitiveType = PrimitiveType::TRIANGLES;
    } else if (foundLine) {
        primitiveDegree = 2;
        mPrimitiveType = PrimitiveType::LINES;
    }

    // Rewind file pointer
    file.clear();
    file.seekg(0, std::ios::beg);

    // Count the number of occurances of each token
    int numPositions = 0;
    int numNormals = 0;
    int numTextureUVs = 0;
    int numPrimitives = 0;
    while (!file.eof()) {
        std::getline(file, line);
        if (line.compare(0, 2, "v ") == 0)
            numPositions++;
        else if (line.compare(0, 3, "vn ") == 0)
            numNormals++;
        else if (line.compare(0, 3, "vt ") == 0)
            numTextureUVs++;
        else if (line.compare(0, 2, "f ") == 0)
            numPrimitives++;
        else if (line.compare(0, 2, "l ") == 0)
            numPrimitives++;
    }

    // Rewind file pointer
    file.clear();
    file.seekg(0, std::ios::beg);

    // Initialize the mesh data structure
    mPositions = Eigen::MatrixXd::Zero(numPositions, 3);
    mNormals = Eigen::MatrixXd::Zero(numNormals, 3);
    mTextureUVs = Eigen::MatrixXd::Zero(numTextureUVs, 2);

    mPrimitives = Eigen::MatrixXi::Zero(numPrimitives, primitiveDegree);
    mNormalPrimitives = Eigen::MatrixXi::Zero(numPrimitives, primitiveDegree);
    mTexturePrimitives = Eigen::MatrixXi::Zero(numPrimitives, primitiveDegree);

    // Populate the matrices
    int iPosition = 0;
    int iNormal = 0;
    int iTextureUV = 0;
    int iPrimitive = 0;
    while (!file.eof()) {
        std::getline(file, line);
        std::istringstream iss(line.c_str());
        if (line.compare(0, 2, "v ") == 0) {
            iss >> trashStr;
            iss >> mPositions(iPosition, 0);
            iss >> mPositions(iPosition, 1);
            iss >> mPositions(iPosition, 2);
            iPosition++;
        } else if (line.compare(0, 3, "vn ") == 0) {
            iss >> trashStr;
            iss >> mNormals(iNormal, 0);
            iss >> mNormals(iNormal, 1);
            iss >> mNormals(iNormal, 2);
            iNormal++;
        } else if (line.compare(0, 3, "vt ") == 0) {
            iss >> trashStr;
            iss >> mTextureUVs(iTextureUV, 0);
            iss >> mTextureUVs(iTextureUV, 1);
            iTextureUV++;
        } else if (line.compare(0, 2, "f ") == 0) {
            iss >> trashStr;

            std::string token;
            for (int i = 0; i < 3; i++) {
                iss >> token;

                int startPos = 0;
                int endPos = findEndOfNumber(token, startPos);
                mPrimitives(iPrimitive, i) = std::stoi(token.substr(startPos, endPos - startPos)) - 1;

                if (mAttributeSettings & AttributeSettings::LOAD_TEXTURES) {
                    startPos = findStartOfNumber(token, endPos);
                    endPos = findEndOfNumber(token, startPos);
                    mTexturePrimitives(iPrimitive, i) = std::stoi(token.substr(startPos, endPos - startPos)) - 1;
                }

                startPos = findStartOfNumber(token, endPos);
                endPos = findEndOfNumber(token, startPos);
                if (!(mAttributeSettings & AttributeSettings::LOAD_TEXTURES) && endPos != token.length()) {
                    startPos = findStartOfNumber(token, endPos);
                    endPos = findEndOfNumber(token, startPos);
                }

                if (mAttributeSettings & AttributeSettings::LOAD_NORMALS)
                    mNormalPrimitives(iPrimitive, i) = std::stoi(token.substr(startPos, endPos - startPos)) - 1;
            }

            iPrimitive++;
        } else if (line.compare(0, 2, "l ") == 0) {
            iss >> trashStr;

            std::string token;
            for (int i = 0; i < 2; i++) {
                iss >> token;

                int startPos = 0;
                int endPos = findEndOfNumber(token, startPos);
                mPrimitives(iPrimitive, i) = std::stoi(token.substr(startPos, endPos - startPos)) - 1;

                if (mAttributeSettings & AttributeSettings::LOAD_TEXTURES) {
                    startPos = findStartOfNumber(token, endPos);
                    endPos = findEndOfNumber(token, startPos);
                    mTexturePrimitives(iPrimitive, i) = std::stoi(token.substr(startPos, endPos - startPos)) - 1;
                }
            }

            iPrimitive++;
        }
    }

    file.close();
}

std::pair<std::vector<GLfloat>, std::vector<GLuint>> Mesh::PackMesh() const {
    std::vector<GLfloat> vertexBufferData;
    std::vector<GLuint> indexBufferData;

    // Map used to remember unique vertices
    std::unordered_map<std::string, unsigned int> indexMap;
    unsigned int currIndex = 0;

    // Pass to compute keys and store vertex buffer data
    for (int i = 0; i < mPrimitives.rows(); i++) {
        for (int j = 0; j < mPrimitives.cols(); j++) {
            int primitiveInd = mPrimitives(i, j);
            int normalPrimitiveInd = mNormalPrimitives(i, j);
            int texturePrimitiveInd = mTexturePrimitives(i, j);
            std::string key = std::to_string(primitiveInd) + "/" + std::to_string(normalPrimitiveInd) + "/" + std::to_string(texturePrimitiveInd);
            if (!indexMap.contains(key)) {
                indexMap[key] = currIndex++;

                if (mAttributeSettings & AttributeSettings::LOAD_POSITIONS) {
                    vertexBufferData.push_back((GLfloat)mPositions(primitiveInd, 0));
                    vertexBufferData.push_back((GLfloat)mPositions(primitiveInd, 1));
                    vertexBufferData.push_back((GLfloat)mPositions(primitiveInd, 2));
                }
                if ((mAttributeSettings & AttributeSettings::LOAD_NORMALS) ||
                    (mAttributeSettings & AttributeSettings::COMPUTE_NORMALS_VERTEX)) {
                    vertexBufferData.push_back((GLfloat)mNormals(normalPrimitiveInd, 0));
                    vertexBufferData.push_back((GLfloat)mNormals(normalPrimitiveInd, 1));
                    vertexBufferData.push_back((GLfloat)mNormals(normalPrimitiveInd, 2));
                }
                if (mAttributeSettings & AttributeSettings::LOAD_TEXTURES) {
                    vertexBufferData.push_back((GLfloat)mTextureUVs(texturePrimitiveInd, 0));
                    vertexBufferData.push_back((GLfloat)mTextureUVs(texturePrimitiveInd, 1));
                }
            }

            // Build Index Buffer
            indexBufferData.push_back(indexMap[key]);
        }
    }

    return {vertexBufferData, indexBufferData};
}

Mesh::PrimitiveType Mesh::GetPrimitiveType() const {
    return mPrimitiveType;
}

void Mesh::ComputeVertexNormals() {
    // Copy the primitive indices into the normals indices
    mNormalPrimitives = mPrimitives;

    int numPrimitives = mPrimitives.rows();

    // First compute the face normals
    Eigen::MatrixXd faceNormals = Eigen::MatrixXd::Zero(numPrimitives, 3);
    Eigen::VectorXd areas = Eigen::VectorXd::Zero(numPrimitives);
    for (int i = 0; i < numPrimitives; i++) {
        Eigen::Vector3i indices = mPrimitives.row(i);
        Eigen::Vector3d ab = mPositions.row(indices(2)) - mPositions.row(indices(0));
        Eigen::Vector3d ac = mPositions.row(indices(1)) - mPositions.row(indices(0));
        Eigen::Vector3d normal = ac.cross(ab);
        faceNormals.row(i) = normal.normalized();
        areas(i) = 0.5 * normal.norm();
    }

    // For each vertex, average the normals of faces of which the particular
    // vertex is a part of
    for (int i = 0; i < mPositions.rows(); i++) {
        Eigen::Vector3d normal = Eigen::Vector3d::Zero();
        double totalArea = 0.0;
        for (int j = 0; j < numPrimitives; j++) {
            if (mPrimitives(j, 0) == i || mPrimitives(j, 1) == i || mPrimitives(j, 2) == i) {
                double area = areas(j);
                normal += area * faceNormals.row(j);
                totalArea += area;
            }
        }

        // Average and Normalize the normal
        normal /= totalArea;
        normal.normalize();
        mNormals.row(i) = normal;
    }
}