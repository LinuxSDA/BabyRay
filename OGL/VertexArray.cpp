//
//  VertexArray.cpp
//  OpenGL
//
//  Created by Sumit Dhingra on 01/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#include "VertexArray.hpp"
#include "Renderer.hpp"

VertexArray::VertexArray() : mIndex(0)
{
    GLCall(glGenVertexArrays(1, &mRendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &mRendererID));
}


void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(mRendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    
    const auto& elements = layout.GetElement();
    unsigned int offset = 0;
    
    for(int index = 0; index < elements.size(); index++)
    {
        const auto& element = elements[index];
        
        GLCall(glEnableVertexAttribArray(mIndex + index));
        GLCall(glVertexAttribPointer(mIndex + index, element.mCount, element.mType, element.mNormalised, layout.GetStride(), reinterpret_cast<const void*>(offset)));
        
        offset += element.mCount * VertexBufferElement::GetSizeOfType(element.mType);
    }

    mIndex += elements.size();
}

unsigned int VertexArray::CreateVBuffer2f(const std::vector<float>& buffer)
{
    vbArray.emplace(buffer);
    VertexBufferLayout layout;
    layout.Push<float>(2);
    AddBuffer(vbArray.back(), layout);
    
    return mIndex - 1;
}

unsigned int VertexArray::CreateVBuffer3f(const std::vector<float>& buffer)
{
    vbArray.emplace(buffer);
    VertexBufferLayout layout;
    layout.Push<float>(3);
    AddBuffer(vbArray.back(), layout);
    
    return mIndex - 1;
}

unsigned int VertexArray::CreateVBuffer4f(const std::vector<float>& buffer)
{
    vbArray.emplace(buffer);
    VertexBufferLayout layout;
    layout.Push<float>(4);
    AddBuffer(vbArray.back(), layout);
    
    return mIndex - 1;
}

void VertexArray::CreateIBuffer(const std::vector<unsigned int>& buffer)
{
    ib = std::make_unique<IndexBuffer>(buffer);
}

unsigned int VertexArray::GetIndicesCount() const
{
    return ib->GetCount();
}
