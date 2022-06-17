//
//  main.cpp
//  3D
//
//  Created by 余均宇 on 2019/8/17.
//  Copyright © 2019年 余均宇. All rights reserved.
//

#include <iostream>

#include"glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.hpp"
#include "RenderEngine.hpp"
#include "stb_image.h"

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
        // 右上角                       //纹理
        0.5f, 0.5f, 0.0f,   1.0f,1.0f,
        // 右下角
        0.5f, -0.5f, 0.0f,   1.0f,0.0f,
        // 左下角
        -0.5f, -0.5f, 0.0f,   0.0f,0.0f,
        // 左上角
        -0.5f, 0.5f, 0.0f,  0.0f,1.0f
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
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,5* sizeof(float),(void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
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

void showOneCube(Shader *shader){
    //模型矩阵
    //物体坐标变换到世界坐标
    //把它从局部坐标，摆到世界中，是沿着x轴旋转-55度
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.5f,1.0f,0.0f));
    //观察矩阵
    //观察的位置的移动
    //从世界坐标到观察空间
    //模拟我们摄像机，沿着z轴向后移3个单位
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0,0.0,-3.0f));
    
    //投影矩阵
    //观察到裁剪空间
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f),
                                  (width/height), 0.1f, 100.0f);
    
    //            GLuint modelLoc = glGetUniformLocation(shader->ID,"model");
    //            glUniformMatrix4fv(modelLoc,1,GL_FALSE,&model[0][0]);
    shader->setMat4("model", &model[0][0]);
    
    //            GLuint viewLoc = glGetUniformLocation(shader->ID,"view");
    //            glUniformMatrix4fv(viewLoc,1,GL_FALSE,&view[0][0]);
    
    shader->setMat4("view", &view[0][0]);
    
    shader->setMat4("projection", &projection[0][0]);
    
    //            GLuint projectionLoc = glGetUniformLocation(shader->ID,"projection");
    //            glUniformMatrix4fv(projectionLoc,1,GL_FALSE,&projection[0][0]);
}


void showMoreCube(Shader *shader){
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    
    view = glm::translate(view, glm::vec3(0.0,0.0,-3.0f));
    projection = glm::perspective(glm::radians(45.0f),
                                  (width/height), 0.1f, 100.0f);
    
    shader->setMat4("view", glm::value_ptr(view));
    shader->setMat4("projection", glm::value_ptr(projection));
    
    for(int i = 0;i<10;i++){
        glm::mat4 model = glm::mat4(1.0f);
        
        
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        if(i % 3 == 0){
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f,3.0f,0.5f));
        }else{
            model = glm::rotate(model, angle, glm::vec3(1.0f,3.0f,0.5f));
        }
        
        shader->setMat4("model", glm::value_ptr(model));
        
        glDrawArrays(GL_TRIANGLES,0,36);
    }
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

        engine->loop(VAO, VBO, texture, 1,shader, [](Shader* shader,GLuint VAO,
                                                     GLuint* texture,GLFWwindow *window){


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

    
//    if(shader&&shader->isCompileSuccess()){
//
//        shader->use();
//        //设置的是纹理单元
//        glUniform1i(glGetUniformLocation(shader->ID,"ourTexture"),0);
//        shader->setInt("ourTexture2", 1);
//
//        //模型矩阵
//        //物体坐标变换到世界坐标
//        //把它从局部坐标，摆到世界中，是沿着x轴旋转-55度
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f,0.0f,0.0f));
//        //观察矩阵
//        //观察的位置的移动
//        //从世界坐标到观察空间
//        //模拟我们摄像机，沿着z轴向后移3个单位
//        glm::mat4 view = glm::mat4(1.0f);
//        view = glm::translate(view, glm::vec3(0.0,0.0,-3.0f));
//
//        //投影矩阵
//        //观察到裁剪空间
//        glm::mat4 projection = glm::mat4(1.0f);
//        projection = glm::perspective(glm::radians(45.0f),
//                                      ((float)engine->screenWidth)/((float)engine->screenHeight), 0.1f, 100.0f);
//
//        GLuint modelLoc = glGetUniformLocation(shader->ID,"model");
//        glUniformMatrix4fv(modelLoc,1,GL_FALSE,&model[0][0]);
//
//        GLuint viewLoc = glGetUniformLocation(shader->ID,"view");
//        glUniformMatrix4fv(viewLoc,1,GL_FALSE,&view[0][0]);
//
//        GLuint projectionLoc = glGetUniformLocation(shader->ID,"projection");
//        glUniformMatrix4fv(projectionLoc,1,GL_FALSE,&projection[0][0]);
//
//
//        engine->loop(VAO, VBO, texture, 1,shader, [](Shader* shader,GLuint VAO,
//                                                     GLuint* texture,GLFWwindow *window){
//
//            //箱子
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D,texture[0]);
//
//            //笑脸
//            glActiveTexture(GL_TEXTURE1);
//            glBindTexture(GL_TEXTURE_2D,texture[1]);
//            glBindVertexArray(VAO);
//            glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//
//
//        });
//    }
    return 0;
}
