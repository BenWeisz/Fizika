#include "graphics/VertexBufferLayout.hpp"

void VertexBufferLayout::AddLayoutElement(const std::string &attribName, GLint attribSize) {
    LayoutElement element;
    element.attribName = attribName;
    element.attribSize = attribSize;

    mLayoutElements.push_back(element);
}

int VertexBufferLayout::GetNumLayoutElements() const {
    return mLayoutElements.size();
}

LayoutElement VertexBufferLayout::GetLayoutElement(int index) const {
    return mLayoutElements.at(index);
}