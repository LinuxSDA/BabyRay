//
//  Shader.hpp
//  OpenGL
//
//  Created by Sumit Dhingra on 02/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <string>
#include <unordered_map>
#include <deque>
#include <map>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Texture.hpp"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
private:
    std::string mFilePath;
    unsigned int mRendererId;
    std::unordered_map<std::string, int> mUniformLocationCache;
    std::deque<Texture> mTextures;
    std::map<std::string, std::string> mTexturePathToUniform;
    
public:
    Shader(const std::string& fileName);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;

    void SetTexture(const std::string& mTexturePath, const std::string& mTextureUniform);
    void SetUniform1i(const std::string& name, int v0);
    void SetUniform1f(const std::string& name, float v0);
    void SetUniform2f(const std::string& name, float v0, float v1);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& );
    void SetUniformVec2f(const std::string& name, const std::vector<glm::vec2>& vec);

private:
    ShaderProgramSource ParseShader(const std::string& filePath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

    /* To be called for binding texture*/
    void PrepareTexture() const;
    void SetUniform1i(const std::string& name, int v0) const;
    int GetUniformLocation(const std::string& name) const;
    int GetUniformLocation(const std::string& name);
    int SetUniformLocation(const std::string& name);
};
#endif /* Shader_hpp */
