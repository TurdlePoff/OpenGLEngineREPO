#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 fragTexCoord;
out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 model; // Or ‘normalMatrix’

uniform mat4 MVP;

void main(void)
{

    //gl_Position = vec4(position, 1.0);
    //gl_Position = MVP * vec4(position, 1.0);

    //fragColor = color;
    //fragTexCoord = texCoord;
	
	gl_Position = MVP * vec4(position, 1.0);
	fragPos = vec3(model * vec4(position, 1.0f));
	fragNormal = mat3(transpose(inverse(model))) * normal;
	fragTexCoord = texCoord;
}