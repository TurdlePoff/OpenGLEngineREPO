#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 vp;
uniform vec3 vQuad1, vQuad2;

out GS_FS_VERTEX{
vec2 texcoord;

}gs_out;

void main() {
	float size = 1.0f;
	vec3 p1 = gl_in[0].gl_Position.xyz +(-vQuad1-vQuad2)* size;
	gl_Position = vp * vec4(p1, 1.0);
	gs_out.texcoord = vec2(0.0, 0.0); EmitVertex();
	
	vec3 p2 = gl_in[0].gl_Position.xyz + (-vQuad1+vQuad2)* size;
	gl_Position = vp * vec4(p2, 1.0);
	gs_out.texcoord = vec2(0.0f, 1.0); EmitVertex();
	
	vec3 p3 = gl_in[0].gl_Position.xyz + (vQuad1-vQuad2)* size;
	gl_Position = vp * vec4(p3, 1.0);
	gs_out.texcoord = vec2(1.0, 0.0); EmitVertex();
	
	vec3 p4 = gl_in[0].gl_Position.xyz + (vQuad1+vQuad2)* size;
	gl_Position = vp * vec4(p4, 1.0);
	gs_out.texcoord = vec2(1.0, 1.0f); EmitVertex();
	EndPrimitive();
}