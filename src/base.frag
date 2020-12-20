#version 330 core

out vec4 fragColor;

in vec2 textureCoor;

uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
    fragColor = mix(texture(tex,textureCoor),texture(tex2,textureCoor),0.5);
}