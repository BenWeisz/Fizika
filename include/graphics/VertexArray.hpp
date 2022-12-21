#pragma once

#include <memory>

#include "glad/glad.h"

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray {
    public:
        VertexArray(const VertexBuffer& buffer, std::shared_ptr<VertexBufferLayout> layout);
        ~VertexArray();
        void Bind() const;
        void Unbind() const;
    private:
        GLuint mID;
};