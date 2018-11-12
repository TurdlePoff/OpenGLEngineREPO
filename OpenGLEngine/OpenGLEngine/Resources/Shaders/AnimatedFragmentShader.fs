#version 330 core

in vec3 fragColor;
in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D tex;

void main() {
    color = texture(tex, fragTexCoords);
}