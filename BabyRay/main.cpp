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


/* TODO: Shader: I need to divide this by original texture to get only light*/

int main(void)
{
    const int ScreenWidth = 720;
    const int ScreenHeight = 720;
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
    shader.Bind();
    shader.SetTexture("/Users/sd/Documents/GitHub/BabyRay/BabyRay/Resources/wood.png", "u_DiffuseTexture");

    Helper::FramebufferRenderer framebuffer(ScreenWidth, ScreenHeight);
    Shader framebufferShader("/Users/sd/Documents/GitHub/BabyRay/BabyRay/Shaders/Framebuffer.shader");


//    Helper::FramebufferRenderer framebufferBlur(ScreenWidth, ScreenHeight);
//    Shader framebufferBlurShader("/Users/sd/Documents/GitHub/BabyRay/BabyRay/Shaders/Blur.shader");

    Renderer renderer;
    
    /* Loop until the user closes the window */
    while (!(window.ShouldCloseWindow()))
    {
        renderer.Clear();

        framebuffer.Bind();
        {
            shader.Bind();
            auto cursor = window.GetNormalizedCursorPosition();
            shader.SetUniform2f("u_LightPos", cursor.first, -cursor.second);
            renderer.Draw(va, shader);
        }
        framebuffer.Unbind();

//        framebufferBlur.Bind();
        framebuffer.Draw(renderer, framebufferShader);
//        framebufferBlur.Unbind();
//        framebufferBlur.Draw(renderer, framebufferBlurShader);
        
        window.SwapBuffersAndPollEvents();
    }
    
    return 0;
}

