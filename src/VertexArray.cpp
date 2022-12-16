#include "VertexArray.hpp"

VertexArray::VertexArray() {
    // Acquire resource handle for this vertex array object
    glGenVertexArrays(1, &mID);

    Bind();

    Unbind();
}

VertexArray::~VertexArray() {
    // Restore the resource handle to the state machine
    glDeleteVertexArrays(1, &mID);
}

void VertexArray::Bind() const {
    // Bind this resource in the state machine
    glBindVertexArray(mID);
}

void VertexArray::Unbind() const {
    // Unbind the resource in the state machine
    glBindVertexArray(0);
}