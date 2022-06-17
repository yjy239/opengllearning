//
//  triangle.cpp
//  first_opengl
//
//  Created by 余均宇 on 2019/5/12.
//  Copyright © 2019年 余均宇. All rights reserved.
//
#include <iostream>
#include <stdio.h>
#include "glad.h"
#include <GLUT/GLUT.h>
#include <GLFW/glfw3.h>

using namespace std;

enum VAO_IDS{Triangles,NumVAOs};

enum Buffer_IDs{ArrayBuffer,NumBuffers};
enum Attrib_IDs{vPosition = 0};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertrces = 6;

//layout(location = 0) in vec4 vPos;

//void
//display( void )
//{
//    static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
//    
//    glClearBufferfv(GL_COLOR, 0, black);
//    
//    glBindVertexArray( VAOs[Triangles] );
//    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
//}
//
//void init(){
//    //初始化顶点名称
//    glGenVertexArrays(NumVAOs,VAOs);
//    //初始化顶点数组，如果是已经初始化则激活
//    glBindVertexArray(VAOs[Triangles]);
//    
//    GLfloat vertices[NumVertrces][2] = {
//        {-0.90,-0.90},//三角1
//        {0.85,-0.90},
//        {-0.90,0.85},
//        {0.90,-0.85},//三角2
//        {0.90,0.90},
//        {-0.85,0.90}
//    };
//    //绑定生成缓存名称
//    glGenBuffers(NumBuffers,Buffers);
//    //激活缓存对象
//    glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
//    //在opengl服务端内存分配size个存储单元，用于存储数据或者索引
//    //如果当前绑定的对象已经存在了关联的数据，首先删除这些数据
//    /*GL_ARRAY_BUFFER：是指顶点属性数据
//    GL_ELEMENT_ARRAY_BUFFER：索引数据
//    GL_PIXEL_UNPACK_BUFFER：opengl的像素数据
//     GL_PXIEL_PACK_BUFFER:从opengl获取的像素数据
//     GL_COPY_READ_BUFFER和GL_CPOY_WRITE 缓存间的肤质数据
//     GL_TRANSFORM_FEEDBACK_BUFFER:通过transform feedback着色器获得结果
//     GL_TEXTURE_BUFFER 纹理缓存
//     GL_UNIFORM_BUFFER:一致变量
//     
//     size：存储数据的总数
//     data：客户端内存指针
//     usage: 分配数据后的读取写入方式
//    */
//    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,
//                 GL_STATIC_DRAW);
//    
//    
//    
//}

