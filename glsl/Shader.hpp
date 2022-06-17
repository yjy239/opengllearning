//
//  Shader.hpp
//  triangle
//
//  Created by 余均宇 on 2019/6/23.
//  Copyright © 2019年 余均宇. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp
#include "glad.h"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class Shader{
public:
    unsigned int ID;
    bool isSuccess = false;
    const char* mVertexPath;
    const char* mFragmentPath;
    
public:
    Shader(const char* vertexPath,const char* fragmentPath);
    void compile();
    void use();
    void setBool(const std::string &name,bool value) const;
    void setInt(const std::string &name,int value) const;
    void setFloat(const std::string &name,float value) const;
    void setMat4(const std::string &name,const GLfloat *value);
    void setVec3(const std::string &name,const GLfloat value1,const GLfloat value2,const GLfloat value3);
    inline bool isCompileSuccess() const{
        return isSuccess;
    }
    ~Shader();
    
private:
    bool checkComplieErrors(unsigned int shader,std::string type);
};

#endif /* Shader_hpp */
