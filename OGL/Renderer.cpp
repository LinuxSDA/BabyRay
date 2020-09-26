//
//  Renderer.cpp
//  OpenGL
//
//  Created by Sumit Dhingra on 01/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#include "Renderer.hpp"

void Renderer::Draw(const VertexArray& va, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ASSERT(va.GetIndicesCount() != 0);
    
    GLCall(glDrawElements(GL_TRIANGLES, va.GetIndicesCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::EnableDepth(GLenum depthType) const
{
    // Enable depth test
    GLCall(glEnable(GL_DEPTH_TEST));
    // Accept fragment if it closer to the camera than the former one
    GLCall(glDepthFunc(depthType));
}

void Renderer::EnableBlend() const
{
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
