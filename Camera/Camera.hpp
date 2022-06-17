//
//  Camera.hpp
//  Camera
//
//  Created by 余均宇 on 2019/9/5.
//  Copyright © 2019年 余均宇. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum MOVEMENT{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    float Yaw = YAW;
    float Pitch = PITCH;
    float MovementSpeed = SPEED;
    float Zoom = ZOOM;
    float MouseSensitivity;
    
    Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f),
           glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),
           float yaw = YAW,float pitch = PITCH);
    
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ,
           float upX, float upY, float upZ,
           float yaw, float pitch);
    
    
    glm::mat4 GetViewMatrix();
    
    void ProcessKeyboard(MOVEMENT movement,float deltaTime);
    
    void ProcessMouseMovement(float xoffset,float yoffset,GLboolean constrainPitch = true);
    
    void ProcessMouseScroll(float yoffset);
    
private:
    void updateCameraVectors();
};
#endif /* Camera_hpp */
