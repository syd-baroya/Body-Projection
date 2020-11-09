#version 430

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoords;
layout(location = 2) in vec4 vertexColor;
layout(location = 3) in vec3 vertexOffset;
layout(location = 4) in int animatePix;
layout(location = 5) in mat4 pixelTrans;
//layout(location = 5) in ivec2 pixelLocation;

out vec4 fragmentColor;
out vec2 fragTexCoords;
flat out int fragAnim;

uniform mat4 view;
uniform mat4 projection;
uniform bool enableShading;

layout(rg32f, binding = 0) restrict readonly uniform image2D xyTable;
layout(r16ui, binding = 1) restrict readonly uniform uimage2D depth;


void main()
{
        gl_Position = projection * view * vec4(vertexOffset, 1);

//        gl_Position = projection * view * pixelTrans * vec4(vertexPosition, 1);
//    if(animatePix==0)
//        gl_Position = projection * view * vec4(vertexPosition + vertexOffset, 1);
    fragmentColor = vertexColor;

    fragTexCoords = vertexTexCoords;

    fragAnim = animatePix;
}
