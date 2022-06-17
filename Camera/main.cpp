//
//  main.cpp
//  Camera
//
//  Created by 余均宇 on 2019/9/3.
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

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


void create3D(GLuint& VAO,GLuint& VBO){
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //glBindVertexArray(0);
    
}

float width = 0;
float height = 0;
//世界坐标
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float fov   =  45.0f;



void showMoreCube(Shader *shader){
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    float radius = 10.0f;
    
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime())*radius;
    //位置，目标，上向量
//    view = glm::lookAt(cameraPos,
//                       cameraPos+cameraFront,
//                       cameraUp);
//    projection = glm::perspective(glm::radians(fov),
//                                  (width/height), 0.1f, 100.0f);
    
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(camera.Zoom), (width/height),
                                  0.1f, 100.0f);
    
    shader->setMat4("view", glm::value_ptr(view));
    shader->setMat4("projection", glm::value_ptr(projection));
    
    for(int i = 0;i<10;i++){
        glm::mat4 model = glm::mat4(1.0f);
        
        
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
//        if(i % 3 == 0){
//            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f,3.0f,0.5f));
//        }else{
            model = glm::rotate(model, angle, glm::vec3(1.0f,3.0f,0.5f));
//        }
        
        shader->setMat4("model", glm::value_ptr(model));
        
        glDrawArrays(GL_TRIANGLES,0,36);
    }
}

bool initTexture(GLuint& texture,const char* str,bool isResver){
    //设置纹理信息
    //生成和绑定
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    //绑定当前的纹理对象，设置环绕，过滤的方式
    //设置S轴和T轴，环绕方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    
    //    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //设置纹理过滤
    //缩小时候
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    //切换级别
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    //加载图片生成wenli数据
    int width ,height,nrChannels;
    stbi_set_flip_vertically_on_load(isResver);
    GLubyte *data = stbi_load(str, &width, &height, &nrChannels, 0);
    
    if(data){
        if(nrChannels == 3){
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        }else if(nrChannels == 4){
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        }
        
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
        return false;
    }
    
    stbi_image_free(data);
    
    return true;
}




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
//上一次的鼠标的位置
bool firstMouse = true;
float yaw   = -90.0f;
// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;


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

//    float sensitivity = 0.05f;
//    xoffset *= sensitivity;
//    yoffset *=sensitivity;
    
    

//    yaw += xoffset;
//    pitch += yoffset;
//
//    if(pitch > 89.0f){
//        pitch = 89.0f;
//    }
//
//    if(pitch < -89.0f){
//        pitch = -89.0f;
//    }
//
//    glm::vec3 front(1.0);
//    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//    front.y = sin(glm::radians(pitch));
//    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
//
//    cameraFront = glm::normalize(front);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
//    if(fov >= 1.0f && fov <= 45.0f)
//        fov -= yoffset;
//    if(fov <= 1.0f)
//        fov = 1.0f;
//    if(fov >= 45.0f)
//        fov = 45.0f;
    
    camera.ProcessMouseScroll(yoffset);
}



int main(int argc, const char * argv[]) {
    // insert code here...
    RenderEngine *engine = new RenderEngine();
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint texture[] = {1};
    const char* vPath = "/Users/yjy/Desktop/iOSworkspace/first_opengl/3D/3d_vertex.glsl";
    const char* fPath = "/Users/yjy/Desktop/iOSworkspace/first_opengl/3D/fragment.glsl";
    Shader *shader = new Shader(vPath,fPath);
    shader->compile();
    //flushRetriangle(VAO, VBO, EBO);
    
    create3D(VAO,VBO);
    
    
    
    
    
    initTexture(texture[0], "/Users/yjy/Desktop/opengl/container.jpg",false);
    initTexture(texture[1], "/Users/yjy/Desktop/opengl/awesomeface.png",true);
    
    
    
    if(shader&&shader->isCompileSuccess()){
        
        shader->use();
        //设置的是纹理单元
        glUniform1i(glGetUniformLocation(shader->ID,"ourTexture"),0);
        shader->setInt("ourTexture2", 1);
        
        
        width = (float)engine->screenWidth;
        height = (float)engine->screenHeight;
        
        glEnable(GL_DEPTH_TEST);
        
        glfwSetInputMode(engine->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(engine->window, mouse_callback);
        glfwSetScrollCallback(engine->window, scroll_callback);
        
        engine->loop(VAO, VBO, texture, 1,shader, [](Shader* shader,GLuint VAO,
                                                     GLuint* texture,GLFWwindow *window){
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            
            processInput(window);
            //箱子
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,texture[0]);
            
            //笑脸
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D,texture[1]);
            
            
            
            //showOneCube(shader);
            
            showMoreCube(shader);
            
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
            
        });
    }
    
    
    return 0;
}
