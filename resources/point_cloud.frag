#version 430
out vec4 fragColor;
in vec4 fragmentColor;

void main()
{
    fragColor = fragmentColor;
}