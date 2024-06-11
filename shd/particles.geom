#version 460 core

#define nv 10
#define M_PI 3.14159265358979323846264338327950288

in vec3 vertex_color[];
in float vertex_radius[];

out vec3 geometry_color;

layout (points) in;
layout (triangle_strip, max_vertices = 3 * nv) out;

void main(void)
{
	//data
	const float dt = 2 * M_PI / nv;
	//vertices
	for(int i = 0; i < nv; i++)
	{
		geometry_color = vertex_color[0];
		gl_Position = gl_in[0].gl_Position;
		EmitVertex();
		geometry_color = vertex_color[0];
		gl_Position = gl_in[0].gl_Position + vertex_radius[0] * vec4(cos(dt * (i + 0)), sin(dt * (i + 0)), 0, 1);
		EmitVertex();
		geometry_color = vertex_color[0];
		gl_Position = gl_in[0].gl_Position + vertex_radius[0] * vec4(cos(dt * (i + 1)), sin(dt * (i + 1)), 0, 1);
		EmitVertex();
	}
	EndPrimitive();
}