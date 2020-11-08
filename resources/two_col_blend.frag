#version 430 core

in vec2 fragTex;

layout(location = 0) out vec4 color;

layout(location = 0) uniform sampler2D color1;
layout(location = 1) uniform sampler2D color2;

void main()

{
	vec4 t0 = texture2D(color1, fragTex);
    vec4 t1 = texture2D(color2, fragTex) ;
    float blend_factor = 0.75f;
    if(t0.rgb==vec3(0) && t1.rgb!=vec3(0))
        blend_factor = 0.0f;
    else if(t0.rgb!=vec3(0) && t1.rgb==vec3(0))
        blend_factor = 1.0f;
    color = t0 * blend_factor + t1* (1.0f - blend_factor);


}