//
//  IndexBuffer.hpp
//  OpenGL
//
//  Created by Sumit Dhingra on 01/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#ifndef IndexBuffer_hpp
#define IndexBuffer_hpp

#include <vector>

class IndexBuffer
{
private:
    unsigned int mRendererId;
    unsigned int mCount;
public:
    IndexBuffer(const std::vector<unsigned int>& data);
    ~IndexBuffer();
    
    void Bind() const;
    void Unbind() const;
    inline unsigned int GetCount() const {return mCount;}
};

#endif /* IndexBuffer_hpp */
