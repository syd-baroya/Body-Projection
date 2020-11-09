#version 430
out vec4 fragColor1;
out vec4 fragColor2;

layout(location = 0) uniform sampler2D scene_tex;

uniform vec4 texoffset;
uniform float totaltime;

in vec4 fragmentColor;
in vec2 fragTexCoords;
flat in int fragAnim;


void main()
{
	vec4 fragColor = fragmentColor;

    if(fragColor.rgb != vec3(0))
    {
         if(fragColor.r >=0.9 && fragColor.g >= 0.9 && fragColor.b >= 0.9)
         {
//            vec2 texcoord = vec2(fragTexCoords.x/texoffset.x, fragTexCoords.y/texoffset.y);
//            if(fragAnim==1)
//            {
//                texcoord.x +=texoffset.z / texoffset.x;
//                texcoord.y +=texoffset.w / texoffset.y;
//            }
//            fragColor1= texture(scene_tex, vec2(texcoord.x,texcoord.y));
//            fragColor2 = vec4(0,0,0,0);
//            if(fragColor1.r < 0.5 && fragColor1.g < 0.5 && fragColor1.b < 0.5)
//                discard;
            fragColor2 = vec4(0,0,0,0);
            fragColor1 = vec4(1, 0, 0, 1);
         }

        else
        {
            fragColor1 = vec4(0,0,0,0);
//            fragColor2 = fragColor;
            fragColor2 = vec4(1, 0.967, .549, 1);
//
        }
        
    }
    else
    {
        fragColor1 = vec4(0);
        fragColor2 = vec4(0);
    }


}