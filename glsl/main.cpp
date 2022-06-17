//
//  main.cpp
//  glsl
//
//  Created by 余均宇 on 2019/7/14.
//  Copyright © 2019年 余均宇. All rights reserved.
//

#include <iostream>
//#include "glad.h"
//#include <GLUT/GLUT.h>
//#include <GLFW/glfw3.h>
//#include <math.h>
//#include "Shader.hpp"
//#include <functional>
#include "RenderEngine.hpp"

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

GLFWwindow*  init(){
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
        return NULL;
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
    
    return window;
    
}

void loop(GLFWwindow *window,const GLuint VAO,const GLuint VBO,Shader *shader,function<void(Shader*)> handle){
    while(!glfwWindowShouldClose(window)){
        processInput(window);
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

void flushTriangleToOpengl(GLuint& VAO,GLuint& VBO){
    //我们要绘制三角形
    float vertices[] = {
        //位置              //颜色
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
}

void mixColorTri(GLuint& VAO,GLuint& VBO){
    float vertices[] = {
        //位置            //着色
        -0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,
        0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,
        0.0f,0.5f,0.0f,0.0f,0.0f,1.0f
    };
    
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    
    //绑定
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //绑定数据
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    //告诉opengl 每6个顶点往后读取下一个数据
    //这个0代表了 layout(location = 0)
    //最后一个参数是偏移量
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),0);
    glEnableVertexAttribArray(0);
    
    //走3个float的偏移量，开始读取数据
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
}

int main(int argc, const char * argv[]) {
    
    //GLFWwindow *window = init();
    RenderEngine *engine = new RenderEngine();

    GLuint VAO;
    //一个缓存对象
    GLuint VBO;
    const char* vPath = "/Users/yjy/Desktop/iOS workspcae/first_opengl/glsl/move_veritex.glsl";
    const char* fPath = "/Users/yjy/Desktop/iOS workspcae/first_opengl/glsl/direct_fragment.glsl";
    Shader *shader = new Shader(vPath,fPath);
    shader->compile();
    if(shader&&shader->isCompileSuccess()){
        mixColorTri(VAO,VBO);
        static float init = 0.0f;
        engine->loop(VAO,VBO,shader,[](Shader *shader){
            //更新uniform颜色
            init += 0.005;

            shader->setFloat("offset", init);
        });
    }
    
    
    return 0;
}
