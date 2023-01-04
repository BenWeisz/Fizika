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

enum Geometry {
    POINT,
    LINE,
    TRIANGLE,
    PLANE,
    CUBE,
    MESH
};

class Mesh {
   public:
    enum LoadOptions {
        POSITIONS = 0x01,
        TEXTURES = 0x02,
        NORMALS = 0x04,
    };

    Eigen::MatrixXd mPositions;
    Eigen::MatrixXd mNormals;
    Eigen::MatrixXd mTextureUVs;

    Mesh(const std::string& path);
    Mesh(const std::string& path, const LoadOptions loadOptions);
    Mesh(const Geometry geometry);
    Mesh(const Geometry geometry, const LoadOptions loadOptions);
    ~Mesh();
    void Bind() const;
    void Unbind() const;
    void Update();
    VertexBuffer* GetVertexBuffer() const;
    VertexArray* GetVertexArray() const;
    IndexBuffer* GetIndexBuffer() const;
    Geometry GetGeometry() const;

    static std::string GetMeshPrimitivePath(const Geometry geometry) {
        std::string path = "../res/models/";
        switch (geometry) {
            case Geometry::POINT:
                path += "point.obj";
                break;
            case Geometry::LINE:
                path += "line.obj";
                break;
            case Geometry::TRIANGLE:
                path += "triangle.obj";
                break;
            case Geometry::PLANE:
                path += "plane.obj";
                break;
            case Geometry::CUBE:
                path += "cube.obj";
                break;
            default:
                path += "triangle.obj";
        }

        return path;
    }
    static GLenum GetDrawModeEnum(const Geometry geometry) {
        switch (geometry) {
            case Geometry::POINT:
                return GL_POINTS;
            case Geometry::LINE:
                return GL_LINES;
            case Geometry::TRIANGLE:
                return GL_TRIANGLES;
            case Geometry::PLANE:
                return GL_TRIANGLES;
            case Geometry::CUBE:
                return GL_TRIANGLES;
            default:
                return GL_TRIANGLES;
        }
    }

   private:
    const LoadOptions mLoadOptions;

    // We won't do tearing simulations for know so the topology doesn't need to be changeable
    Eigen::MatrixXi mTopology;

    VertexBuffer* mVertexBuffer;
    VertexArray* mVertexArray;
    IndexBuffer* mIndexBuffer;

    Geometry mGeometry;

    void InitMesh(const std::string& path);
    void LoadFromFile(const std::string& path);
    static bool IsOBJControlToken(const std::string& token) {
        return token == "v" || token == "vt" || token == "vn" || token == "#" || token == "p" || token == "l" || token == "f";
    }
};