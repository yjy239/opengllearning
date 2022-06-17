//
//  RenderEngine.hpp
//  glsl
//
//  Created by 余均宇 on 2019/7/24.
//  Copyright © 2019年 余均宇. All rights reserved.
//

#ifndef RenderEngine_hpp
#define RenderEngine_hpp
#include <iostream>
#include <stdio.h>
#include "glad.h"
#include <GLUT/GLUT.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include <functional>

class RenderEngine{
        
    
public:
    GLFWwindow *window;
    RenderEngine();
    ~RenderEngine();
    int screenWidth = 0;
    int screenHeight = 0;
    void loop(GLuint VAO,GLuint VBO,Shader *shader,function<void(Shader*)> handle);
    
    void loop(GLuint VAO,GLuint VBO,GLuint* texture, int texture_count, Shader *shader,function<void(Shader*,GLuint VAO,GLuint* texture,GLFWwindow *)> handle);
    
    
    void loop(GLuint VAO,GLuint VBO,vector<Shader> shaders,GLFWwindow *window,function<void(vector<Shader>,GLFWwindow *)> handle);
    
    GLFWwindow* getWindow();
    
    
};



#endif /* RenderEngine_hpp */
