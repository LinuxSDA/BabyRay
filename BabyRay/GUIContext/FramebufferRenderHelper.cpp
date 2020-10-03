//
//  FramebufferRenderHelper.cpp
//  OpenGL
//
//  Created by Sumit Dhingra on 19/07/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#include "FramebufferRenderHelper.hpp"

namespace Helper
{
    FramebufferRenderer::FramebufferRenderer(int ScreenWidth, int ScreenHeight): mFramebufferTexture(ScreenWidth, ScreenHeight, 3)
    {
        InitBuffers();
        mQuadVA.CreateVBuffer2f(mScreenVertices);
        mQuadVA.CreateVBuffer2f(mTexCoords);
        mQuadVA.CreateIBuffer(mIndices);
    }
    
    FramebufferRenderer::~FramebufferRenderer()
    {
        
    }
    
    void FramebufferRenderer::Draw(const Renderer& renderer, const Shader& shader) const
    {
        mFramebufferTexture.Bind();
        renderer.DisableDepth(); // disable depth test so screen-space quad isn't discarded due to depth test.
        renderer.Draw(mQuadVA, shader);
        renderer.EnableDepth(GL_LESS);
    }
    
    void FramebufferRenderer::InitBuffers()
    {
        mScreenVertices.reserve(8);
    
        mScreenVertices.emplace_back(-1.0f);
        mScreenVertices.emplace_back( 1.0f);
    
        mScreenVertices.emplace_back( 1.0f);
        mScreenVertices.emplace_back( 1.0f);
    
        mScreenVertices.emplace_back( 1.0f);
        mScreenVertices.emplace_back(-1.0f);

        mScreenVertices.emplace_back(-1.0f);
        mScreenVertices.emplace_back(-1.0f);

        mIndices.reserve(6);
        
        mIndices.emplace_back(0);
        mIndices.emplace_back(1);
        mIndices.emplace_back(2);
        
        mIndices.emplace_back(0);
        mIndices.emplace_back(2);
        mIndices.emplace_back(3);

        mTexCoords.reserve(8);
    
        mTexCoords.emplace_back(0.0f);
        mTexCoords.emplace_back(0.0f);
    
        mTexCoords.emplace_back(1.0f);
        mTexCoords.emplace_back(0.0f);
        
        mTexCoords.emplace_back(1.0f);
        mTexCoords.emplace_back(1.0f);

        mTexCoords.emplace_back(0.0f);
        mTexCoords.emplace_back(1.0f);
    };
    
    void FramebufferRenderer::Bind()
    {
        mFramebuffer.Bind(mFramebufferTexture);
    }
    
    void FramebufferRenderer::Unbind()
    {
        mFramebuffer.Unbind();
    }
}
