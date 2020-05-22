#version 430
out vec4 fragColor;
layout(location = 0) uniform sampler2D scene_tex;

in vec4 fragmentColor;

void main()
{
    fragColor = fragmentColor;
}