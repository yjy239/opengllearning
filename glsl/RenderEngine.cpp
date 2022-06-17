//
//  RenderEngine.cpp
//  glsl
//
//  Created by 余均宇 on 2019/7/24.
//  Copyright © 2019年 余均宇. All rights reserved.
//

#include "RenderEngine.hpp"
#include <vector>

using namespace std;

void render_framebuffer_size_callback(GLFWwindow *window,int width,int height){
    glViewport(0,0,width,height);
}



void render_processInput(GLFWwindow *window){
    //glfwGetKey确认这个窗口有没有处理按键
    //GLFW_KEY_ESCAPE 代表esc按键
    //GLFW_PRESS代表按下 GLFW_RELEASE 代表没按下
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        //关闭窗口
        glfwSetWindowShouldClose(window, true);
    }
}


RenderEngine::RenderEngine(){
    //初始化GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //创建一个800*600的窗口
    screenWidth = 800;
    screenHeight = 600;
    window = glfwCreateWindow(screenWidth, screenHeight, "Learn opengl", NULL, NULL);
    if(!window){
        cout <<"fail open window"<<endl;
        glfwTerminate();
    }
    
    //把这个窗口作为当前线程主要上下文
    glfwMakeContextCurrent(window);
    //GLAD是用来管理OpenGL的函数指针的,需要初始化
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout<< "failed to init glad" <<endl;
    }
    
    //-------------------
    //我们还要告诉opengl渲染的窗口大小
    //渲染可以比window小，这样就只会在window内部一个小窗口渲染
    //    glViewport(0,0,800,600);
    //    glFinish();
    //实际上opengl的坐标范围从-1到1，会映射到（800，600）
    
    //当拉动窗口调整大小
    glfwSetFramebufferSizeCallback(window, render_framebuffer_size_callback);
}


RenderEngine::~RenderEngine(){
    
}

GLFWwindow* RenderEngine::getWindow(){
    return window;
}


void RenderEngine::loop(GLuint VAO, GLuint VBO, GLuint* texture,int texture_count, Shader *shader, function<void (Shader *,GLuint VAO,GLuint* texture,GLFWwindow *)> handle){
    if(!window){
        return;
    }
    
    while(!glfwWindowShouldClose(window)){
        render_processInput(window);
        //交换颜色缓冲，他是一个存储着GLFW窗口每一个像素颜色值的大缓冲
        //会在这个迭代中用来绘制，并且显示在屏幕上
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        //我们已经告诉，程序要数据什么数据，以及怎么解释整个数据
        //数据传输之后运行程序
        //glUseProgram(shaderProgram);
        shader->use();
        
        if(handle){
            handle(shader,VAO,texture,window);
        }
        
    
        
        glfwSwapBuffers(window);
        //检查有没有触发事件，键盘输入，鼠标移动，更新噶 u 那个口耦
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1, &VBO);
    for(int i = 0;i<texture_count;i++){
        glDeleteTextures(1,&texture[i]);
    }
    
    
    glfwTerminate();
    
}


void RenderEngine::loop(GLuint VAO, GLuint VBO, Shader *shader, function<void (Shader *)> handle){
    if(!window){
        return;
    }
    while(!glfwWindowShouldClose(window)){
        render_processInput(window);
        //交换颜色缓冲，他是一个存储着GLFW窗口每一个像素颜色值的大缓冲
        //会在这个迭代中用来绘制，并且显示在屏幕上
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //我们已经告诉，程序要数据什么数据，以及怎么解释整个数据
        //数据传输之后运行程序
        //glUseProgram(shaderProgram);
        shader->use();
        
        if(handle){
            handle(shader);
        }
        
        
        
        //绑定数据
        glBindVertexArray(VAO);
        //绘制一个三角形
        //从0开始，3个
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //绘制矩形
        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        //检查有没有触发事件，键盘输入，鼠标移动，更新噶 u 那个口耦
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
}


void RenderEngine::loop(GLuint VAO,GLuint VBO,vector<Shader> shaders,GLFWwindow *window,function<void(vector<Shader>,GLFWwindow *)> handle){
    if(!window){
        return;
    }
    int len = shaders.size();
    while(!glfwWindowShouldClose(window)){
        render_processInput(window);
        //交换颜色缓冲，他是一个存储着GLFW窗口每一个像素颜色值的大缓冲
        //会在这个迭代中用来绘制，并且显示在屏幕上
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //我们已经告诉，程序要数据什么数据，以及怎么解释整个数据
        //数据传输之后运行程序
        //glUseProgram(shaderProgram);
//        for(int i = 0;i<len;i++){
//            shaders[i].use();
//        }
//        
        
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


