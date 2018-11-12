#version 330 core

in vec2 fragTexCoord;
in vec3 fragPos;
in vec3 fragNormal;

out vec4 color;

uniform float ambientStr; // = 0.05f;
uniform vec3 ambientColor; // = glm::vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightColor; // = glm::vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos; // = glm::vec3(5.0f, 5.0f, 0.5f);
uniform float lightSpecStr;// = 1.0f;
uniform vec3 camPos;// = vec3(1.0f, 10.0f, 600.0f);
uniform float shininess;// = 1.0f;

uniform sampler2D tex;

void main() {
    vec3 ambient = ambientStr * ambientColor;
	
	// Light Direction
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(fragPos - lightPos);
	
	// Diffuse Coloring
	float diffuseStr = max(dot(norm, -lightDir ), 0.0f);
	vec3 diffuse = diffuseStr * lightColor;
	
	// Specular Highlight
	vec3 negViewDir = normalize(camPos - fragPos);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(negViewDir , reflectDir), 0.0f), shininess);
	vec3 specular = lightSpecStr * spec * lightColor;
		
	color = vec4(ambient + diffuse + specular, 1.0f) * texture(tex, fragTexCoord);
	
	
	//color = texture(tex, fragTexCoord);// * vec4(fragColor, 1.0f);
}