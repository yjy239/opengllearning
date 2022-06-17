//
//  CameraRenderEngine.cpp
//  Camera
//
//  Created by 余均宇 on 2019/11/18.
//  Copyright © 2019年 余均宇. All rights reserved.
//

#include "CameraRenderEngine.hpp"

Camera _camera(glm::vec3(0.0f, 0.0f, 3.0f));
//上一次的鼠标的位置
bool _firstMouse = true;
float _yaw   = -90.0f;
// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float _pitch =  0.0f;
float _lastX =  800.0f / 2.0;
float _lastY =  600.0 / 2.0;
float _deltaTime = 0.0f; // 当前帧与上一帧的时间差
float _lastFrame = 0.0f; // 上一帧的时间
glm::vec3 _cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 _cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 _cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float _fov   =  45.0f;


void _mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(_firstMouse){
        _lastX = xpos;
        _lastY = ypos;
        _firstMouse = false;
    }
    float xoffset = xpos - _lastX;
    float yoffset = ypos - _lastY;
    
    _lastX = xpos;
    _lastY = ypos;
    
    _camera.ProcessMouseMovement(xoffset, yoffset,true);
    
}

void _scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    
    _camera.ProcessMouseScroll(yoffset);
}


void _processInput(GLFWwindow *window){
    float cameraSpeed = 2.5f * _deltaTime;//不让移动变化过大
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        //往前走
        //cameraPos += cameraSpeed * cameraFront;
        _camera.ProcessKeyboard(FORWARD, _deltaTime);
    }else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        //往左走
        //通过叉乘计算法向量，刚好就是左右方向
        //        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        _camera.ProcessKeyboard(LEFT, _deltaTime);
    }else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        //往后走
        //        cameraPos -= cameraSpeed * cameraFront;
        _camera.ProcessKeyboard(BACKWARD, _deltaTime);
    }else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        //        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        _camera.ProcessKeyboard(RIGHT, _deltaTime);
    }
    
}

CameraRenderEngine::CameraRenderEngine(){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwSetCursorPosCallback(getWindow(), _mouse_callback);
    glfwSetScrollCallback(getWindow(), _scroll_callback);
}



void CameraRenderEngine::loop(GLuint VAO,GLuint VBO,vector<Shader> shaders,GLFWwindow *window,function<void(vector<Shader>,GLFWwindow *)> handle){
    if(!window){
        return;
    }
    int len = shaders.size();
    while(!glfwWindowShouldClose(window)){
        _processInput(window);
        //交换颜色缓冲，他是一个存储着GLFW窗口每一个像素颜色值的大缓冲
        //会在这个迭代中用来绘制，并且显示在屏幕上
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        float currentFrame = glfwGetTime();
        _deltaTime = currentFrame - _lastFrame;
        _lastFrame = currentFrame;
        
        _processInput(window);
        
        
        if(handle){
            handle(shaders,window);
        }
        
        
        
        
        glfwSwapBuffers(window);
        //检查有没有触发事件，键盘输入，鼠标移动，更新噶 u 那个口耦
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
}
