//
//  main.cpp
//  Mat
//
//  Created by 余均宇 on 2019/8/11.
//  Copyright © 2019年 余均宇. All rights reserved.
//

#include <iostream>
#include"glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.hpp"
#include "RenderEngine.hpp"
#include "stb_image.h"

#define PI 3.14159265


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

//要画矩形，因此要两个三角形
void flushRetriangle(GLuint& VAO,GLuint& VBO,GLuint& EBO){
    float vertices[] = {
        //位置
        // 右上角          //颜色              //纹理
        0.5f, 0.5f, 0.0f,   1.0f,0.0f,0.0f,  1.0f,1.0f,
        // 右下角
        0.5f, -0.5f, 0.0f,  0.0f,1.0f,0.0f,  1.0f,0.0f,
        // 左下角
        -0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f,  0.0f,0.0f,
        // 左上角
        -0.5f, 0.5f, 0.0f,  1.0f,1.0f,0.0f,  0.0f,1.0f
    };
    
    unsigned int indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    
    //分配VAO
    glGenVertexArrays(1,&VAO);
    //分配VBO
    glGenBuffers(1,&VBO);
    //分配EBO
    glGenBuffers(1,&EBO);
    
    //绑定
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    
    //设置缓存数据
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    //索引
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    
    //告诉OpenGL怎么读取数据
    //分成三次来读取
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8* sizeof(float),(void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8* sizeof(float),(void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    
    
    //解绑
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //glBindTexture(GL_TEXTURE_2D,0);
    glBindVertexArray(0);
    
    
}

void scaleMat4(GLfloat dst[4][4],GLfloat src[4][4],GLfloat* vec){
    
    dst[0][0] = src[0][0] * vec[0];
    dst[1][1] = src[1][1] * vec[1];
    dst[2][2] = src[2][2] * vec[3];
    dst[3][3] = src[3][3];

}

void rotationZ(GLfloat dst[4][4],GLfloat src[4][4],double degree){
    double angle = degree * static_cast<double>(0.01745329251994329576923690768489);
    
    dst[0][0] = src[0][0]*cos(angle) + src[1][0]*sin(angle);
    dst[0][1] = src[0][1]*cos(angle) + src[1][1]*sin(angle);
    dst[0][2] = src[0][2]*cos(angle) + src[1][2]*sin(angle);
    dst[0][3] = src[0][3]*cos(angle) + src[1][3]*sin(angle);
    
    
    dst[1][0] = src[0][0]*sin(angle)  + src[1][0]*cos(angle);
    dst[1][1] = src[0][1]*sin(angle)  + src[1][1]*cos(angle);
    dst[1][2] = src[0][2]*sin(angle)  + src[1][2]*cos(angle);
    dst[1][3] = src[0][3]*sin(angle)  + src[1][3]*cos(angle);
    
    dst[2][0] = src[2][0];
    dst[2][1] = src[2][1];
    dst[2][2] = src[2][2];
    dst[2][3] = src[2][3];
    
    dst[3][0] = src[3][0];
    dst[3][1] = src[3][1];
    dst[3][2] = src[3][2];
    dst[3][3] = src[3][3];
}





int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
//    glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);
//    glm::mat4 trans = glm::mat4(1.0f);
//
//    trans = glm::translate(trans, glm::vec3(1.0f,1.0f,0.0f));
//
//    vec = trans * vec;
//
//    std::cout << vec.x <<vec.y<<vec.z<<endl;
    RenderEngine *engine = new RenderEngine();
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint texture[] = {1};
    const char* vPath = "/Users/yjy/Desktop/iOSworkspace/first_opengl/Mat/mat_vertex.glsl";
    const char* fPath = "/Users/yjy/Desktop/iOSworkspace/first_opengl/Mat/mat_fragment.glsl";
    Shader *shader = new Shader(vPath,fPath);
    shader->compile();
    flushRetriangle(VAO, VBO, EBO);
    
    printf("%d\n",sizeof(float));
    
    initTexture(texture[0], "/Users/yjy/Desktop/opengl/container.jpg",false);
    initTexture(texture[1], "/Users/yjy/Desktop/opengl/awesomeface.png",true);
    
    if(shader&&shader->isCompileSuccess()){
        
        shader->use();
        //设置的是纹理单元
        glUniform1i(glGetUniformLocation(shader->ID,"ourTexture"),0);
        shader->setInt("ourTexture2", 1);
        
        GLfloat mat4[4][4] = {
            {1.0f,0.0f,0.0f,0.0f},
            {0.0f,1.0f,0.0f,0.0f},
            {0.0f,0.0f,1.0f,0.0f},
            {0.0f,0.0f,0.0f,1.0f}
        };
        
        GLfloat result[4][4] = {
            {1.0f,0.0f,0.0f,0.0f},
            {0.0f,1.0f,0.0f,0.0f},
            {0.0f,0.0f,1.0f,0.0f},
            {0.0f,0.0f,0.0f,1.0f}
        };
        
        GLfloat vec3[] = {
            0.5f,0.5f,1.0f
            
        };
        
        GLfloat dst[4][4] = {
            {1.0f,0.0f,0.0f,0.0f},
            {0.0f,1.0f,0.0f,0.0f},
            {0.0f,0.0f,1.0f,0.0f},
            {0.0f,0.0f,0.0f,1.0f}
        };
        
//        glm::mat4 trans = glm::mat4(1.0f);
//        trans = glm::rotate(trans, glm::radians(90.0f),glm::vec3(0.0,0.0,1.0));
//        trans = glm::scale(trans, glm::vec3(0.5f,0.5f,1.0f));
        
//        scaleMat4(result,mat4, vec3);
//        rotationZ(dst,result,90.0);

//        GLuint transformLoc = glGetUniformLocation(shader->ID,"transform");
//        glUniformMatrix4fv(transformLoc,1,GL_FALSE,&trans[0][0]);
        
        
        
        engine->loop(VAO, VBO, texture, 1,shader, [](Shader* shader,GLuint VAO,
                                                     GLuint* texture,GLFWwindow *window){
            
            //            changeMixValue(window);
            //            shader->setFloat("mixValue", mixValue);
            
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(0.5f,-0.5f,0.0f));
            trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f,0.0f,1.0f));
            
            
            
            GLuint transformLoc = glGetUniformLocation(shader->ID,"transform");
            glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans));
            
            //箱子
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,texture[0]);
            
            //笑脸
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D,texture[1]);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
            
            trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(-0.5f,0.5f,0.0f));
            float scale = sin(glfwGetTime());
            trans = glm::scale(trans, glm::vec3(scale,scale,scale));
            
            glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans));
            glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
            
        });
    }
    
    return 0;
}
