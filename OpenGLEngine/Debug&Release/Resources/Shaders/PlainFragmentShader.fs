#version 330 core

in vec3 fragNorm;

out vec4 color;

uniform sampler2D tex;

void main() {
	vec3 lightDir = vec3(0.5,0.5,0.0);
	float intensity;
	vec4 color2;
	
	intensity = dot(lightDir,normalize(fragNorm));
	if (intensity > 0.95)
		color2 = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		color2 = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.25)
		color2 = vec4(0.4,0.2,0.2,1.0);
	else
		color2 = vec4(0.2,0.1,0.1,1.0);
		
	color = mix(vec4(fragNorm,1.0), color2, 0.6);
}