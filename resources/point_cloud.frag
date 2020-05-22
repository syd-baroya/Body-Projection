#version 430
out vec4 fragColor;
layout(location = 0) uniform sampler2D scene_tex;


uniform float totaltime;

in vec4 fragmentColor;
in vec2 fragTexCoords;
void main()
{
    fragColor = fragmentColor;

//    if(fragColor.rgb != vec3(0))
//    {
//        vec2 texcoords = fragTexCoords;
//        fragColor.rgb= texture(scene_tex, texcoords).rgb;
//        fragColor.a = 1;
//    }
}