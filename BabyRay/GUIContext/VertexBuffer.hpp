//
//  VertexBuffer.hpp
//  OpenGL
//
//  Created by Sumit Dhingra on 01/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#ifndef VertexBuffer_hpp
#define VertexBuffer_hpp

#include <vector>
#include "ErrorHandler.hpp"

class VertexBuffer
{
private:
    unsigned int mRendererId;

public:
    template <typename T>
    VertexBuffer(const std::vector<T>& buffer)
    {
        GLCall(glGenBuffers(1, &mRendererId));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, mRendererId));
        GLCall(glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(T), buffer.data(), GL_STATIC_DRAW));
    }

    ~VertexBuffer();
    
    void Bind() const;
    void Unbind() const;
};
#endif /* VertexBuffer_hpp */
