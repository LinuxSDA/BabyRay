//
//  ErrorHandler.cpp
//  OpenGL
//
//  Created by Sumit Dhingra on 02/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#include "ErrorHandler.hpp"
#include <iostream>
void GlClearError() {
    while (GL_NO_ERROR != glGetError());
}

bool GlLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << std::hex << "[OpenGL Error] (" << error << "): " << function << " " << file << " : " << std::dec << line << std::endl;
        return false;
    }
    return true;
}
