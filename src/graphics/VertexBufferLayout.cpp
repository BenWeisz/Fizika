#include "graphics/VertexBufferLayout.hpp"

VertexBufferLayout::VertexBufferLayout(std::shared_ptr<Shader> shader) : mShader(shader) {}

void VertexBufferLayout::AddLayoutElement(const std::string &attribName, GLint attribSize) {
    GLint attribLocation = mShader->GetAttribLocation(attribName);

    LayoutElement element;
    element.attribLocation = attribLocation;
    element.attribSize = attribSize;
    
    mLayoutElements.push_back(element);
}

int VertexBufferLayout::GetNumLayoutElements() const {
    return mLayoutElements.size();
}

LayoutElement VertexBufferLayout::GetLayoutElement(int index) const {
    return mLayoutElements.at(index);
}