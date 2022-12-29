#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <Eigen/Core>

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

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

    Presentation mPresentation;
    const LoadOptions mLoadOptions;
    VertexBuffer* mVertexBuffer;
    VertexArray* mVertexArray;

    Eigen::MatrixXd mPositions;
    Eigen::MatrixXd mTexturesUVs;
    Eigen::MatrixXd mNormals;
    Eigen::MatrixXi mPrimitives;

    Mesh(const std::string& path, const LoadOptions loadOptions);
    Mesh(const Geometry geometry);
    Mesh(const Geometry geometry, const LoadOptions loadOptions);
    ~Mesh();
    void Bind() const;
    void Unbind() const;
    VertexBuffer* GetVertexBuffer();
    VertexArray* GetVertexArray();
    void ChangePresentation(const Presentation presentation);

   public:
    void InitMesh(const std::string& path);
    void LoadFromFile(const std::string& path);
    static bool IsOBJControlToken(const std::string& token) {
        return token == "v" || token == "vt" || token == "vn" || token == "#" || token == "p" || token == "l" || token == "f";
    }
};