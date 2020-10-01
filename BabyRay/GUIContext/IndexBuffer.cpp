//
//  IndexBuffer.cpp
//  OpenGL
//
//  Created by Sumit Dhingra on 01/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#include "IndexBuffer.hpp"
#include "ErrorHandler.hpp"

IndexBuffer::IndexBuffer(const std::vector<unsigned int>& indices) : mCount(static_cast<unsigned int>(indices.size()))
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    
    GLCall(glGenBuffers(1, &mRendererId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), static_cast<const void *>(indices.data()), GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &mRendererId));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererId));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
