#version 430 core
out vec4 color;
layout(location = 0) uniform sampler2D tex;
layout(location = 1) uniform sampler2D tex2;

in vec2 fragtex;
uniform vec3 redmul;
uniform vec3 greenmul;
uniform vec3 bluemul;
uniform float totaltime;
uniform float texblend;

void main()
{
vec2 texcoord = fragtex;

//color.rg=fragtex;
vec4 texcol= texture(tex, vec2(texcoord.x,1.-texcoord.y));
//if(texblend>0.0001)
//	{
//	//texblend = clamp(texblend,0.0,1.0);
//	vec4 texcol2 = texture(tex2, vec2(texcoord.x,1.-texcoord.y));
//	texcol = texcol * (1. - texblend) + texcol2 * texblend;
//	}


color.rgb = vec3(	texcol.r * redmul.r + texcol.g * redmul.g + texcol.b * redmul.b, 
					texcol.r * greenmul.r + texcol.g * greenmul.g + texcol.b * greenmul.b,
					texcol.r * bluemul.r + texcol.g * bluemul.g + texcol.b * bluemul.b);

color.a = 1;

//color = vec4(0.5,0,1,1);

}

