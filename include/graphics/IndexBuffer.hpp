#pragma once

#include <vector>
#include <iostream>

#include "glad/glad.h"

class IndexBuffer {
   public:
    IndexBuffer(const std::vector<GLuint>& indices);
    ~IndexBuffer();
    void Bind();
    void Unbind();
    GLsizei GetCount() const;

   private:
    GLuint mID;
    GLsizei mCount;
};