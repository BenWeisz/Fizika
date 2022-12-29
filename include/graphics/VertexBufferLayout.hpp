#pragma once

#include <memory>
#include <vector>

#include "Shader.hpp"
#include "glad/glad.h"

typedef struct LayoutElement {
    GLuint attribLocation;
    GLint attribSize;
} LayoutElement;

class VertexBufferLayout {
   public:
    VertexBufferLayout(std::shared_ptr<Shader> shader);
    void AddLayoutElement(const std::string& attribName, GLint attribSize);
    int GetNumLayoutElements() const;
    LayoutElement GetLayoutElement(int index) const;

   private:
    const std::shared_ptr<Shader> mShader;
    std::vector<LayoutElement> mLayoutElements;
};