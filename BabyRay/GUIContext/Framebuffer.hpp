//
//  Framebuffer.hpp
//  OpenGL
//
//  Created by Sumit Dhingra on 24/06/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#ifndef Framebuffer_hpp
#define Framebuffer_hpp

#include <vector>
#include "Texture.hpp"

/* ToDo: support bufferwidth height changing functionality */
class Framebuffer
{
public:
    Framebuffer();
    ~Framebuffer();

    void Bind(const Texture& texture);
    void Unbind();
private:
    
    unsigned int mFramebufferID{};
    unsigned int mRenderbufferID{};
    int          mViewPort[4]{};
};


#endif /* Framebuffer_hpp */
