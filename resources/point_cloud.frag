#version 430
out vec4 fragColor;
layout(location = 0) uniform sampler2D scene_tex;

uniform vec4 texoffset;
uniform float totaltime;

in vec4 fragmentColor;
in vec2 fragTexCoords;
in float fragAnimLife;

void main()
{


    if(fragmentColor.r!=0 && fragmentColor.g!=0)// && fragmentColor.b!=0)
    {
        vec2 texcoord = vec2(fragTexCoords.x/texoffset.x, fragTexCoords.y/texoffset.y);
         if(fragAnimLife<=0.0f)
         {
            texcoord.x +=texoffset.z / texoffset.x;
            texcoord.y +=texoffset.w / texoffset.y;
         }

        fragColor= texture(scene_tex, vec2(texcoord.x,texcoord.y));
//        fragColor = vec4(fragmentColor.rgb, 1);

        if(fragAnimLife>0.0f)
            fragColor.a = 0.1f;
       
        if(fragColor.rgb == vec3(0))
            discard;
    }
    else
        discard;
}