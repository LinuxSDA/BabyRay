//
//  Texture.cpp
//  OpenGL
//
//  Created by Sumit Dhingra on 02/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#include "Texture.hpp"
#include "ErrorHandler.hpp"
#include "stb_image.h"
#include <fstream>

Texture::Texture(unsigned int width, unsigned int height, unsigned int channels): mWidth(width), mHeight(height), mChannels(channels)
{
    if (mChannels == 1)
        mFormat = GL_RED;
    else if (mChannels == 3)
        mFormat = GL_RGB;
    else if (mChannels == 4)
        mFormat = GL_RGBA;
    else
        throw std::runtime_error("Bad Channel!");
    
    GLCall(glGenTextures(1, &mRendererId));
    GLCall(glBindTexture(GL_TEXTURE_2D, mRendererId));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, mFormat, mWidth, mHeight, 0, mFormat, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::Texture(const std::string& path):mRendererId(0), mFilePath(path), mLocalBuffer(nullptr), mWidth(0), mHeight(0), mChannels(0)
{
    std::ifstream f(path.c_str());
    ASSERT(f.good());
    
    stbi_set_flip_vertically_on_load(true);
    mLocalBuffer = stbi_load(path.c_str(), &mWidth, &mHeight, &mChannels, 0);
    
    if (mChannels == 1)
        mFormat = GL_RED;
    else if (mChannels == 3)
        mFormat = GL_RGB;
    else if (mChannels == 4)
        mFormat = GL_RGBA;
    
    GLCall(glGenTextures(1, &mRendererId));
    GLCall(glBindTexture(GL_TEXTURE_2D, mRendererId));
    
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, mFormat, mWidth, mHeight, 0, mFormat, GL_UNSIGNED_BYTE, mLocalBuffer));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    
    if(mLocalBuffer)
        stbi_image_free(mLocalBuffer);
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &mRendererId));
}

void Texture::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, mRendererId));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

const std::string& Texture::GetTexturePath() const
{
    return mFilePath;
}
