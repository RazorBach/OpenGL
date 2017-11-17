#version 330 core
out vec4 color;


in vec3 resultColor;

void main()
{	
	color = vec4(resultColor, 1.0f);
}