//
//  Texture.hpp
//  OpenGL
//
//  Created by Sumit Dhingra on 02/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <string>

class Texture
{
private:
    unsigned int mRendererId;
    std::string mFilePath;
    unsigned char* mLocalBuffer;
    int mWidth,mHeight, mChannels;
    
public:
    Texture(const std::string& path);
    ~Texture();
    
    const std::string& GetTexturePath() const;

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
    
    inline int GetWidth() { return mWidth;}
    inline int GetHeight() { return mHeight;}
    
};
#endif /* Texture_hpp */
