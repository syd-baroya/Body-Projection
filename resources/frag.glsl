#version 430 core
out vec4 color;
layout(location = 0) uniform sampler2D tex;
layout(location = 1) uniform sampler2D texmask;
in vec2 fragtex;

void main()
{
vec2 texcoords = fragtex;
texcoords.x = 1.-texcoords.x;
color.rgb= texture(tex, texcoords).rgb;
color.rgb = vec3(1,0,0);
float mask = texture(texmask, texcoords,0).r;// * texture(texmask, texcoords,3).r;
color.a=mask;
//color.r=mask;
}
