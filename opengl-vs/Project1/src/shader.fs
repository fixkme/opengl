#version 330 core

out vec4 outColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{   
    outColor = vec4(texture(texture_diffuse1, TexCoords));
}