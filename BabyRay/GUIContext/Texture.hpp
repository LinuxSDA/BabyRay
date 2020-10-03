/* Texture_hpp */
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
    unsigned int mRendererId{};
    std::string mFilePath{};
    unsigned char* mLocalBuffer{};
    int mWidth{}, mHeight{}, mChannels{};
    unsigned int mFormat{};
    
public:
    Texture(const std::string& path);
    Texture(unsigned int width, unsigned int height, unsigned int channel);
    ~Texture();
    
    const std::string& GetTexturePath() const;

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
    
    inline int GetWidth() const { return mWidth;}
    inline int GetHeight() const { return mHeight;}
    inline int GetTextureID() const { return mRendererId;}
    
};
#endif /* Texture_hpp */
