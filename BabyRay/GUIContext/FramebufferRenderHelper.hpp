//
//  FramebufferRenderHelper.hpp
//  OpenGL
//
//  Created by Sumit Dhingra on 19/07/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#ifndef FramebufferRenderHelper_hpp
#define FramebufferRenderHelper_hpp

#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Renderer.hpp"
#include "Framebuffer.hpp"
#include "Texture.hpp"

namespace Helper
{
    class FramebufferRenderer
    {
    public:
        FramebufferRenderer(int ScreenWidth, int ScreenHeight);
        ~FramebufferRenderer();
        void Draw(const Renderer& renderer, const Shader& shader) const;
        void Bind();
        void Unbind();
    private:
        void InitBuffers();
        std::vector<float> mScreenVertices;
        std::vector<float> mTexCoords;
        std::vector<unsigned int> mIndices;
        Framebuffer mFramebuffer;
        Texture     mFramebufferTexture;
        VertexArray mQuadVA;
    };
}
#endif /* FramebufferRenderHelper_hpp */
