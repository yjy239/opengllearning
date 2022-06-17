//
//  triangle.cpp
//  triangle
//
//  Created by 余均宇 on 2019/5/13.
//  Copyright © 2019年 余均宇. All rights reserved.
//
#include <iostream>
#include <stdio.h>
#include "glad.h"
#include <GLUT/GLUT.h>
#include <GLFW/glfw3.h>
#include <math.h>

using namespace std;

//顶点着色器
//注意GLSL一个向量最多4个分量x,y,z代表三维代表透视除法
#define VERTEX_SHADER ("#version 330 core\n\
layout (location = 0) in vec3 aPos;\n\
void main(){\n\
gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n\
}\0")

#define VERTEX_SHADER2 ("#version 330 core\n\
layout (location = 0) in vec3 aPos;\n\
layout(location = 1) in vec3 aColor;\n\
out vec3 ourColor;\n\
void main(){\n\
gl_Position = vec4(aPos,1.0);\n\
ourColor = aColor;\
}\0")

#define FRAGMENT_SHADER3 ("#version 330 core\n\
out vec4 FragColor;\n\
in vec3 ourColor; \n\
void main(){\n\
FragColor = vec4(ourColor,1.0);\n\
}\0")

#define FRAGMENT_SHADER ("#version 330 core\n\
out vec4 FragColor;\n\
uniform vec4 ourColor;\n\
void main(){\n\
FragColor = ourColor;\n\
}\0")

#define FRAGMENT_SHADER2 ("#version 330 core\n\
out vec4 FragColor;\n\
in vec4 ourColor; \n\
void main(){\n\
FragColor = ourColor;\n\
}\0")



void flushTriangleToOpengl(GLuint& VAO,GLuint& VBO){
    //我们要绘制三角形
    float vertices[] = {
        //位置              //颜色
        -0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,
        0.5f, -0.5f, 0.0f,0.0f,1.0f,0.0f,
        0.0f,  0.5f, 0.0f,0.0f,0.0f,1.0f
    };
    
    //生成分配VAO
    glGenVertexArrays(1,&VAO);
    //生成一个VBO缓存对象
    glGenBuffers(1, &VBO);
    
    //绑定VAO，注意在core模式，没有绑定VAO，opengl拒绝绘制任何东西
    glBindVertexArray(VAO);
    
    
    //绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //复制顶点给opengl缓存
    //类型为GL_ARRAY_BUFFER 第二第三参数说明要放入缓存的多少，GL_STATIC_DRAW当画面不懂的时候推荐使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //设定顶点属性指针
    //第一个参数指定我们要配置顶点属性，对应vertex glsl中location 确定位置
    //第二参数顶点大小，顶点属性是一个vec3，由3个值组成，大小是3
    //第三参数指定数据类型，都是float(glsl中vec*都是float)
    //第四个参数：是否被归一化
    //第五参数：步长，告诉我们连续的顶点属性组之间的间隔，这里是每一段都是3个float，所以是3*float
    //最后一个是偏移量
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
}

//要画矩形，因此要两个三角形
void flushRetriangle(GLuint& VAO,GLuint& VBO,GLuint& EBO){
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角33333333333333
        -0.5f, 0.5f, 0.0f   // 左上角
    };
    
    unsigned int indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    //生成分配VAO
    glGenVertexArrays(1,&VAO);
    //生成一个VBO缓存对象
    glGenBuffers(1, &VBO);
    //生成一个索引
    glGenBuffers(1,&EBO);
    
    //绑定VAO，注意在core模式，没有绑定VAO，opengl拒绝绘制任何东西
    glBindVertexArray(VAO);
    //绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //绑定索引
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    
    //复制顶点给opengl缓存
    //类型为GL_ARRAY_BUFFER 第二第三参数说明要放入缓存的多少，GL_STATIC_DRAW当画面不懂的时候推荐使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //把索引复制缓存
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
   
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

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

int draw(){
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
    
    //-------------------
    //我们还要告诉opengl渲染的窗口大小
    //渲染可以比window小，这样就只会在window内部一个小窗口渲染
    //    glViewport(0,0,800,600);
    //    glFinish();
    //实际上opengl的坐标范围从-1到1，会映射到（800，600）
    
    //当拉动窗口调整大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
    
    //------------------- 编译一个小型glsl程序
    //因为opengl实际上是一个gpu编译器，很开放，相当于允许我们写代码，走一遍编译流程
    //根据流程依次初始化着色器，片段着色器
    
    //1.初始化着色器
    const char* vertexShaderSource = VERTEX_SHADER2;
    GLuint vertexShader;
    //创建一个着色器类型
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //把代码复制进着色器中
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //编译顶点着色器
    glCompileShader(vertexShader);
    
    //判断是否编译成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    //判断是否编译成功
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout<< "error when vertex compile:"<<infoLog<<endl;
        return 0;
    }
    
    ///下一个阶段是片段着色器
    const char* fragmentShaderSource = FRAGMENT_SHADER3;
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    
    //判断是否编译成功
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout<< "error when fragment compile:"<<infoLog<<endl;
        return 0;
    }
    
    
    //链接，创建一个程序
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    
    //链接
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout<< "error when link compile:"<<infoLog<<endl;
    }
    
    
    //编译好了之后，删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    //----------------要传输数据到程序中
    //一个顶点对象
    GLuint VAO;
    //一个缓存对象
    GLuint VBO;
    
    GLuint EBO;
    flushTriangleToOpengl(VAO,VBO);
    //flushRetriangle(VAO, VBO, EBO);
    //并不希望绘制之一个图像之后，进程就退出。因此可以在主动关闭之前接受用户输入
    //判断当前窗口是否被要求退出。
    while(!glfwWindowShouldClose(window)){
        processInput(window);
        //交换颜色缓冲，他是一个存储着GLFW窗口每一个像素颜色值的大缓冲
        //会在这个迭代中用来绘制，并且显示在屏幕上
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //我们已经告诉，程序要数据什么数据，以及怎么解释整个数据
        //数据传输之后运行程序
        glUseProgram(shaderProgram);
        
        //更新uniform颜色
//        float timeValue = glfwGetTime();
//        float colorValue = sin(timeValue) / 2.0 + 0.5f;
//        //拿到uniform的位置索引
//        int vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
//        glUniform4f(vertexColorLocation, 0.0f, colorValue, 0.0f, 1.0f);
        
        
        //绑定数据
        glBindVertexArray(VAO);
        //绘制一个三角形
        //从0开始，3个
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //绘制矩形
        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        //检查有没有触发事件，键盘输入，鼠标移动，更新噶 u 那个口耦
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    
    return 0;
}

int drawTwoTrangle(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow *window = glfwCreateWindow(800, 800, "test opengl", NULL, NULL);
    if(!window){
        cout <<"fail open window"<<endl;
        glfwTerminate();
        return -1;
    }
    
    
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
         cout<< "failed to init glad" <<endl;
    }
    
     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    const char* vertextShaderSource = VERTEX_SHADER;
    GLint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //复制代码
    glShaderSource(vertexShader,1, &vertextShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout<< "error when link compile:"<<infoLog<<endl;
    }
    
    
    const char* fragmentShaderSource = FRAGMENT_SHADER;
    GLint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout<< "error when fragment compile:"<<infoLog<<endl;
        return 0;
    }
    

    GLint pram;
    pram = glCreateProgram();
    glAttachShader(pram,vertexShader);
    glAttachShader(pram,fragmentShader);
    glLinkProgram(pram);
    
    glGetProgramiv(pram,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(pram, 512, NULL, infoLog);
        cout<< "error when link compile:"<<infoLog<<endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    GLuint VAO,VBO;
    
    //我们要绘制三角形
    float vertices[] = {
        //第一个三角形
        -0.5f, -0.5f, 0.0f,//左下角
        0.5f, -0.5f, 0.0f,//右下角
        -0.5f,  0.5f, 0.0f,//左上角
        
        
        //第二个三角形
        0.5f,0.5f,0.0f,//右上角
        0.5f, -0.5f, 0.0f,//右下角
        -0.5f,  0.5f, 0.0f//左上角
    };
    
    //设置VAO 顶点数组对象
    glGenVertexArrays(1,&VAO);
    //设置VBO
    glGenBuffers(1,&VBO);
    
    //绑定
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //绑定数据
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),&vertices,GL_STATIC_DRAW);
    
    
    //设置指针移动模式
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(pram);
        glBindVertexArray(VAO);
        
        
        
        glDrawArrays(GL_LINE_LOOP,0,6);
        
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        
        glfwPollEvents();
        
    }
    
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    
    glfwTerminate();
    
    return 0;
    
}


int drawTwoTrangleSingle(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow *window = glfwCreateWindow(800, 800, "test opengl", NULL, NULL);
    if(!window){
        cout <<"fail open window"<<endl;
        glfwTerminate();
        return -1;
    }
    
    
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout<< "failed to init glad" <<endl;
    }
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    const char* vertextShaderSource = VERTEX_SHADER;
    GLint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    //复制代码
    glShaderSource(vertexShader,1, &vertextShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout<< "error when link compile:"<<infoLog<<endl;
    }
    
    
    const char* fragmentShaderSource = FRAGMENT_SHADER;
    GLint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout<< "error when fragment compile:"<<infoLog<<endl;
        return 0;
    }
    
    
    GLint pram;
    pram = glCreateProgram();
    glAttachShader(pram,vertexShader);
    glAttachShader(pram,fragmentShader);
    glLinkProgram(pram);
    
    glGetProgramiv(pram,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(pram, 512, NULL, infoLog);
        cout<< "error when link compile:"<<infoLog<<endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    GLuint VAOs[2],VBOs[2];

    
    //我们要绘制三角形
    float vertices[] = {
        //第一个三角形
        0.0f, 0.0f, 0.0f,//
        -0.5f, -0.5f, 0.0f,//顶点
    
        -0.5f,  0.0f, 0.0f,//
        
    };
    
    
    float vertices1[] = {

        //第二个三角形
        0.0f, -0.0f, 0.0f,//
        0.5f,0.5f,0.0f,//右上角
        0.5f,  0.0f, 0.0f//左上角
    };
    
    //设置VAO 顶点数组对象
    glGenVertexArrays(2,VAOs);
    //设置VBO
    glGenBuffers(2,VBOs);
    
    
    //绑定
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);
    //绑定数据
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),&vertices,GL_STATIC_DRAW);
    //设置指针移动模式
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    
    
    
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);
    //绑定数据
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices1),&vertices1,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(pram);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        
        glfwPollEvents();
        
    }
    
    
    glDeleteVertexArrays(2,VAOs);
    glDeleteBuffers(2,VBOs);
    
    glfwTerminate();
    
    return 0;
    
}


int drawTwoTrangleTwoFragment(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow *window = glfwCreateWindow(800, 800, "test opengl", NULL, NULL);
    if(!window){
        cout <<"fail open window"<<endl;
        glfwTerminate();
        return -1;
    }
    
    
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout<< "failed to init glad" <<endl;
    }
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    const char* vertextShaderSource = VERTEX_SHADER;
    GLint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    //复制代码
    glShaderSource(vertexShader,1, &vertextShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout<< "error when link compile:"<<infoLog<<endl;
    }
    
    
    const char* fragmentShaderSource = FRAGMENT_SHADER;
    GLint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout<< "error when fragment compile:"<<infoLog<<endl;
        return 0;
    }
    
    const char* fragmentShaderSource2 = FRAGMENT_SHADER2;
    GLint fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2,1,&fragmentShaderSource2,NULL);
    glCompileShader(fragmentShader2);
    glGetShaderiv(fragmentShader2,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        cout<< "error when fragment compile:"<<infoLog<<endl;
        return 0;
    }
    
    
    GLint pram;
    pram = glCreateProgram();
    glAttachShader(pram,vertexShader);
    glAttachShader(pram,fragmentShader);
    glLinkProgram(pram);
    
    glGetProgramiv(pram,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(pram, 512, NULL, infoLog);
        cout<< "error when link compile:"<<infoLog<<endl;
    }
    
    GLint pram2;
    pram2 = glCreateProgram();
    glAttachShader(pram2,vertexShader);
    glAttachShader(pram2,fragmentShader2);
    glLinkProgram(pram2);
    
    glGetProgramiv(pram2,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(pram, 512, NULL, infoLog);
        cout<< "error when link compile:"<<infoLog<<endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);
    
    GLuint VAOs[2],VBOs[2];
    
    
    //我们要绘制三角形
    float vertices[] = {
        //第一个三角形
        0.0f, 0.0f, 0.0f,//
        -0.5f, -0.5f, 0.0f,//顶点
        
        -0.5f,  0.0f, 0.0f,//
        
    };
    
    
    float vertices1[] = {
        
        //第二个三角形
        0.0f, -0.0f, 0.0f,//
        0.5f,0.5f,0.0f,//右上角
        0.5f,  0.0f, 0.0f//左上角
    };
    
    //设置VAO 顶点数组对象
    glGenVertexArrays(2,VAOs);
    //设置VBO
    glGenBuffers(2,VBOs);
    
    
    //绑定
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);
    //绑定数据
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),&vertices,GL_STATIC_DRAW);
    //设置指针移动模式
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    
    
    
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);
    //绑定数据
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices1),&vertices1,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(pram);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        glUseProgram(pram2);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        
        glfwPollEvents();
        
    }
    
    
    glDeleteVertexArrays(2,VAOs);
    glDeleteBuffers(2,VBOs);
    
    glfwTerminate();
    
    return 0;
    
}





int main(int argc, const char * argv[]){
   //基础类型
    //int float double bool
    /*
     //从float向量转化为int向量
     vec3 v = vec3(0.0,2.0,3.0);
     ivec3 s = ivec3(v);
     
     
     vec4 color;
     vec3 rgb = vec3(color);//这样就能截断前三个数据
     
     
     vec3 white = vec3(1.0);//white = (1.0,1.0,1.0)
     vec4 t = vec3(white,0.5);//(1.0,1.0,1.0,0.5)
     
     //相当于为对角赋值4.0
     m =  mat3(4.0) = (4.0,0,0
                       0,4.0,0
                       0,0,4.0)
     
     mat3 M = mat3(1.0,2.0,3.0,
                    4.0,5.0,6.0,
                    7.0,8.0,9.0);
     
     vec3 c1 = vec3(1.0,2.0,3.0);
     vec3 c2 = vec3(4.0,5.0,6.0);
     vec3 c3 = vec3(7.0,8.0,9.0);
     mat3 M = mat3(c1,c2,c3);
     
     vec2 c1 = vec2(1.0,2.0;
     vec2 c2 = vec2(4.0,5.0);
     vec2 c3 = vec2(7.0,8.0);
     
     mat3 M = mat3(c1,3.0,
                    c2,6.0,
                    c3,9.0);
     
     先设置列，在设置行
     这些结果都是矩阵：(1.0 4.0 7.0
                     2.0 5.0 8.0
                    3.0 4.0 9.0)
     
     
     */
    
    draw();
    
    return 0;
}
