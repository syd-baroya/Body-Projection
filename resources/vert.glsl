#version 430 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 vertTex;


out vec2 fragtex;

void main()
{
	fragtex = vertTex;
	gl_Position =vec4(vertPos, 1.0);
}