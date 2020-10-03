//
//  Shader.cpp
//  OpenGL
//
//  Created by Sumit Dhingra on 02/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#include "Shader.hpp"
#include "ErrorHandler.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "gtc/type_ptr.hpp"
#include <vector>

Shader::Shader(const std::string& filePath)
: mFilePath(filePath), mRendererId(0)
{
    ShaderProgramSource source = ParseShader(filePath);
    mRendererId  = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(mRendererId));
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);
    
    enum class ShaderType_e {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };
    
    std::string line;
    std::stringstream ss[2];
    ShaderType_e type = ShaderType_e::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType_e::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType_e::FRAGMENT;
            }
        }
        else {
            ss[static_cast<int>(type)] << line << "\n";
        }
    }
    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));

    const char* src = source.c_str();

    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));
    
    int result;

    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (GL_FALSE == result)
    {
        int length{};
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        char message[length];
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile shader " << (GL_VERTEX_SHADER == type ? "Vertex" : "Fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        
        GLCall(glDeleteShader(id));
        return 0;
    }
    
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());

    GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    
    GLCall(glDetachShader(program, vs));
    GLCall(glDetachShader(program, fs));
    
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    
    return program;
}


void Shader::Bind() const
{
    GLCall(glUseProgram(mRendererId));
    PrepareTexture();
}

void Shader::PrepareTexture() const
{
    for(int slot = 0; slot < mTextures.size(); ++slot)
    {
        mTextures[slot].Bind();
        const std::string& texPath = mTextures[slot].GetTexturePath();
        SetUniform1i(mTexturePathToUniform.at(texPath).c_str(), slot);
    }
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetTexture(const std::string& mTexturePath, const std::string& mTextureUniform)
{
    mTexturePathToUniform[mTexturePath] = mTextureUniform;
    mTextures.emplace_back(mTexturePath);
    SetUniformLocation(mTextureUniform);
}

void Shader::SetUniform1i(const std::string& name, int v0) const
{
    GLCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
    GLCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
    GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
    GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void Shader::SetUniformVec2f(const std::string& name, const std::vector<glm::vec2>& vec)
{
   GLCall(glUniform2fv(GetUniformLocation(name), static_cast<int>(vec.size()), glm::value_ptr(vec[0])));
}

int Shader::GetUniformLocation(const std::string& name) const
{
    return mUniformLocationCache.at(name);
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
        return mUniformLocationCache[name];
 
    return SetUniformLocation(name);
}

int Shader::SetUniformLocation(const std::string& name)
{
    GLCall(int location = glGetUniformLocation(mRendererId, name.c_str()));
    
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesnt exist!" << std::endl;
    
    mUniformLocationCache[name] = location;
    
    return location;
}
