#include "VertexArray.hpp"

VertexArray::VertexArray(const VertexBuffer& buffer, const VertexBufferLayout& layout) {
    // Acquire resource handle for this vertex array object
    glGenVertexArrays(1, &mID);

    Bind();
    buffer.Bind();

    int numLayoutElements = layout.GetNumLayoutElements();

    // Compute the stride
    GLsizei stride = 0;
    for (int i = 0; i < numLayoutElements; i++) {
        LayoutElement element = layout.GetLayoutElement(i);
        stride += element.attribSize * sizeof(GLfloat);
    }

    // Enable the attributes in the layout
    for (int i = 0; i < layout.GetNumLayoutElements(); i++) {
        LayoutElement element = layout.GetLayoutElement(i);

        // Enable the attribute
        glEnableVertexAttribArray(element.attribLocation);
        
        // Specify offsets
        glVertexAttribPointer(element.attribLocation, element.attribSize, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
    }

    buffer.Unbind();
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