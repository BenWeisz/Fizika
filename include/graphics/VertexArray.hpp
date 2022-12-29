#pragma once

#include <memory>

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "glad/glad.h"

class VertexArray {
   public:
    VertexArray(const VertexBuffer& buffer, std::shared_ptr<VertexBufferLayout> layout);
    ~VertexArray();
    void Bind() const;
    void Unbind() const;

   private:
    GLuint mID;
};