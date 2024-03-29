//
//  GUIContext.hpp
//  2DRayTracer
//
//  Created by SD on 23/09/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include <string>
#include <vector>

class GLFWInitWindow
{
public:
    GLFWInitWindow(const int ScreenWidth, const int ScreenHeight, const std::string& WindowName): mScreenWidth(ScreenWidth), mScreenHeight(ScreenHeight), mWindowName(WindowName)
    {
        /* Initialize the library */
        if (!glfwInit())
            throw std::runtime_error("Can't init GLFW!");
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        

        /* Create a windowed mode window and its OpenGL context */
        mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, mWindowName.c_str(), NULL, NULL);
        if (!mWindow)
        {
            glfwTerminate();
            throw std::runtime_error("Can't init GLFW Window!");
        }
        
        /* Make the window's context current */
        glfwMakeContextCurrent(mWindow);
        glfwSwapInterval(1);
        
        GLenum err = glewInit();
        if (GLEW_OK != err)
            std::cout << "Glew Not Okay" << std::endl;

        std::cout << "OpenGL Version: => " << glGetString(GL_VERSION) << std::endl;
    }
    
    
    bool ShouldCloseWindow()
    {
        return glfwWindowShouldClose(mWindow);
    }
    
    std::pair<double, double> GetCursorPosition()
    {
        double xpos;
        double ypos;

        glfwGetCursorPos(mWindow, &xpos, &ypos);
        return std::make_pair(xpos, ypos);
    }

    std::pair<double, double> GetNormalizedCursorPosition()
    {
        auto cursor = GetCursorPosition();
        cursor.first = ((cursor.first/mScreenWidth) * 2) - 1;
        cursor.second = ((cursor.second/mScreenHeight) * 2) - 1;
        
        auto clamp = [](double value){
            if (value > 1)
                value = 1;
            else if (value < -1)
                value = -1;
            
            return value;
        };
        
        return std::make_pair(clamp(cursor.first), clamp(cursor.second));
    }
    
    void HideCursor()
    {
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    
    void SwapBuffersAndPollEvents()
    {
        /* Swap front and back buffers */
        glfwSwapBuffers(mWindow);
        
        /* Poll for and process events */
        glfwPollEvents();
    }

    ~GLFWInitWindow()
    {
        glfwTerminate();
    }

private:
    int mScreenWidth = 1280;
    int mScreenHeight = 720;
    std::string mWindowName = "OpenGL Window";
    GLFWwindow* mWindow;
};
