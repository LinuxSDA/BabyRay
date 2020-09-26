//
//  VertexArray.hpp
//  OpenGL
//
//  Created by Sumit Dhingra on 01/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#ifndef VertexArray_hpp
#define VertexArray_hpp

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include <queue>

class VertexArray
{
    unsigned int mRendererID;
    unsigned int mIndex;

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    std::queue<VertexBuffer> vbArray;
    std::unique_ptr<IndexBuffer> ib;
    
public:
    VertexArray();
    ~VertexArray();

    unsigned int CreateVBuffer2f(const std::vector<float>& buffer);
    unsigned int CreateVBuffer3f(const std::vector<float>& buffer);
    unsigned int CreateVBuffer4f(const std::vector<float>& buffer);

    using StartEndIndex = std::pair<unsigned int, unsigned int>;
    
    template <typename T>
    StartEndIndex CreateVBufferf(const std::vector<T>& buffer, const VertexBufferLayout& layout)
    {
        StartEndIndex pair;
        pair.first = mIndex;

        vbArray.emplace(buffer);
        AddBuffer(vbArray.back(), layout);
        
        pair.second = mIndex;
        
        return pair;
    }

    void CreateIBuffer(const std::vector<unsigned int>& buffer);

    void Bind() const;
    void Unbind() const;

    unsigned int GetIndicesCount() const;
};
#endif /* VertexArray_hpp */
