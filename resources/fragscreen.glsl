#version 430 core
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 mask;

layout(location = 0) uniform sampler2D tex;

in vec2 fragtex;

void main()
{
vec2 texcoords = fragtex;
//texcoords.x = 1.-texcoords.x;
color.rgb= texture(tex, texcoords).rgb;
//color.rgb = vec3(0,1,0);
color.a=1;
mask = vec4(0);
}
