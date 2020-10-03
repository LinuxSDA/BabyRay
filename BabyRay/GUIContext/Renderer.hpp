//
//  Renderer.hpp
//  OpenGL
//
//  Created by Sumit Dhingra on 01/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer
{
public:
    void Clear() const;
    void EnableDepth(GLenum depthType) const;
    void DisableDepth() const;
    void EnableBlend() const;
    void Draw(const VertexArray& va, const Shader& shader ) const;
};
#endif /* Renderer_hpp */
