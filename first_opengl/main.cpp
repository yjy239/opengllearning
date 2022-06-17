//
//  main.cpp
//  first_opengl
//
//  Created by 余均宇 on 2019/5/12.
//  Copyright © 2019年 余均宇. All rights reserved.
//
#include <iostream>
#include <stdio.h>
#include "glad.h"
#include <GLFW/glfw3.h>


using namespace std;

void framebuffer_size_callback(GLFWwindow *window,int width,int height){
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow *window){
    //glfwGetKey确认这个窗口有没有处理按键
    //GLFW_KEY_ESCAPE 代表esc按键
    //GLFW_PRESS代表按下 GLFW_RELEASE 代表没按下
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
       //关闭窗口
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, const char * argv[]){
    //初始化GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //创建一个800*600的窗口
    GLFWwindow *window = glfwCreateWindow(800, 600, "Learn opengl", NULL, NULL);
    if(!window){
        cout <<"fail open window"<<endl;
        glfwTerminate();
        return -1;
    }
    
    //把这个窗口作为当前线程主要上下文
    glfwMakeContextCurrent(window);
    
    //GLAD是用来管理OpenGL的函数指针的,需要初始化

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout<< "failed to init glad" <<endl;
    }
    
    //我们还要告诉opengl渲染的窗口大小
    //渲染可以比window小，这样就只会在window内部一个小窗口渲染
    glViewport(0,0,800,600);
    //实际上opengl的坐标范围从-1到1，会映射到（800，600）
    
    //当拉动窗口调整大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
    //并不希望智慧之一个图像之后，进程就退出。因此可以在主动关闭之前接受用户输入
    //判断当前窗口是否被要求退出。
    while(!glfwWindowShouldClose(window)){
        processInput(window);
        //交换颜色缓冲，他是一个存储着GLFW窗口每一个像素颜色值的大缓冲
        //会在这个迭代中用来绘制，并且显示在屏幕上
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(window);
        //检查有没有触发事件，键盘输入，鼠标移动，更新噶 u 那个口耦
        glfwPollEvents();
    }
    
    //双缓冲体系
    //应用使用单缓冲绘图会造成图像闪烁问题。因为图像不是一下子被绘制出来
    //而是按照从左到右，从上到下逐个像素绘制出来。最终图像不是在瞬间显示给用户
    //而是一步步生成，这导致渲染结果布政使。
    
    //为了规避这些问题，我们使用双缓冲渲染创酷应用。前缓冲保存着最终输出图像，显示在屏幕
    //而所有的渲染指令都会在后缓冲上绘制，当所哟肚饿渲染指令执行完毕之后，
    //我们交换前后缓冲，这样图像就显示出来。
    
    glfwTerminate();
    
    return 0;
}
