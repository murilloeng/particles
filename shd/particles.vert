#version 460 core

uniform uint width;
uniform uint height;

out vec3 vertex_color;
out float vertex_radius;

layout(location = 1) in vec3 color;
layout(location = 2) in float radius;
layout(location = 0) in vec2 position;

void main(void)
{
	//data
	const float w = width;
	const float h = height;
	const float m = min(w, h);
	//outputs
	vertex_color = color;
	vertex_radius = radius;
	gl_Position = vec4(vec2(m / w, m / h) * position, 0.0, 1.0);
}