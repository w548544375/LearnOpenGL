#version 330 core
layout (location=0)in vec3 aPos;

layout (location=1)in vec2 aTexCoor;

out vec2 textureCoor;

void main()
{
    gl_Position = vec4(aPos,1.0f);
    textureCoor = aTexCoor;
}