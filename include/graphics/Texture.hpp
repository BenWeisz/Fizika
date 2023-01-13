#pragma once

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>

class Texture {
   public:
    enum Filter {
        NEAREST,
        LINEAR
    };

    Texture(const std::string& path);
    Texture(const std::string& path, const Filter& magFilter, const Filter& minFilter);
    ~Texture();
    void Bind() const;
    void Unbind() const;
    GLuint GetID() const;

   private:
    GLuint mID;
    unsigned char* mImageData;
    int mWidth;
    int mHeight;
    int mNumChannels;
    Filter mMagFilter;
    Filter mMinFilter;

    void InitTexture(const std::string& path);
    void LoadImageFile(const std::string& path);
    static GLint GetGLFilterEnum(const Filter& filter) {
        switch (filter) {
            case Filter::NEAREST:
                return GL_NEAREST;
            case Filter::LINEAR:
                return GL_LINEAR;
            default:
                return Filter::NEAREST;
        }
    }
};