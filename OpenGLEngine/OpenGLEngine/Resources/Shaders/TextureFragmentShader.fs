#version 330 core

in vec3 fragColor;
in vec2 fragTexCoord;

out vec4 color;

uniform sampler2D tex;
uniform vec3 lightDir = vec3(1.0, 1.0, 1.0);

void main() {
    color = texture(tex, fragTexCoord);
	vec4(fragColor, 1.0);
	
	//float intensity = dot(normalize(lightDir), color);
	//vec4 color2 = vec4(1.0, 1.0, 1.0, 1.0);

	//if (intensity > 0.95) color2 = vec4(1.0, 1.0, 1.0, //1.0);

	//color = color2 * fragTexCoord;
}