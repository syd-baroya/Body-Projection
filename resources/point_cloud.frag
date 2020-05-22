#version 430
out vec4 fragColor;
layout(location = 0) uniform sampler2D scene_tex;

uniform vec4 texoffset;
uniform float totaltime;

in vec4 fragmentColor;
in vec2 fragTexCoords;
void main()
{

//    fragColor = fragmentColor;

    if(fragmentColor.r!=0 && fragmentColor.g!=0)// && fragmentColor.b!=0)
    {

//        vec2 texcoords = fragTexCoords;
//        fragColor= texture(scene_tex, texcoords);
//        fragColor.a = 1;

        vec2 texcoord = vec2(fragTexCoords.x/texoffset.x, fragTexCoords.y/texoffset.y);
        texcoord.x +=texoffset.z / texoffset.x;
        texcoord.y +=texoffset.w / texoffset.y;

        fragColor= texture(scene_tex, vec2(texcoord.x,texcoord.y));
        }
    else{
        fragColor = vec4(0,0,0,0);
        }
}