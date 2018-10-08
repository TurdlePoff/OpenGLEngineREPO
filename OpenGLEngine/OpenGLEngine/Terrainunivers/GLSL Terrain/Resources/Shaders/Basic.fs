#version 330 
smooth out vec4 fragColor;

uniform sampler2D textureSampler;
smooth in vec2 texCoord;

void main()
{
    fragColor = texture2D(textureSampler, texCoord);
} 