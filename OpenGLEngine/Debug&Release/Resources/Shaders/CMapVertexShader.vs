#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 MVP;

out vec3 fragTexCoord;

void main()
{
	gl_Position = MVP * vec4(position, 1.0);

	fragTexCoord = position;

}