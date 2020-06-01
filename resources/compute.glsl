#version 450 
#extension GL_ARB_shader_storage_buffer_object : require
layout(local_size_x = 40, local_size_y = 16) in;	
layout (binding = 0, offset = 0) uniform atomic_uint ac;

//	for texture handling
layout(rgba8, binding = 0) uniform image2D img;		//input/output image
//		vec4 col=imageLoad(img_input, pixel_coords);
//		imageStore(img_output, pixel_coords, col);


// K4A_DEPTH_MODE_WFOV_UNBINNED =====> pointCloudMaxSize = 1024 * 1024
// K4A_DEPTH_MODE_WFOV_2X2BINNED =====> pointCloudMaxSize = 512 * 512
// K4A_DEPTH_MODE_NFOV_2X2BINNED =====> pointCloudMaxSize = 320 * 288
// K4A_DEPTH_MODE_NFOV_UNBINNED =====> pointCloudMaxSize = 640 * 576
// default =====> pointCloudMaxSize = 1024 * 1024


//local group of shaders
layout (std430, binding=0) volatile buffer shader_data
{ 
  vec4 colorInput[  320 * 288 ];
  int outlineIndices[(320 * 288)/2];
//  vec2 pixelLife[320 * 288];
};

void main() 
{
	uint gid_index_x = gl_GlobalInvocationID.x;
	uint gid_index_y = gl_GlobalInvocationID.y;


	uint index = gid_index_x*288 + gid_index_y;


	if( colorInput[index].rgb != vec3(0))
	{
		vec4 up, down, left, right;
		up = down = left = right = vec4(1);

		//check if up is valid
		if( index >= 320)
			up = colorInput[ index - 320 ];
		
		//check if down is valid
		if( index < (320*288 -320) )
			down = colorInput[ index + 320 ];

		//check if left is valid
		if( index%320 != 0  )
			left = colorInput[ index - 1 ];
		
		//check if right is valid
		if( index%320 != 319 )
			right = colorInput[ index + 1 ];

		if( (up.rgb == vec3(0)) || (down.rgb == vec3(0)) || (left.rgb == vec3(0)) || (right.rgb == vec3(0)) ) 
		{
			outlineIndices[index] = int(index);

//			if(pixelLife[index].x <= -20.0f)
//				pixelLife[index].x = pixelLife[index].y;
//			else
//				pixelLife[index].x = pixelLife[index].x - 1.0f;
		}
		
	}
	

}