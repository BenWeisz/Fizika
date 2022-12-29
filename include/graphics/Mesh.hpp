#pragma once

#include <string>

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
        NORMALS = 0x02,
        TEXTURES = 0x04
    };

    Presentation mPresentation;
    VertexBuffer* mVertexBuffer;
    VertexArray* mVertexArray;

    Mesh(const std::string& path, const LoadOptions loadOptions);
    Mesh(const Geometry geometry);
    ~Mesh();
    void Bind() const;
    void Unbind() const;

    void InitMesh(const std::string& path);
    void LoadFromFile(const std::string& path);
    VertexBuffer* GetVertexBuffer();
    VertexArray* GetVertexArray();
    void ChangePresentation(const Presentation presentation);
};