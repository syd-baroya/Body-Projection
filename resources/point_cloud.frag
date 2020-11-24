#version 430
out vec4 fragColor1;
//out vec4 fragColor2; //uncomment if highlighting outline

layout(location = 0) uniform sampler2D scene_tex;

uniform vec4 texoffset;
uniform float totaltime;

in vec4 fragmentColor;
in vec2 fragTexCoords;
flat in int fragAnim;


void main()
{
	vec4 fragColor = fragmentColor;

	//comment if running regular
	if(fragColor.rgb != vec3(0))
		fragColor1 = vec4(1,0,0,1);
	else
		fragColor1 = vec4(0,0,0,1);

	//uncomment if highlighting outline
//    if(fragColor.rgb != vec3(0))
//    {
//         if(fragColor.r >=0.9 && fragColor.g >= 0.9 && fragColor.b >= 0.9)
//         {
//
//            fragColor2 = vec4(0,0,0,0);
//            fragColor1 = vec4(1, 0, 0, 1);
//         }
//
//        else
//        {
//            fragColor1 = vec4(0,0,0,0);
//            fragColor2 = vec4(1, 0.967, .549, 1);
//        }
//        
//    }
//    else
//    {
//        fragColor1 = vec4(0);
//        fragColor2 = vec4(0);
//    }


}