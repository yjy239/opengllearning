#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
uniform float mixValue;

void main(){
    //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    //mix = mix(x,y,a) = x * (1-a) + y * a
    //箱子 * 0.2 + 笑脸 * 0.8
    FragColor = mix(texture(ourTexture, TexCoord),texture(ourTexture2, TexCoord),mixValue);
}
