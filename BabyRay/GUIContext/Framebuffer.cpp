//
//  Framebuffer.cpp
//  OpenGL
//
//  Created by Sumit Dhingra on 24/06/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#include "Framebuffer.hpp"
#include "ErrorHandler.hpp"

Framebuffer::Framebuffer()
{
    GLCall(glGenFramebuffers(1, &mFramebufferID));
    GLCall(glGenRenderbuffers(1, &mRenderbufferID));
}

Framebuffer::~Framebuffer()
{
    GLCall(glDeleteBuffers(1, &mFramebufferID));
    GLCall(glDeleteBuffers(1, &mRenderbufferID));
}

void Framebuffer::Bind(const Texture& texture)
{
    GLCall(glGetIntegerv(GL_VIEWPORT, mViewPort));
    
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.GetTextureID(), 0));

    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, mRenderbufferID));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, texture.GetWidth(), texture.GetHeight()));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderbufferID));

    ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    GLCall(glViewport(0, 0, texture.GetWidth(), texture.GetHeight()));
    GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glEnable(GL_DEPTH_TEST));
}

void Framebuffer::Unbind()
{
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    GLCall(glViewport(mViewPort[0], mViewPort[1], mViewPort[2], mViewPort[3]));
}
