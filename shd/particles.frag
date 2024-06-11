#version 460 core

in vec3 geometry_color;
out vec4 fragment_color;

void main(void)
{
	fragment_color = vec4(geometry_color, 1.0);
}