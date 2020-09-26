//
//  ErrorHandler.hpp
//  OpenGL
//
//  Created by Sumit Dhingra on 02/05/20.
//  Copyright © 2020 LinuxSDA. All rights reserved.
//

#ifndef ErrorHandler_hpp
#define ErrorHandler_hpp

#include "GL/glew.h"
#include <iostream>

#define ASSERT(x) if (!(x)) __asm("int3")
#define GLCall(x) GlClearError();\
x;\
ASSERT(GlLogCall(#x, __FILE__, __LINE__))           /// # turn it to string

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);

#endif /* ErrorHandler_hpp */
