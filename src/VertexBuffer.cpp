#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const std::vector<GLfloat>& data) {
    // Acquire resource handle for vertex buffer object
    glGenBuffers(1, &mID);

    // Bind this array buffer to the array buffer state
    Bind();

    // Allocate space for the buffer   
    mSize = sizeof(GLfloat) * data.size();
    glBufferData(GL_ARRAY_BUFFER, mSize, 0, GL_DYNAMIC_DRAW);

    // Update the buffer with the data
    UpdateBuffer(data);

    GLint size = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    if (size != mSize) {
        glDeleteBuffers(1, &mID);
        std::cout << "Error: Failed to create vertex buffer" << std::endl;
    }

    Unbind();
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &mID);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, mID);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::UpdateBuffer(const std::vector<GLfloat>& data) {
    assert(data.size() * sizeof(GLfloat) == mSize);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mSize, &data[0]);
}