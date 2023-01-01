#pragma once

#include "glad/glad.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray {
   public:
    VertexArray();
    ~VertexArray();
    void Bundle(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, VertexBufferLayout* layout, Shader* shader);
    void Bind() const;
    void Unbind() const;

   private:
    GLuint mID;
};