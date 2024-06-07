#version 460 core

uniform uint width;
uniform uint height;

layout(location = 0) in vec2 position;

void main(void)
{
	const float w = width;
	const float h = height;
	const float m = min(w, h);
	gl_Position = vec4(vec2(m / w, m / h) * position, 0.0, 1.0);
}