#pragma once

#include <vector>
#include <string>

#include "Shader.hpp"
#include "glad/glad.h"

typedef struct LayoutElement {
    std::string attribName;
    GLint attribSize;
} LayoutElement;

class VertexBufferLayout {
   public:
    void AddLayoutElement(const std::string& attribName, GLint attribSize);
    int GetNumLayoutElements() const;
    LayoutElement GetLayoutElement(int index) const;

   private:
    std::vector<LayoutElement> mLayoutElements;
};