//
//  CameraRenderEngine.hpp
//  Camera
//
//  Created by 余均宇 on 2019/11/18.
//  Copyright © 2019年 余均宇. All rights reserved.
//

#ifndef CameraRenderEngine_hpp
#define CameraRenderEngine_hpp

#include <stdio.h>
#include "RenderEngine.hpp"
#include "glm.hpp"
#include "Camera.hpp"

class CameraRenderEngine : public RenderEngine{
    
public:
    CameraRenderEngine();
    ~CameraRenderEngine();
    void loop(GLuint VAO,GLuint VBO,vector<Shader> shaders,GLFWwindow *window,function<void(vector<Shader>,GLFWwindow *)> handle);


    
};

#endif /* CameraRenderEngine_hpp */
