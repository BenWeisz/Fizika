#include "graphics/FrameBuffer.hpp"

FrameBuffer* FrameBuffer::DefaultFrameBuffer = nullptr;

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
    : mWidth(width), mHeight(height), mHasColorBuffer(false), mHasDepthBuffer(false), mHasStencilBuffer(false) {
    glGenFramebuffers(1, &mID);

    mClearColor = glm::vec3(0.0f, 0.0f, 0.0f);
}

FrameBuffer::~FrameBuffer() {
    if (mID != 0)
        glDeleteFramebuffers(1, &mID);

    // Delete the associated textures
    for (unsigned int i = 0; i < mAttachments.size(); i++) {
        glDeleteTextures(1, &(mAttachments[i].ID));
    }
}

FrameBuffer::FrameBuffer() {
}

void FrameBuffer::AddAttachment(const AttachmentType attachmentType) {
    // Don't allow adding attachments for the default framebuffer
    if (mID == 0)
        return;

    if (mAttachments.size() == 3) {
        Log_Error("Failed to add attachment, there are already 3");
        return;
    }

    GLuint ID;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    GLint internalFormat;
    GLenum format;
    GLenum type;

    if (attachmentType == AttachmentType::COLOR) {
        internalFormat = GL_RGB;
        format = GL_RGB;
        type = GL_UNSIGNED_BYTE;
        mHasColorBuffer = true;
    } else if (attachmentType == AttachmentType::DEPTH) {
        internalFormat = GL_DEPTH_COMPONENT;
        format = GL_DEPTH_COMPONENT;
        type = GL_FLOAT;
        mHasDepthBuffer = true;
    } else if (attachmentType == AttachmentType::STENCIL) {
        internalFormat = GL_STENCIL_INDEX;
        format = GL_STENCIL_INDEX;
        type = GL_UNSIGNED_BYTE;
        mHasStencilBuffer = true;
    } else if (attachmentType == AttachmentType::DEPTH_STENCIL) {
        internalFormat = GL_DEPTH24_STENCIL8;
        format = GL_DEPTH_STENCIL;
        type = GL_UNSIGNED_INT_24_8;
        mHasDepthBuffer = true;
        mHasStencilBuffer = true;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, format, type, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    mAttachments.push_back({attachmentType, ID});
}

GLuint FrameBuffer::GetAttachmentBufferID(const AttachmentType attachmentType) const {
    // Return 0 if this is the default framebuffer
    if (mID == 0)
        return 0;

    for (int i = 0; i < mAttachments.size(); i++) {
        if (mAttachments[i].type == attachmentType)
            return mAttachments[i].ID;
    }

    return 0;
}

void FrameBuffer::SetClearColor(const float r, const float g, const float b) {
    mClearColor = glm::vec3(r, g, b);
}

// Bind must be called before packing
void FrameBuffer::Pack() {
    // Don't allow packing for the default framebuffer
    if (mID == 0)
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, mID);

    for (unsigned int i = 0; i < mAttachments.size(); i++) {
        AttachmentType attachmentType = mAttachments[i].type;
        GLenum attachment;
        if (attachmentType == AttachmentType::COLOR) {
            attachment = GL_COLOR_ATTACHMENT0;
        } else if (attachmentType == AttachmentType::DEPTH) {
            attachment = GL_DEPTH_ATTACHMENT;
        } else if (attachmentType == AttachmentType::STENCIL) {
            attachment = GL_STENCIL_ATTACHMENT;
        } else if (attachmentType == AttachmentType::DEPTH_STENCIL) {
            attachment = GL_DEPTH_STENCIL_ATTACHMENT;
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, mAttachments[i].ID, 0);
    }

    if (!mHasColorBuffer) {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log_Error("One or more FrameBuffer attachments failed");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BeginDraw() {
    glViewport(0, 0, mWidth, mHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, mID);

    GLbitfield clearMask = 0;
    clearMask |= mHasColorBuffer ? GL_COLOR_BUFFER_BIT : 0;
    clearMask |= mHasDepthBuffer ? GL_DEPTH_BUFFER_BIT : 0;
    clearMask |= mHasStencilBuffer ? GL_STENCIL_BUFFER_BIT : 0;

    if (mHasColorBuffer)
        glClearColor(mClearColor.x, mClearColor.y, mClearColor.z, 0.0);
    if (mHasDepthBuffer)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if (mHasStencilBuffer)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);

    glClear(clearMask);
}

void FrameBuffer::EndDraw() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}