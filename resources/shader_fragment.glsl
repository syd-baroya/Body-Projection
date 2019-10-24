#version 430 core
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 mask;

layout(location = 0) uniform sampler2D tex;
layout(location = 1) uniform sampler2D tex2;
layout(location = 2) uniform sampler2D texA;
layout(location = 3) uniform sampler2DArray texarr;
in vec2 fragtex;
uniform vec2 texsplit;
uniform float totaltime;
uniform float firescale;
void main()
{
float tick = totaltime*25;
int itick = int(tick)%25;
vec2 texcoord = fragtex.xy;
texcoord-=vec2(0.5);
texcoord*=firescale;
texcoord+=vec2(0.5);

vec4 fire = texture(texarr, vec3(texcoord.x,1-texcoord.y,float(itick)));
fire.a = max(fire.a,texture(texA, texcoord).r);
color =fire;
mask = vec4(1,1,1,1);
}
