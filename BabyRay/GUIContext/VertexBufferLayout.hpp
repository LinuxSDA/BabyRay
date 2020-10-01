//
//  VertexBufferLayout.hpp
//  OpenGL
//
//  Created by Sumit Dhingra on 01/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#ifndef VertexBufferLayout_hpp
#define VertexBufferLayout_hpp

#include "GL/glew.h"
#include <vector>

struct VertexBufferElement
{
    unsigned int mType;
    unsigned int mCount;
    unsigned int mNormalised;
    
    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch(type)
        {
            case GL_FLOAT:          return sizeof(GLfloat);
            case GL_UNSIGNED_INT:   return sizeof(GLuint);
            case GL_UNSIGNED_BYTE:   return sizeof(GLbyte);
        }
        
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> mElements;
    unsigned int mStride;
public:
    VertexBufferLayout() : mStride(0){}
    
    template<typename T>
    void Push(unsigned int count) = delete;

    template<>
    void Push<float>(unsigned int count)
    {
        mElements.push_back({GL_FLOAT, count, GL_FALSE});
        mStride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }
    
    template<>
    void Push<unsigned int>(unsigned int count)
    {
        mElements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        mStride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int count)
    {
        mElements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        mStride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<VertexBufferElement> GetElement() const
    {
        return mElements;
    }
    
    inline unsigned int GetStride() const
    {
        return mStride;
    }
};
#endif /* VertexBufferLayout_hpp */
