#pragma once

#include <vector>

#include "glad/glad.h"

#include "util/log.hpp"

class FrameBuffer {
   public:
    enum AttachmentType {
        COLOR,
        DEPTH,
        STENCIL,
        DEPTH_STENCIL
    };

    struct Attachment {
        AttachmentType type;
        GLuint ID;
    };

   private:
    GLuint mID;

    unsigned int mWidth;
    unsigned int mHeight;

    std::vector<Attachment> mAttachments;

    // These are computed from the attachments in mAttachments
    bool mHasColorBuffer;
    bool mHasDepthBuffer;
    bool mHasStencilBuffer;

    float mClearColor[4];

   public:
    FrameBuffer(unsigned int width, unsigned int height);
    ~FrameBuffer();

    void AddAttachment(const AttachmentType attachmentType);
    GLuint GetAttachmentBufferID(const AttachmentType attachmentType) const;

    void SetClearColor(const float r, const float g, const float b, const float a);

    // Bind must be called before packing
    void Pack();
    void BeginDraw();
    void EndDraw();
};