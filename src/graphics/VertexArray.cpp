#include "graphics/VertexArray.hpp"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &mID);
}

VertexArray::~VertexArray() {
    // Restore the resource handle to the state machine
    glDeleteVertexArrays(1, &mID);
}

void VertexArray::Bundle(VertexBuffer* buffer, IndexBuffer* indexBuffer, VertexBufferLayout* layout, Shader* shader) {
    Bind();
    buffer->Bind();
    indexBuffer->Bind();

    int numLayoutElements = layout->GetNumLayoutElements();

    // Compute the stride
    GLsizei stride = 0;
    for (int i = 0; i < numLayoutElements; i++) {
        LayoutElement element = layout->GetLayoutElement(i);
        stride += element.attribSize * sizeof(GLfloat);
    }

    int currentPos = 0;
    // Enable the attributes in the layout
    for (int i = 0; i < layout->GetNumLayoutElements(); i++) {
        LayoutElement element = layout->GetLayoutElement(i);
        GLint attribLocation = shader->GetAttribLocation(element.attribName);
        if (attribLocation == -1) {
            std::cout << "ERROR: Could not locate " << element.attribName << " in shader" << std::endl;
            return;
        }

        // Enable the attribute
        glEnableVertexAttribArray((GLuint)attribLocation);

        // Specify offsets
        glVertexAttribPointer((GLuint)attribLocation, element.attribSize, GL_FLOAT, GL_FALSE, stride, (GLvoid*)currentPos);

        // Update the current total attrib position
        currentPos += sizeof(GLfloat) * element.attribSize;
    }

    Unbind();
    buffer->Unbind();
    indexBuffer->Unbind();
}

void VertexArray::Bind() const {
    // Bind this resource in the state machine
    glBindVertexArray(mID);
}

void VertexArray::Unbind() const {
    // Unbind the resource in the state machine
    glBindVertexArray(0);
}