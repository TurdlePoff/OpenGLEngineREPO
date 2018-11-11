#version 330 core

const int MAX_JOINTS = 100;//max joints allowed in a skeleton
const int MAX_WEIGHTS = 4;//max number of joints that can affect a vertex

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 vp;
uniform mat4 model;
uniform mat4 jointTransforms[MAX_JOINTS];

void main(void){

	vec4 totalLocalPosition = vec4(0.0);
	vec4 totalNormal = vec4(0.0);
	
	for(int i = 0; i < MAX_WEIGHTS; i++){
		vec4 posePosition = jointTransforms[boneIds[i]] * vec4(position, 1.0);
		totalLocalPosition += posePosition * weights[i];
		vec4 worldNormal = jointTransforms[boneIds[i]] * vec4(normal, 0.0);
		totalNormal += worldNormal * weights[i];
	}
	
	gl_Position = vp * model * totalLocalPosition;
	TexCoord = texCoord;
	FragPos = vec3(model* vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;

}