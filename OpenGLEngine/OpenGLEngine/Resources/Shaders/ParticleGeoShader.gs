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
	gl_Position = vp * vec4(p1, size);
	gs_out.texcoord = vec2(0.0f, 0.0f); EmitVertex();
	
	vec3 p2 = gl_in[0].gl_Position.xyz + (-vQuad1+vQuad2)* size;
	gl_Position = vp * vec4(p2, size);
	gs_out.texcoord = vec2(0.0f, 1.0f); EmitVertex();
	
	vec3 p3 = gl_in[0].gl_Position.xyz + (vQuad1-vQuad2)* size;
	gl_Position = vp * vec4(p3, size);
	gs_out.texcoord = vec2(1.0f, 0.0f); EmitVertex();
	
	vec3 p4 = gl_in[0].gl_Position.xyz + (vQuad1+vQuad2)* size;
	gl_Position = vp * vec4(p4, size);
	gs_out.texcoord = vec2(1.0f, 1.0f); EmitVertex();
	
		/*

		float Scale = 2.0f;

	gl_Position = gs_in[0].position + vp * (vec4(-vQuad1.xy - vQuad2.xy, 0.0f, 0.0f) * Scale);
    gs_out.texcoord = vec2(0.0f, 0.0f); EmitVertex(); // Top Left

    gl_Position = gs_in[0].position + vp * (vec4(-vQuad1.xy + vQuad2.xy, 0.0f, 0.0f) * Scale);
    gs_out.texcoord = vec2(1.0f, 0.0f); EmitVertex(); // Top Right

    gl_Position = gs_in[0].position + vp * (vec4(vQuad1.xy -vQuad2.xy, 0.0f, 0.0f) * Scale); 
    gs_out.texcoord = vec2(0.0f, 1.0f); EmitVertex(); // Bottom Left

    gl_Position = gs_in[0].position + vp * (vec4(vQuad1.xy + vQuad2.xy, 0.0f, 0.0f) * Scale);
    gs_out.texcoord = vec2(1.0f, 1.0f); EmitVertex(); // Bottom Right
	*/
	EndPrimitive();
}