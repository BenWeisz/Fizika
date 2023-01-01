#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <Eigen/Core>
#include "glad/glad.h"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

class Mesh {
   public:
    enum Presentation {
        POINTS,
        LINES,
        TRIANGLES
    };

    enum Geometry {
        POINT,
        LINE,
        TRIANGLE,
        PLANE,
        CUBE
    };

    enum LoadOptions {
        POSITIONS = 0x01,
        TEXTURES = 0x02,
        NORMALS = 0x04,
    };

    Eigen::MatrixXd mPositions;
    Eigen::MatrixXd mNormals;

    Mesh(const std::string& path, const LoadOptions loadOptions);
    Mesh(const Geometry geometry);
    Mesh(const Geometry geometry, const LoadOptions loadOptions);
    ~Mesh();
    void Bind() const;
    void Unbind() const;
    void Update();
    VertexBuffer* GetVertexBuffer();
    VertexArray* GetVertexArray();
    void ChangePresentation(const Presentation presentation);

   private:
    Presentation mPresentation;
    const LoadOptions mLoadOptions;

    // We won't do tearing simulations for know so the topology doesn't need to be changeable
    Eigen::MatrixXi mPrimitives;
    // We don't need to changethe UV coordinates
    Eigen::MatrixXd mTextureUVs;

    VertexBuffer* mVertexBuffer;
    VertexArray* mVertexArray;
    IndexBuffer* mIndexBuffer;

    void InitMesh(const std::string& path);
    void LoadFromFile(const std::string& path);
    static bool IsOBJControlToken(const std::string& token) {
        return token == "v" || token == "vt" || token == "vn" || token == "#" || token == "p" || token == "l" || token == "f";
    }
};