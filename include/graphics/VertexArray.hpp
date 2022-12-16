#pragma once

#include "glad/glad.h"

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray {
    public:
        VertexArray(const VertexBuffer& buffer, const VertexBufferLayout& layout);
        ~VertexArray();
        void Bind() const;
        void Unbind() const;
    private:
        GLuint mID;
};