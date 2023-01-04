#include "../include/graphics/Mesh.hpp"

Mesh::Mesh(const std::string& path) : mLoadOptions(LoadOptions::POSITIONS) {
    // Initialize the mesh
    InitMesh(path);
}

Mesh::Mesh(const std::string& path, const LoadOptions loadOptions) : mLoadOptions((LoadOptions)(LoadOptions::POSITIONS | loadOptions)) {
    // Initialize the mesh
    InitMesh(path);
}

Mesh::Mesh(const Geometry geometry) : mLoadOptions(LoadOptions::POSITIONS) {
    // Initialize the mesh using a default mesh
    std::string path = GetMeshPrimitivePath(geometry);
    InitMesh(path);
}

Mesh::Mesh(const Geometry geometry, const LoadOptions loadOptions) : mLoadOptions((LoadOptions)(LoadOptions::POSITIONS | loadOptions)) {
    // Initialize the mesh using a default mesh
    std::string path = GetMeshPrimitivePath(geometry);
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
    std::vector<GLfloat> bufferData;
    for (int i = 0; i < mPositions.rows(); i++) {
        if (mLoadOptions & LoadOptions::POSITIONS) {
            bufferData.push_back((GLfloat)mPositions(i, 0));
            bufferData.push_back((GLfloat)mPositions(i, 1));
            bufferData.push_back((GLfloat)mPositions(i, 2));
        } else if (mLoadOptions & LoadOptions::NORMALS) {
            bufferData.push_back((GLfloat)mNormals(i, 0));
            bufferData.push_back((GLfloat)mNormals(i, 1));
            bufferData.push_back((GLfloat)mNormals(i, 2));
        } else if (mLoadOptions & LoadOptions::TEXTURES) {
            bufferData.push_back((GLfloat)mTextureUVs(i, 0));
            bufferData.push_back((GLfloat)mTextureUVs(i, 1));
        }
    }

    mVertexBuffer->Update(bufferData);
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
    std::vector<GLfloat> bufferData;
    for (int i = 0; i < mPositions.rows(); i++) {
        if (mLoadOptions & LoadOptions::POSITIONS) {
            bufferData.push_back((GLfloat)mPositions(i, 0));
            bufferData.push_back((GLfloat)mPositions(i, 1));
            bufferData.push_back((GLfloat)mPositions(i, 2));
        } else if (mLoadOptions & LoadOptions::NORMALS) {
            bufferData.push_back((GLfloat)mNormals(i, 0));
            bufferData.push_back((GLfloat)mNormals(i, 1));
            bufferData.push_back((GLfloat)mNormals(i, 2));
        } else if (mLoadOptions & LoadOptions::TEXTURES) {
            bufferData.push_back((GLfloat)mTextureUVs(i, 0));
            bufferData.push_back((GLfloat)mTextureUVs(i, 1));
        }
    }

    mVertexBuffer = new VertexBuffer(bufferData);

    // Load the mesh topology into the index buffer
    std::vector<GLuint>
        indices;
    for (int iPrimitive = 0; iPrimitive < mTopology.rows(); iPrimitive++)
        for (int dim = 0; dim < mTopology.cols(); dim++)
            indices.push_back((GLuint)mTopology(iPrimitive, dim));

    mIndexBuffer = new IndexBuffer(indices);

    // Create the texture
    mVertexArray = new VertexArray();
}

// Operates on a dumbed down version of .obj where the
// position, texture and normal index must match for each vertex
void Mesh::LoadFromFile(const std::string& path) {
    std::ifstream file(path.c_str());

    // Simple error checking
    if (file.bad()) {
        std::cout << "ERROR: Could not load .obj file: " << path << std::endl;
        return;
    }

    // Convert the file buffer into a string stream
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::stringstream::pos_type startPos = buffer.tellg();

    // Initial Pass
    // Compute the number of vertices, textureUVs and normals
    int numPositions = 0;
    int numTextureUVs = 0;
    int numNormals = 0;
    int numPoints = 0;
    int numLines = 0;
    int numFaces = 0;

    std::string token;
    buffer >> token;
    while (!buffer.eof()) {
        if (token == "v" && (mLoadOptions & Mesh::LoadOptions::POSITIONS))
            numPositions++;
        else if (token == "vt" && (mLoadOptions & Mesh::LoadOptions::TEXTURES))
            numTextureUVs++;
        else if (token == "vn" && (mLoadOptions & Mesh::LoadOptions::NORMALS))
            numNormals++;
        else if (token == "p")
            numPoints++;
        else if (token == "l")
            numLines++;
        else if (token == "f")
            numFaces++;

        buffer >> token;
        while (!buffer.eof() && !Mesh::IsOBJControlToken(token))
            buffer >> token;
    }

    // Make sure that we are only modeling one type of primitive at a time
    int hasMultipleTypes = (numPoints > 0 ? 1 : 0) + (numLines > 0 ? 1 : 0) + (numFaces > 0 ? 1 : 0);
    if (hasMultipleTypes > 1) {
        std::cout << "ERROR: " << path << " contains multiple types of primitives, please use only one" << std::endl;
        return;
    }

    // Check to see that at least some connective data exists
    const int numPrimitives = numPoints + numLines + numFaces;
    if (numPrimitives == 0) {
        std::cout << "ERROR: " << path << " contains no connective data" << std::endl;
        return;
    }

    // Compute the number of vertices required to show the primitive in the file
    const int primitiveDegree = (numPoints > 0 ? 1 : (numLines > 0 ? 2 : 3));

    // Set up the matrices for data loading
    mPositions = Eigen::MatrixXd::Zero(numPositions, 3);
    mTextureUVs = Eigen::MatrixXd::Zero(numTextureUVs, 2);
    mNormals = Eigen::MatrixXd::Zero(numNormals, 3);

    mTopology = Eigen::MatrixXi::Zero(numPrimitives, primitiveDegree);

    // Rewind to the beginning of the file
    buffer.clear();
    buffer.seekg(startPos, buffer.beg);

    // Start loading the data into the matrices
    int iPosition = 0;
    int iTextureUV = 0;
    int iNormal = 0;
    int iPrimitive = 0;
    buffer >> token;
    while (!buffer.eof()) {
        // Load the positional data
        if (token == "v" && (mLoadOptions & Mesh::LoadOptions::POSITIONS)) {
            double v1, v2, v3;
            buffer >> v1 >> v2 >> v3;
            mPositions(iPosition, 0) = v1;
            mPositions(iPosition, 1) = v2;
            mPositions(iPosition, 2) = v3;

            iPosition++;
            buffer >> token;
        }
        // Load the texture uv data
        else if (token == "vt" && (mLoadOptions & Mesh::LoadOptions::TEXTURES)) {
            double vt1, vt2;
            buffer >> vt1 >> vt2;
            mTextureUVs(iTextureUV, 0) = vt1;
            mTextureUVs(iTextureUV, 1) = vt2;

            iTextureUV++;
            buffer >> token;
        }
        // Load the normal data
        else if (token == "vn" && (mLoadOptions & Mesh::LoadOptions::NORMALS)) {
            double vn1, vn2, vn3;
            buffer >> vn1 >> vn2 >> vn3;
            mNormals(iNormal, 0) = vn1;
            mNormals(iNormal, 1) = vn2;
            mNormals(iNormal, 2) = vn3;

            iNormal++;
            buffer >> token;
        }
        // Skip the comments
        else if (token == "#") {
            buffer >> token;
            while (!buffer.eof() && !Mesh::IsOBJControlToken(token))
                buffer >> token;
        }
        // Load the primitive connections
        else if (token == "p" || token == "l" || token == "f") {
            buffer >> token;
            int i = 0;
            while (!buffer.eof() && !Mesh::IsOBJControlToken(token)) {
                mTopology(iPrimitive, i) = std::stoi(token) - 1;

                i++;
                buffer >> token;
            }

            iPrimitive++;
        }

        // Skip blank lines
        else {
            buffer >> token;
        }
    }
}
