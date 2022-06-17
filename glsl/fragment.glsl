#version 330 core
out vec4 mFragColor;
uniform vec4 mChangingColor;
void main(){
    mFragColor = mChangingColor;
}
