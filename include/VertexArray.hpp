#pragma once

#include "glad/glad.h"

class VertexArray {
    public:
        VertexArray();
        ~VertexArray();
        void Bind() const;
        void Unbind() const;
    private:
        GLuint mID;
};