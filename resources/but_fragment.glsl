#version 430 core
out vec4 color;
layout(location = 0) uniform sampler2D tex;


in vec2 fragtex;
uniform vec4 texsplit;
uniform vec3 redmul;
uniform vec3 greenmul;
uniform vec3 bluemul;
uniform float totaltime;

void main()
{
vec2 texcoord = vec2(fragtex.x/texsplit.x,fragtex.y/texsplit.y);
texcoord.x +=texsplit.z / texsplit.x;
texcoord.y +=texsplit.w / texsplit.y;
//color.rg=fragtex;
vec4 texcol= texture(tex, vec2(texcoord.x,texcoord.y));
color = texcol;
/*color.rgb = vec3(	texcol.r * redmul.r + texcol.g * redmul.g + texcol.b * redmul.b, 
					texcol.r * greenmul.r + texcol.g * greenmul.g + texcol.b * greenmul.b,
					texcol.r * bluemul.r + texcol.g * bluemul.g + texcol.b * bluemul.b);*/

//if(texcol.r==0 && texcol.g==0 && texcol.b==0)
//	color.a = 0;
//else
	color.a = texcol.a;
}
