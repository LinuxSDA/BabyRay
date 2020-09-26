//
//  main.cpp
//  JustGUI
//
//  Created by SD on 23/09/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#include "GUIContext.hpp"
#include "gtc/matrix_transform.hpp"



int main(void)
{
    const int ScreenWidth = 720;
    const int ScreenHeight = 720;
    const std::string WindowName = "Baby Ray";
    
    GLFWInitWindow window(ScreenWidth, ScreenHeight, WindowName);

    std::vector<float> positions {  -1.0,  1.0,
                                     1.0,  1.0,
                                     1.0, -1.0,
                                    -1.0, -1.0 };
    
    std::vector<unsigned int> indices { 0, 1, 2,
                                        0, 2, 3 };

    
    VertexArray va;
    va.CreateVBuffer2f(positions);
    va.CreateIBuffer(indices);
    
    Shader shader("/Users/sd/Documents/GitHub/BabyRay/Shaders/Ray.shader");
    shader.Bind();
    
    Renderer renderer;
    
    /* Loop until the user closes the window */
    while (!(window.ShouldCloseWindow()))
    {
        renderer.Clear();

        auto cursor = window.GetNormalizedCursorPosition();
        shader.SetUniform2f("u_LightPos", cursor.first, cursor.second);
        
        renderer.Draw(va, shader);
        window.SwapBuffersAndPollEvents();
    }
    
    return 0;
}

        
