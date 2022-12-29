#pragma once

#include <cassert>
#include <iostream>
#include <vector>

#include "glad/glad.h"

class VertexBuffer {
   public:
    VertexBuffer(const std::vector<GLfloat>& data);
    ~VertexBuffer();
    void Bind() const;
    void Unbind() const;
    void UpdateBuffer(const std::vector<GLfloat>& data);

   private:
    GLuint mID;
    GLsizeiptr mSize;
};