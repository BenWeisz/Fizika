#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include <Eigen/Core>
#include <Eigen/Dense>
#include "glad/glad.h"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

class Mesh {
   public:
    enum AttributeSettings {
        LOAD_POSITIONS = 0x01,
        LOAD_NORMALS = 0x02,
        LOAD_TEXTURES = 0x04,
    };

    enum PrimitiveType {
        TRIANGLES = GL_TRIANGLES,
        LINES = GL_LINES
    };

    Mesh(const std::string& path);
    Mesh(const std::string& path, const AttributeSettings attribSettings);
    ~Mesh();

    void Bind() const;
    void Unbind() const;
    void Update();

    VertexBuffer* GetVertexBuffer() const;
    VertexArray* GetVertexArray() const;
    IndexBuffer* GetIndexBuffer() const;

    PrimitiveType GetPrimitiveType() const;

    Eigen::MatrixXd mPositions;
    Eigen::MatrixXi mPrimitives;

   private:
    const AttributeSettings mAttributeSettings;
    PrimitiveType mPrimitiveType;

    // We won't do tearing simulations for know so the topology doesn't need to be changeable
    Eigen::MatrixXd mNormals;
    Eigen::MatrixXd mTextureUVs;

    Eigen::MatrixXi mNormalPrimitives;
    Eigen::MatrixXi mTexturePrimitives;

    VertexBuffer* mVertexBuffer;
    VertexArray* mVertexArray;
    IndexBuffer* mIndexBuffer;

    void InitMesh(const std::string& path);
    void ReadFromFile(const std::string& path, bool& containsNormals);
    void WriteToFile(const std::string& path);
    std::pair<std::vector<GLfloat>, std::vector<GLuint>> PackMesh() const;

    void ComputeVertexNormals();
    void ComputeCornerNormals();
};