#include "graphics/Texture.hpp"

Texture::Texture(const std::string& path) : mMagFilter(Filter::NEAREST), mMinFilter(Filter::NEAREST) {
    // Initialize the texture
    InitTexture(path);
}

Texture::Texture(const std::string& path, const Filter& magFilter, const Filter& minFilter) : mMagFilter(magFilter), mMinFilter(minFilter) {
    // Initialize the texture
    InitTexture(path);
}

Texture::~Texture() {
    glDeleteTextures(1, &mID);
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, mID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::InitTexture(const std::string& path) {
    // Load the image file
    LoadImageFile(path);

    if (!path.ends_with(".png")) {
        std::cout << "ERROR: Only .png texture files are supported: " << path << std::endl;
        return;
    }

    if (!mImageData) {
        std::cout << "ERROR: Could not load texture: " << path << std::endl;
        return;
    }

    // Generate a resource handle
    glGenTextures(1, &mID);

    Bind();

    // Set the filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilterEnum(mMagFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilterEnum(mMinFilter));

    // Load the image data into the texture buffer
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mImageData);

    Unbind();

    // Free the image resource reference to file
    stbi_image_free(mImageData);
}

void Texture::LoadImageFile(const std::string& path) {
    // Load the image file
    stbi_set_flip_vertically_on_load(true);
    mImageData = stbi_load(path.c_str(), &mWidth, &mHeight, &mNumChannels, 0);
}

GLuint Texture::GetID() const {
    return mID;
}