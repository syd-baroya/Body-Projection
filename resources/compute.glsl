#version 450 
#extension GL_ARB_shader_storage_buffer_object : require
layout(local_size_x = 90, local_size_y = 16) in;	
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
  vec4 colorOutput[ 320 * 288];

};

uniform int sizeofbuffer;
void main() 
{
	uint lid_index_x = gl_LocalInvocationID.x;
	uint lid_index_y = gl_LocalInvocationID.y;
	uint gid_index_x = gl_GlobalInvocationID.x;
	uint gid_index_y = gl_GlobalInvocationID.y;


	uint index = ( gid_index_x * 90 ) + lid_index_x + ( gid_index_y * 90 * 72 * 4 ) + ( lid_index_y * 90 * 4 );


	if(index < sizeofbuffer )
	{

		if( colorInput[index].rgb != vec3(0))
		{
			vec4 up, down, left, right;
			up = down = left = right = vec4(1);

			//check if up is valid
			if( gid_index_y > 0 && lid_index_y > 0 )
				up = colorInput[ index - ( 4 * 90 ) ];
		
			//check if down is valid
			if( gid_index_y < 4 && lid_index_y < 72 )
				down = colorInput[ index + ( 4 * 90 ) ];

			//check if left is valid
			if( gid_index_x > 0 && lid_index_x > 0 )
				left = colorInput[ index - 1 ];
		
			//check if right is valid
			if( gid_index_x < 4 && lid_index_x < 90 )
				right = colorInput[ index + 1 ];

			if( (up.rgb == vec3(0)) || (down.rgb == vec3(0)) || (left.rgb == vec3(0)) || (right.rgb == vec3(0)) ) 
				colorOutput[index] = colorInput[index];

			else
				colorOutput[index] = vec4(0,0,0,1);

		}


	}
	//barrier();
}