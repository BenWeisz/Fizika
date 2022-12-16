#pragma once

#include <vector>

#include "glad/glad.h"

#include "Shader.hpp"

typedef struct LayoutElement {
    GLuint attribLocation;
    GLint attribSize;
} LayoutElement;

class VertexBufferLayout {
    public:
        VertexBufferLayout(const Shader& shader);
        void AddLayoutElement(const std::string& attribName, GLint attribSize);
        int GetNumLayoutElements() const;
        LayoutElement GetLayoutElement(int index) const;
    private:
        const Shader& mShader;
        std::vector<LayoutElement> mLayoutElements;
};