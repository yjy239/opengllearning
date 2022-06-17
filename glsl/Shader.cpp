//
//  Shader.cpp
//  triangle
//
//  Created by 余均宇 on 2019/6/23.
//  Copyright © 2019年 余均宇. All rights reserved.
//

#include "Shader.hpp"

Shader::Shader(const char* vertexPath,const char* fragmentPath){
    mVertexPath = vertexPath;
    mFragmentPath = fragmentPath;
}

void Shader:: compile(){
    if(!mVertexPath || !mFragmentPath){
        cout<< "Error::Shader::please set file Path";
        isSuccess = false;
        return;
    }
    
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    
    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    
    try{
        vShaderFile.open(mVertexPath);
        fShaderFile.open(mFragmentPath);
        
        stringstream vShaderStream,fShaderStream;
        vShaderStream <<vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        vertexCode =vShaderStream.str();
        fragmentCode = fShaderStream.str();
        
        
    }catch(ifstream::failure e){
        isSuccess = false;
        cout<< "Error::Shader::file loaded fail";
    }
    
    if(vertexCode.empty()|| fragmentCode.empty()){
        isSuccess = false;
        return;
    }
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode =fragmentCode.c_str();
    
    GLuint vertexShader;
    //创建一个着色器类型
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //把代码复制进着色器中
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    //编译顶点着色器
    glCompileShader(vertexShader);
    
    //判断是否编译成功
    
    if(!checkComplieErrors(vertexShader, "VERTEX")){
        isSuccess = false;
        return;
    }
    
    ///下一个阶段是片段着色器
    
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    
    glCompileShader(fragmentShader);
    
    if(!checkComplieErrors(fragmentShader, "Fragment")){
        isSuccess = false;
        return;
    }
    
    
    //链接，创建一个程序
    
    ID = glCreateProgram();
    
    //链接
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    
    if(!checkComplieErrors(ID, "PROGRAM")){
        return;
    }
    
    //编译好了之后，删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    isSuccess = true;
}

void Shader:: use(){
    if(!isSuccess){
        return;
    }
    glUseProgram(ID);
}
void Shader::setBool(const std::string &name,bool value) const{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
}
void Shader:: setInt(const std::string &name,int value) const{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
}
void Shader:: setFloat(const std::string &name,float value) const{
    glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setMat4(const std::string &name,const GLfloat *value){
    glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()),1,GL_FALSE,value);
}

void Shader::setVec3(const std::string &name,const GLfloat value1,const GLfloat value2,const GLfloat value3){
    glUniform3f(glGetUniformLocation(ID,name.c_str()),value1,value2,value3);
}

bool Shader::checkComplieErrors(unsigned int shader, std::string type){
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            isSuccess = false;
            return false;
        }
    }else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            isSuccess = false;
            return false;
        }
    }
    
    return true;
}

Shader::~Shader(){
    
}
