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

    if(fragmentColor.rgb != vec3(0))
    {
       
         if(fragAnim >= 0)
         {
            vec2 texcoord = vec2(fragTexCoords.x/texoffset.x, fragTexCoords.y/texoffset.y);
            if(fragAnim==1)
            {
                texcoord.x +=texoffset.z / texoffset.x;
                texcoord.y +=texoffset.w / texoffset.y;
            }
            fragColor1= texture(scene_tex, vec2(texcoord.x,texcoord.y));
            fragColor2 = vec4(0,0,0,0);
            if(fragColor1.rgb == vec3(0))
                discard;
         }

        else
        {
            fragColor1 = vec4(0,0,0,0);
            fragColor2 = vec4(1, 0.967, .549, 1);
        }
        
    }
    else{
        discard;
    }

}