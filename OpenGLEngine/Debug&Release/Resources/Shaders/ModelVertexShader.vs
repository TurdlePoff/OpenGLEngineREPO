#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 fragTexCoords;

uniform mat4 MVP;

void main(void){
	gl_Position = MVP * vec4(position, 1.0);
	fragTexCoords = texCoords;
}