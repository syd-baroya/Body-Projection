
#version 430 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec2 fragtex;

void main()
{

	fragtex = vertTex;
	vec4 tpos =  M * vec4(vertPos, 1.0);
	gl_Position = P * V * tpos;



}
