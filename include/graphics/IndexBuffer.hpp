#pragma once

#include <vector>

#include "glad/glad.h"

class IndexBuffer {
   public:
    IndexBuffer(const std::vector<GLuint>& indices);
    ~IndexBuffer();
    void Bind();
    void Unbind();

   private:
    GLuint mID;
};