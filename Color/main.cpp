//
//  main.cpp
//  Color
//
//  Created by 余均宇 on 2019/9/5.
//  Copyright © 2019年 余均宇. All rights reserved.
//

#include <iostream>
#include"glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.hpp"
#include "RenderEngine.hpp"
#include "stb_image.h"
#include "Camera.hpp"


GLuint lightVAO;

void initLight(GLuint &VAO,GLuint &VBO){
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    
    //设置物体
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);
    glEnableVertexAttribArray(0);
    
    //设置光源
    glGenVertexArrays(1,&lightVAO);
    glBindVertexArray(lightVAO);
    
    //因为只是反射，所以还是一样的物体属性
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    
    //只需要点
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);
    glEnableVertexAttribArray(0);
    
    
    
    
}

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//上一次的鼠标的位置
bool firstMouse = true;
float yaw   = -90.0f;
// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float fov   =  45.0f;


void processInput(GLFWwindow *window){
    float cameraSpeed = 2.5f * deltaTime;//不让移动变化过大
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        //往前走
        //cameraPos += cameraSpeed * cameraFront;
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        //往左走
        //通过叉乘计算法向量，刚好就是左右方向
        //        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        camera.ProcessKeyboard(LEFT, deltaTime);
    }else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        //往后走
        //        cameraPos -= cameraSpeed * cameraFront;
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        //        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset,true);
    
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    
    camera.ProcessMouseScroll(yoffset);
}

glm::vec3 lightPos(1.2f,1.0f,2.0f);

int main(int argc, const char * argv[]) {
    // insert code here...
    static RenderEngine *engine = new RenderEngine();
    const char* model =
    "/Users/yjy/Desktop/iOSworkspace/first_opengl/Color/model.glsl";
    const char* cl =
    "/Users/yjy/Desktop/iOSworkspace/first_opengl/Color/color_fragment.glsl";
    const char* lamp =
    "/Users/yjy/Desktop/iOSworkspace/first_opengl/Color/lamp.glsl";
    Shader objShader(model,cl);
    Shader lightShader(model,lamp);
    
    objShader.compile();
    lightShader.compile();
    
    static GLuint VAO;
    static GLuint VBO;
    
    initLight(VAO, VBO);
    
    vector<Shader> shaders;
    shaders.push_back(objShader);
    shaders.push_back(lightShader);
    
    glfwSetInputMode(engine->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(engine->window, mouse_callback);
    glfwSetScrollCallback(engine->window, scroll_callback);
    
    engine->loop(VAO, VBO, shaders,engine->window,[](vector<Shader> shaders,
                                                     GLFWwindow* window){
        //设置物体的颜色以及接受光源的颜色
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        
        shaders[0].use();
    
        shaders[0].setVec3("objColor", 1.0f, 0.5f, 0.31f);
        shaders[0].setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        
        glm::mat4 model = glm::mat4(1.0f);
        shaders[0].setMat4("model", glm::value_ptr(model));
        glm::mat4 pro = glm::perspective(glm::radians(camera.Zoom), (float)(engine->screenWidth/engine->screenHeight), 0.1f, 100.0f);
        shaders[0].setMat4("projection", glm::value_ptr(pro));
        
        shaders[0].setMat4("view", glm::value_ptr(camera.GetViewMatrix()));
        
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        //设置光源
        
        shaders[1].use();
        //缩小
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        
        
        
        shaders[1].setMat4("model", glm::value_ptr(model));
        shaders[1].setMat4("projection", glm::value_ptr(pro));
        shaders[1].setMat4("view", glm::value_ptr(camera.GetViewMatrix()));
        
        
        
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
    });
    
    
    
    
    
    return 0;
}
