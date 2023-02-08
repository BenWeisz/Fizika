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

    // Count the number of occurances of each token
    int numPositions = 0;
    int numNormals = 0;
    int numTextureUVs = 0;
    int numFaces = 0;
    while (!file.eof()) {
        std::getline(file, line);
        if (line.compare(0, 2, "v ") == 0)
            numPositions++;
        else if (line.compare(0, 3, "vn ") == 0)
            numNormals++;
        else if (line.compare(0, 3, "vt ") == 0)
            numTextureUVs++;
        else if (line.compare(0, 2, "f ") == 0)
            numFaces++;
    }

    // Rewind file pointer
    file.clear();
    file.seekg(0, std::ios::beg);

    // Initialize the mesh data structure
    mPositions = Eigen::MatrixXd::Zero(numPositions, 3);
    mNormals = Eigen::MatrixXd::Zero(numNormals, 3);
    mTextureUVs = Eigen::MatrixXd::Zero(numTextureUVs, 2);

    mFaces = Eigen::MatrixXi::Zero(numFaces, 3);
    mNormalFaces = Eigen::MatrixXi::Zero(numFaces, 3);
    mTextureFaces = Eigen::MatrixXi::Zero(numFaces, 3);

    // Populate the matrices
    int iPosition = 0;
    int iNormal = 0;
    int iTextureUV = 0;
    int iFace = 0;
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
                mFaces(iFace, i) = std::stoi(token.substr(startPos, endPos - startPos)) - 1;

                startPos = findStartOfNumber(token, endPos);
                endPos = findEndOfNumber(token, startPos);
                mTextureFaces(iFace, i) = std::stoi(token.substr(startPos, endPos - startPos)) - 1;

                startPos = findStartOfNumber(token, endPos);
                endPos = findEndOfNumber(token, startPos);
                mNormalFaces(iFace, i) = std::stoi(token.substr(startPos, endPos - startPos)) - 1;
            }

            iFace++;
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
    for (int i = 0; i < mFaces.rows(); i++) {
        for (int j = 0; j < mFaces.cols(); j++) {
            int faceInd = mFaces(i, j);
            int normalFaceInd = mNormalFaces(i, j);
            int textureFaceInd = mTextureFaces(i, j);
            std::string key = std::to_string(faceInd) + "/" + std::to_string(normalFaceInd) + "/" + std::to_string(textureFaceInd);
            if (!indexMap.contains(key)) {
                indexMap[key] = currIndex++;

                if (mAttributeSettings & AttributeSettings::LOAD_POSITIONS) {
                    vertexBufferData.push_back((GLfloat)mPositions(faceInd, 0));
                    vertexBufferData.push_back((GLfloat)mPositions(faceInd, 1));
                    vertexBufferData.push_back((GLfloat)mPositions(faceInd, 2));
                }
                if (mAttributeSettings & AttributeSettings::LOAD_NORMALS) {
                    vertexBufferData.push_back((GLfloat)mNormals(normalFaceInd, 0));
                    vertexBufferData.push_back((GLfloat)mNormals(normalFaceInd, 1));
                    vertexBufferData.push_back((GLfloat)mNormals(normalFaceInd, 2));
                }
                if (mAttributeSettings & AttributeSettings::LOAD_TEXTURES) {
                    vertexBufferData.push_back((GLfloat)mTextureUVs(textureFaceInd, 0));
                    vertexBufferData.push_back((GLfloat)mTextureUVs(textureFaceInd, 1));
                }
            }

            // Build Index Buffer
            indexBufferData.push_back(indexMap[key]);
        }
    }

    return {vertexBufferData, indexBufferData};
}