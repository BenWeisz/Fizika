#include "../include/graphics/IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const std::vector<GLuint>& indices) {
    mCount = (GLsizei)indices.size();
    glGenBuffers(1, &mID);

    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    Unbind();
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &mID);
}

void IndexBuffer::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
}

void IndexBuffer::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLsizei IndexBuffer::GetCount() const {
    return mCount;
}