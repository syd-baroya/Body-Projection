#version 430 core

in vec2 fragTex;

layout(location = 0) out vec4 color;

layout(location = 0) uniform sampler2D color1;
layout(location = 1) uniform sampler2D color2;

void main()

{

	color = texture(color1, fragTex) +  texture(color2, fragTex);
}
