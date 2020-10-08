//
//  GLRenderScene.cpp
//  JustGUI
//
//  Created by SD on 23/09/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#include "GUIContext/GUIContext.hpp"
#include "gtc/matrix_transform.hpp"
#include "FramebufferRenderHelper.hpp"

void DumpFrameRateInfo()
{
    static double lastTime = glfwGetTime();
    static int nbFrames = 0;

    // Measure speed
    double currentTime = glfwGetTime();
    nbFrames++;
    if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
        // printf and reset timer
        printf("%i fps \n", nbFrames);
        nbFrames = 0;
        lastTime += 1.0;
    }
}

int main(void)
{
    const int ScreenWidth = 480;
    const int ScreenHeight = 480;
    const std::string WindowName = "Baby Ray";
    
    GLFWInitWindow window(ScreenWidth, ScreenHeight, WindowName);
    window.HideCursor();
    
    std::vector<float> positions {  -1.0,  1.0,
                                     1.0,  1.0,
                                     1.0, -1.0,
                                    -1.0, -1.0 };
    
    std::vector<unsigned int> indices { 0, 1, 2,
                                        0, 2, 3 };

    std::vector<float> uv { 0.0f, 0.0f,
                            1.0f, 0.0f,
                            1.0f, 1.0f,
                            0.0f, 1.0f };
                            
        
    VertexArray va;
    va.CreateVBuffer2f(positions);
    va.CreateIBuffer(indices);
    va.CreateVBuffer2f(uv);
    
    Shader shader("/Users/sd/Documents/GitHub/BabyRay/BabyRay/Shaders/Ray.shader");
    shader.SetTexture("/Users/sd/Documents/GitHub/BabyRay/BabyRay/Resources/Stone.jpg", "u_DiffuseTexture");
    
    Renderer renderer;
    
    /* Loop until the user closes the window */
    while (!(window.ShouldCloseWindow()))
    {
        renderer.Clear();
        DumpFrameRateInfo();

        shader.Bind();
        auto cursor = window.GetNormalizedCursorPosition();
        shader.SetUniform2f("u_LightPos", cursor.first, -cursor.second);
        renderer.Draw(va, shader);

        window.SwapBuffersAndPollEvents();
    }
    
    return 0;
}

