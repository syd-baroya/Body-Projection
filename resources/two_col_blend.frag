#version 430 core

in vec2 fragTex;

layout(location = 0) out vec4 color;

layout(location = 0) uniform sampler2D color1;
layout(location = 1) uniform sampler2D color2;

void main()

{
	vec4 t0 = texture2D(color1, fragTex);
    vec4 t1 = texture2D(color2, fragTex);
    color = t0 + t1;

}