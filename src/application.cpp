#include "application.h"



					/*Note that any gl calls must always happen after a GL state is initialized */
					void init_atomic()
					{
						glGenBuffers(1, &atomicsBuffer);
						// bind the buffer and define its initial storage capacity
						glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer);
						glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * 1, NULL, GL_DYNAMIC_DRAW);
						// unbind the buffer 
						glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
					}
					void reset_atomic()
					{
						GLuint* userCounters;
						glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer);
						// map the buffer, userCounters will point to the buffers data
						userCounters = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER,
							0,
							sizeof(GLuint),
							GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT
						);
						// set the memory to zeros, resetting the values in the buffer
						memset(userCounters, 0, sizeof(GLuint));
						// unmap the buffer
						glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
					}
					void read_atomic()
					{
						GLuint* userCounters;
						glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer);
						// again we map the buffer to userCounters, but this time for read-only access
						userCounters = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER,
							0,
							sizeof(GLuint),
							GL_MAP_READ_BIT
						);
						// copy the values to other variables because...
						//cout << endl << *userCounters << endl;
						// ... as soon as we unmap the buffer
						// the pointer userCounters becomes invalid.
						glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
					}

					void create_SSBO() {
						for (int i = 0; i < ssbo_size; i++) {
							ssbo_CPUMEM.positions_list[i] = ivec4(i, 0, 0, 0);
						}

						glGenBuffers(1, &ssbo_GPU_id);
						glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_GPU_id);
						glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ssbo_data), &ssbo_CPUMEM, GL_DYNAMIC_DRAW);
						glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_GPU_id);
						glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
					}

					void get_SSBO_back() {
						// Get SSBO back
						glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_GPU_id);
						GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
						int siz = sizeof(ssbo_data);
						memcpy(&ssbo_CPUMEM, p, siz);
						glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
					}


					void roll_dice()
					{

						int scenerand = rand() % 3;
						if (scenerand == 0)scenemode = SCENE_BUTTERFLY;
						if (scenerand == 1)scenemode = SCENE_SCELETONHEART;
						if (scenerand == 2)scenemode = SCENE_FUR;



						animation = false;
						fur_phase_total_time = 0;
						phaseprogresstotaltime = 0;

						redtone = normalize(vec3(frand(), frand(), frand()));
						greentone = normalize(vec3(frand(), frand(), frand()));
						bluetone = normalize(vec3(frand(), frand(), frand()));

						vec3 colorscaling = normalize(vec3(frand(), frand(), frand()));
						redtone *= colorscaling.x;
						greentone *= colorscaling.y;
						bluetone *= colorscaling.z;

						for (int ii = 0; ii < BUTTERFLYCOUNT; ii++)
						{
							butterfly[ii].rotz = frand() * 3.1415926 * 2.0;
							butterfly[ii].scale = frand() * 0.05;
							butterfly[ii].red = vec3(frand(), frand(), frand());
							butterfly[ii].green = vec3(frand(), frand(), frand());
							butterfly[ii].blue = vec3(frand(), frand(), frand());
							butterfly[ii].startanim = 1 + frand() * 4;
						}

						//torso
						int c = 13;
						butterfly[c].iA = 20;		butterfly[c].iB = 12;		butterfly[c].scale += 0.13;		butterfly[c].rationAB = 0.6; c++;
						butterfly[c].iA = 1;		butterfly[c].iB = 8;		butterfly[c].scale += 0.07;		butterfly[c].rationAB = 0.3; c++;
						butterfly[c].iA = 1;		butterfly[c].iB = 8;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.9; c++;
						butterfly[c].iA = 20;		butterfly[c].iB = 8;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.2; c++;
						butterfly[c].iA = 4;		butterfly[c].iB = 1;		butterfly[c].scale += 0.06;		butterfly[c].rationAB = 0.7; c++;
						butterfly[c].iA = 4;		butterfly[c].iB = 20;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.5; c++;
						//head:
						butterfly[c].iA = 3;		butterfly[c].iB = 2;		butterfly[c].scale += 0.06;		butterfly[c].rationAB = 0.4; c++;
						//hip
						butterfly[c].iA = 16;		butterfly[c].iB = 16;		butterfly[c].scale += 0.04;		butterfly[c].rationAB = 0.5; c++;
						//rightleg
						butterfly[c].iA = 16;		butterfly[c].iB = 17;		butterfly[c].scale += 0.07;		butterfly[c].rationAB = 0.5; c++;
						butterfly[c].iA = 17;		butterfly[c].iB = 18;		butterfly[c].scale += 0.04;		butterfly[c].rationAB = 0.2; c++;
						butterfly[c].iA = 17;		butterfly[c].iB = 18;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.5; c++;
						butterfly[c].iA = 17;		butterfly[c].iB = 18;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.9; c++;
						butterfly[c].iA = 18;		butterfly[c].iB = 19;		butterfly[c].scale += 0.04;		butterfly[c].rationAB = 0.5; c++;

						//leftleg
						butterfly[c].iA = 12;		butterfly[c].iB = 13;		butterfly[c].scale += 0.06;		butterfly[c].rationAB = 0.6; c++;
						butterfly[c].iA = 12;		butterfly[c].iB = 13;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.2; c++;
						butterfly[c].iA = 13;		butterfly[c].iB = 14;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.2;	c++;
						butterfly[c].iA = 13;		butterfly[c].iB = 14;		butterfly[c].scale += 0.05;		butterfly[c].rationAB = 0.5;	c++;
						butterfly[c].iA = 14;		butterfly[c].iB = 15;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.8;	c++;
						butterfly[c].iA = 14;		butterfly[c].iB = 15;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.2;	c++;
						//0-20 till here:21

						//right hand
						butterfly[c].iA = 8;		butterfly[c].iB = 9;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.4;	c++;
						butterfly[c].iA = 8;		butterfly[c].iB = 9;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.2;	c++;
						butterfly[c].iA = 8;		butterfly[c].iB = 9;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.9;	c++;
						butterfly[c].iA = 9;		butterfly[c].iB = 10;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.5;	c++;
						butterfly[c].iA = 10;		butterfly[c].iB = 10;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.0;	c++;
						butterfly[c].iA = 10;		butterfly[c].iB = 11;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.5;	c++;

						//left hand
						butterfly[c].iA = 4;		butterfly[c].iB = 5;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.6;	c++;
						butterfly[c].iA = 5;		butterfly[c].iB = 6;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.3;	c++;
						butterfly[c].iA = 5;		butterfly[c].iB = 6;		butterfly[c].scale += 0.03;		butterfly[c].rationAB = 0.5;	c++;
						butterfly[c].iA = 6;		butterfly[c].iB = 7;		butterfly[c].scale += 0.02;		butterfly[c].rationAB = 0.5;	c++;
						//here 31
						butterflyactual = c;
					}

					void Close_Kinect()
					{
						body.CloseSensor();
					}

bool Update_Kinect(float frametime)
{

	int trackedbodies = 0;
#ifdef NOKINECT
	get_record(&body, &body.trackedbody);
	trackedbodies = body.trackedbody.size();
#endif
#ifndef NOKINECT
	trackedbodies = body.Update(frametime);
#endif
	if (body.getNumBodies() > 0)
	{
		for (int i = 0; i < body.getDeviceCount(); i++) {
			new_trackedbody_* tb = &body.trackedbody.at(i);
			generate_joint_angles(&tb->jointAngleMap, *tb);
		}
		std::map<k4abt_joint_id_t, long double> avg_angles = average_all_joint_angles(body.trackedbody);
		std::map < k4abt_joint_id_t, vector<k4abt_joint_id_t>> angleHierarchy;
		angle_hierarchy(&angleHierarchy);
		vec3 master_positions[K4ABT_JOINT_COUNT];
		copy(begin(body.trackedbody.at(0).joint_positions), end(body.trackedbody.at(0).joint_positions), begin(master_positions));

		std::cout << "Length of array = " << (sizeof(master_positions) / sizeof(*master_positions)) << std::endl;

		for (pair< k4abt_joint_id_t, long double> element : avg_angles)
		{
			double theta = 0;

			long double new_angle = avg_angles[element.first];
			long double old_angle = body.trackedbody.at(0).jointAngleMap[element.first];

			if (new_angle > old_angle)
				theta = new_angle - old_angle;
			else
				theta = old_angle - new_angle;

			k4abt_joint_id_t joint_for_angle = element.first;
			k4abt_joint_id_t joint_for_position = element.first;

			vec3 new_master_joint_position = vec3(coord_after_rotation(vec2(master_positions[joint_for_position].x, master_positions[joint_for_position].y), vec2(master_positions[joint_for_angle].x, master_positions[joint_for_angle].y), theta, new_angle), master_positions[joint_for_position].z);

			vec3 change_in_joint_position = master_positions[joint_for_position] - new_master_joint_position;

			for (k4abt_joint_id_t hierarchy_joints : angleHierarchy[element.first])
			{
				master_positions[hierarchy_joints] = master_positions[hierarchy_joints] - change_in_joint_position;
			}

		}
		copy(begin(master_positions), end(master_positions), begin(body.trackedbody.at(0).joint_positions));
	}
	vector<vec3> temp_posb;

	generate_body_vertices(&body.trackedbody.at(0), &temp_posb, app_posb);

	app_posb = temp_posb;

	glBindBuffer(GL_ARRAY_BUFFER, VBbody);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * temp_posb.size(), temp_posb.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return trackedbodies;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (key == GLFW_KEY_F1 && action == GLFW_RELEASE) { runmode = RUNFIRE; roll_dice(); }
	if (key == GLFW_KEY_F2 && action == GLFW_RELEASE) { runmode = RUN_NOFIRE; roll_dice(); }
	if (key == GLFW_KEY_F3 && action == GLFW_RELEASE) { runmode = RUN_DEBUGFIRE; roll_dice(); }

	if (key == GLFW_KEY_6 && action == GLFW_RELEASE) { roll_dice(); scenemode = SCENE_SCELETONHEART; }
	if (key == GLFW_KEY_7 && action == GLFW_RELEASE) { roll_dice(); scenemode = SCENE_BUTTERFLY; }
	if (key == GLFW_KEY_8 && action == GLFW_RELEASE) { roll_dice(); scenemode = SCENE_LINES; }
	if (key == GLFW_KEY_9 && action == GLFW_RELEASE) { roll_dice(); scenemode = SCENE_SCELETON; }
	if (key == GLFW_KEY_0 && action == GLFW_RELEASE) { roll_dice(); scenemode = SCENE_FUR; }

	if (key == GLFW_KEY_T && action == GLFW_RELEASE)
	{
		string resourceDirectory = "../resources";
		string str = resourceDirectory + "/skeleton.png";
		char filepath[1000];
		strcpy(filepath, str.c_str());
		int width, height, channels;
		unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
		glDeleteTextures(1, &TextureSkeletonH);
		TextureSkeletonH = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		delete[] data;
	}

	static float val = 0.01;
	static float mul = 1.05;




	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		val = 0.006;
		mul = 1.05;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		val = 0.05;
		mul = 1.2;
	}
	if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
	{
		correctmode = CORR_SHIFT;
	}
	if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
	{
		correctmode = CORR_SCALE;
	}
	if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
	{
		correctmode = CORR_SCEWSCALEUP;
	}
	/*	if (key == GLFW_KEY_4 && action == GLFW_RELEASE)
			{
			correctmode = CORR_SCEWSCALEDOWN;
			}*/


	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		correctmode = SCALE_TORSO_WIDTH;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		correctmode = SCALE_TORSO_HEIGHT;
	}

	if (key == GLFW_KEY_C && action == GLFW_RELEASE)
	{
		correctmode = SCALE_ARM;
	}
	if (key == GLFW_KEY_V && action == GLFW_RELEASE)
	{
		correctmode = SCALE_LEG;
	}
	if (key == GLFW_KEY_B && action == GLFW_RELEASE)
	{
		correctmode = SCALE_FOOT;
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		correctmode = SCALE_HEAD;
	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		correctmode = SHIFT_CHIN;
	}
	if (key == GLFW_KEY_J && action == GLFW_RELEASE)
	{
		correctmode = SHIFT_SIDE_CHIN;
	}


	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		switch (correctmode)
		{
		default:
		case CORR_SHIFT:			modelpos.y += val;	break;
		case CORR_SCALE:			modelscale.y += val; break;
		case SCALE_TORSO_WIDTH:		torso_width_scale += val;	break;
		case SCALE_TORSO_HEIGHT:	torso_height_scale += val; break;
		case SCALE_ARM:				arm_thickness_scale += val;	break;
		case SCALE_LEG:				leg_thickness_scale += val; break;
		case SCALE_FOOT:			foot_thickness_scale += val;	break;
		case SCALE_HEAD:			head_thickness_scale += val; break;
		case SHIFT_CHIN:			chinstart += val;	break;
		case SHIFT_SIDE_CHIN:		sidechinstart += val; break;
		}
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		switch (correctmode)
		{
		default:
		case CORR_SHIFT:			modelpos.y -= val;	break;
		case CORR_SCALE:			modelscale.y -= val; break;
		case SCALE_TORSO_WIDTH:		torso_width_scale -= val;	break;
		case SCALE_TORSO_HEIGHT:	torso_height_scale -= val; break;
		case SCALE_ARM:				arm_thickness_scale -= val;	break;
		case SCALE_LEG:				leg_thickness_scale -= val; break;
		case SCALE_FOOT:			foot_thickness_scale -= val;	break;
		case SCALE_HEAD:			head_thickness_scale -= val; break;
		case SHIFT_CHIN:			chinstart -= val;	break;
		case SHIFT_SIDE_CHIN:		sidechinstart -= val; break;
		}
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		switch (correctmode)
		{
		default:
		case CORR_SHIFT:	modelpos.x -= val;	break;
		case CORR_SCALE:	modelscale.x -= val; break;
		}
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		switch (correctmode)
		{
		default:
		case CORR_SHIFT:	modelpos.x += val;	break;
		case CORR_SCALE:	modelscale.x += val; break;
		}
	}
	if (key == GLFW_KEY_HOME && action == GLFW_RELEASE)
	{
		rectpos.clear();
		rectpos.push_back(vec3(-1.0, -1.0, 0.0));
		rectpos.push_back(vec3(1.0, -1.0, 0.0));
		rectpos.push_back(vec3(-1.0, 1.0, 0.0));
		rectpos.push_back(vec3(1.0, -1.0, 0.0));
		rectpos.push_back(vec3(1.0, 1.0, 0.0));
		rectpos.push_back(vec3(-1.0, 1.0, 0.0));
		update_postproc_rect();
	}
	if (key == GLFW_KEY_END && action == GLFW_RELEASE)
	{

		/*rectpos.push_back(vec3(-1.0, -1.0, 0.0));
		rectpos.push_back(vec3(1.0, -1.0, 0.0));
		rectpos.push_back(vec3(-1.0, 1.0, 0.0));
		rectpos.push_back(vec3(1.0, -1.0, 0.0));
		rectpos.push_back(vec3(1.0, 1.0, 0.0));
		rectpos.push_back(vec3(-1.0, 1.0, 0.0));
		*/
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		mycam.w = 1;
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		mycam.w = 0;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		mycam.s = 1;
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		mycam.s = 0;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		mycam.a = 1;
	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		mycam.a = 0;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		mycam.d = 1;
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		mycam.d = 0;
	}


	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		mycam.q = 1;
	}
	if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
	{
		mycam.q = 0;
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		mycam.e = 1;
	}
	if (key == GLFW_KEY_E && action == GLFW_RELEASE)
	{
		mycam.e = 0;
	}
	if (key == GLFW_KEY_O && action == GLFW_RELEASE)
	{
		camfov -= 0.2;
	}
	if (key == GLFW_KEY_P && action == GLFW_RELEASE)
	{
		camfov += 0.2;
	}
	if (key == GLFW_KEY_F && action == GLFW_RELEASE)
	{
		fullscreen = !fullscreen;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		//mycam.holdcamera();
		ofstream f("pos.dat");
		if (f.is_open() == false) return;
		f << modelpos.x << endl;
		f << modelpos.y << endl;
		f << modelpos.z << endl;
		f << modelscale.x << endl;
		f << modelscale.y << endl;
		f << modelscale.z << endl;
		f << camfov << endl;

		f.close();
	}
	if (key == GLFW_KEY_K && action == GLFW_RELEASE)
	{
		record();
	}
	if (key == GLFW_KEY_L && action == GLFW_RELEASE)
	{
		animation = true;
	}
	if (key == GLFW_KEY_M && action == GLFW_RELEASE)
	{
		cout << "OLD DEVICE COUNT: " << body.getDeviceCount() << endl;

		if (body.getDeviceCount() != 1)
			body.setDeviceCount(1);
		else
		{
#ifdef NOKINECT
			ifstream f("anim.txt");
			if (f.is_open() == false)return;
			static uint32_t deviceCount;
			f >> deviceCount;
			body.setDeviceCount(deviceCount);
			f.close();
#endif
#ifndef NOKINECT
			body.setDeviceCount(k4a_device_get_installed_count());
#endif
		}
		cout << "NEW DEVICE COUNT: " << body.getDeviceCount() << endl;
	}
}
void record()
{
	ofstream f;
	f.open("anim.txt");
	f << body.getDeviceCount() << '\n';
	for (int ii = 0; ii < K4ABT_JOINT_COUNT; ii++)
	{
		for (int i = 0; i < body.getDeviceCount(); i++) {
			vec3 t = body.trackedbody.at(i).new_get_joint(FORECASTFACT, ii);
			f << t.x << t.y << t.z;
		}
	}
	f.close();
}
void get_record(new_body_* body, vector<new_trackedbody_>* tracked_body)
{
	static vector<vec3> pos;
	static uint32_t deviceCount;
	static bool first = true;
	if (first)
	{
		ifstream f("anim.txt");
		if (f.is_open() == false)return;
		first = false;
		f >> deviceCount;
		body->setDeviceCount(deviceCount);
		for (uint32_t tracked_count = 0; tracked_count < deviceCount; tracked_count++) {
			tracked_body->push_back(new_trackedbody_());
		}
		for (uint32_t ii = 0; ii < K4ABT_JOINT_COUNT * deviceCount; ii++)
		{
			vec3 file_vec;
			f >> file_vec.x;
			f >> file_vec.y;
			f >> file_vec.z;
			pos.push_back(file_vec);
		}
		f.close();
	}

	for (uint32_t ii = 0; ii < K4ABT_JOINT_COUNT; ii++)
	{
		for (uint32_t i = 0; i < deviceCount; i++)
		{
			int pos_index = (2 * ii) + i;
			tracked_body->at(i).joint_positions[ii] = pos.at(pos_index);
		}
	}

}
// callback for the mouse when clicked move the triangle when helper functions
// written
void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	double posX, posY;
	float newPt[2];

}

//if the window is resized, capture the new size and reset the viewport
void resizeCallback(GLFWwindow* window, int in_width, int in_height)
{
	glDeleteFramebuffers(1, &fb);
	glDeleteFramebuffers(1, &fbbut);


	glDeleteTextures(1, &FBOcolor);
	glDeleteTextures(1, &FBOcolorbut);
	glDeleteTextures(1, &FBOmask);

	glDeleteRenderbuffers(1, &depth_rbbut);
	glDeleteRenderbuffers(1, &depth_rb);

	generate_framebuffers();

	//get the window size - may be different then pixels for retina
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}
GLuint generate_texture2D(GLushort colortype, int width, int height, GLushort colororder, GLushort datatype, unsigned char* data, GLushort wrap, GLushort minfilter, GLushort magfilter)
{
	GLuint textureID;
	//RGBA8 2D texture, 24 bit depth texture, 256x256
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);
	glTexImage2D(GL_TEXTURE_2D, 0, colortype, width, height, 0, colororder, datatype, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	return textureID;
}
/*Note that any gl calls must always happen after a GL state is initialized */
void change(vec3& a, vec3& b)
{
	vec3 c = a;
	a = b;
	b = c;
}
////////////////////////
void generate_framebuffers()
{
	//create frame buffer
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	//RGBA8 2D texture, 24 bit depth texture, 256x256
	int width, height;
	glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

	FBOcolor = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, NULL, GL_CLAMP_TO_BORDER, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	FBOmask = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, NULL, GL_CLAMP_TO_BORDER, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	//Attach 2D texture to this FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOcolor, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, FBOmask, 0);
	//create depth buffer
	glGenRenderbuffers(1, &depth_rb);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
	//Attach depth buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb);
	//-------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//*******************
	//create frame buffer TWO
	glGenFramebuffers(1, &fbbut);
	glBindFramebuffer(GL_FRAMEBUFFER, fbbut);
	//RGBA8 2D texture, 24 bit depth texture, 256x256

	glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

	FBOcolorbut = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, NULL, GL_CLAMP_TO_BORDER, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	//Attach 2D texture to this FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOcolorbut, 0);
	//create depth buffer
	glGenRenderbuffers(1, &depth_rbbut);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_rbbut);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
	//Attach depth buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rbbut);
	//-------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
//////////////////////////
void initGeom()
{
						roll_dice();
					#ifndef NOKINECT
						body.InitializeDefaultSensor();
					#endif
						vector<GLushort> indices;

						/***BODY WITHOUT CLAVICLES***/
						//torso w/o clavicles
						indices.push_back(0);	indices.push_back(2);	indices.push_back(3);
						indices.push_back(0);	indices.push_back(3);	indices.push_back(1);
						indices.push_back(2);	indices.push_back(4);	indices.push_back(5);
						indices.push_back(2);	indices.push_back(5);	indices.push_back(3);
						indices.push_back(5);	indices.push_back(6);	indices.push_back(3);
						indices.push_back(4);	indices.push_back(7);	indices.push_back(10);
						indices.push_back(4);	indices.push_back(10);	indices.push_back(5);
						indices.push_back(10);	indices.push_back(8);	indices.push_back(5);
						indices.push_back(8);	indices.push_back(11);	indices.push_back(5);
						indices.push_back(5);	indices.push_back(11);	indices.push_back(6);
						indices.push_back(6);	indices.push_back(11);	indices.push_back(9);
						indices.push_back(12);	indices.push_back(14);	indices.push_back(13);
						indices.push_back(14);	indices.push_back(15);	indices.push_back(13);
						indices.push_back(14);	indices.push_back(7);	indices.push_back(4);
						indices.push_back(14);	indices.push_back(4);	indices.push_back(15);
						//arms
						indices.push_back(4);	indices.push_back(2);	indices.push_back(15);
						indices.push_back(16);	indices.push_back(18);	indices.push_back(19);
						indices.push_back(16);	indices.push_back(19);	indices.push_back(17);
						indices.push_back(18);	indices.push_back(3);	indices.push_back(6);
						indices.push_back(18);	indices.push_back(6);	indices.push_back(19);
						indices.push_back(6);	indices.push_back(9);	indices.push_back(19);
						indices.push_back(20);	indices.push_back(22);	indices.push_back(21);
						indices.push_back(22);	indices.push_back(23);	indices.push_back(21);
						//legs
						indices.push_back(22);	indices.push_back(24);	indices.push_back(23);
						indices.push_back(24);	indices.push_back(25);	indices.push_back(23);
						indices.push_back(24);	indices.push_back(0);	indices.push_back(25);
						indices.push_back(0);	indices.push_back(26);	indices.push_back(25);
						indices.push_back(0);	indices.push_back(1);	indices.push_back(26);
						indices.push_back(27);	indices.push_back(30);	indices.push_back(28);
						indices.push_back(27);	indices.push_back(29);	indices.push_back(30);
						indices.push_back(29);	indices.push_back(31);	indices.push_back(32);
						indices.push_back(29);	indices.push_back(32);	indices.push_back(30);
						indices.push_back(31);	indices.push_back(1);	indices.push_back(32);
						indices.push_back(32);	indices.push_back(26);	indices.push_back(1);
						indices.push_back(10);	indices.push_back(34);	indices.push_back(33);
						indices.push_back(10);	indices.push_back(33);	indices.push_back(8);
						//head
						indices.push_back(8);	indices.push_back(33);	indices.push_back(11);
						indices.push_back(33);	indices.push_back(35);	indices.push_back(11);
						indices.push_back(33);	indices.push_back(34);	indices.push_back(35);
						indices.push_back(34);	indices.push_back(36);	indices.push_back(37);
						indices.push_back(34);	indices.push_back(37);	indices.push_back(35);
						//continue from 38
						//hands
						//indices.push_back(38);	indices.push_back(12);	indices.push_back(13);
						//indices.push_back(38);	indices.push_back(13);	indices.push_back(39);
						//indices.push_back(40);	indices.push_back(16);	indices.push_back(41);
						//indices.push_back(16);	indices.push_back(17);	indices.push_back(41);


						/***BODY INDICES WITH CLAVICLES***/
						////torso w/o clavicles
						//indices.push_back(0);	indices.push_back(2);	indices.push_back(3);
						//indices.push_back(0);	indices.push_back(3);	indices.push_back(1);
						//indices.push_back(2);	indices.push_back(4);	indices.push_back(5); //mtl, shoulderleft, clavicleleft
						//indices.push_back(2);	indices.push_back(5);	indices.push_back(3); //mtl, clavicleleft, mtr
						//indices.push_back(3);	indices.push_back(6);	indices.push_back(2); //mtr, clavicleright, mtl
						//indices.push_back(3);	indices.push_back(7);	indices.push_back(6); //clavicleright, shoulderright, mtr
						//indices.push_back(4);	indices.push_back(8);	indices.push_back(11); //shoulderleft, utl, sll
						//indices.push_back(4);	indices.push_back(11);	indices.push_back(5); //shoulderleft, sll, clavicleleft
						//indices.push_back(11);	indices.push_back(9);	indices.push_back(5); //sll, neck, clavicleleft
						//indices.push_back(9);	indices.push_back(12);	indices.push_back(6); //neck, slr, clavicleright
						//indices.push_back(6);	indices.push_back(12);	indices.push_back(7); //clavicleright, slr, shoulderright
						//indices.push_back(7);	indices.push_back(12);	indices.push_back(10); //shoulderright, slr, utr
						//indices.push_back(13);	indices.push_back(15);	indices.push_back(14); 
						//indices.push_back(15);	indices.push_back(16);	indices.push_back(14);
						//indices.push_back(15);	indices.push_back(8);	indices.push_back(4);
						//indices.push_back(15);	indices.push_back(4);	indices.push_back(16);
						////arms
						//indices.push_back(4);	indices.push_back(2);	indices.push_back(16);
						//indices.push_back(17);	indices.push_back(19);	indices.push_back(20);
						//indices.push_back(17);	indices.push_back(20);	indices.push_back(18);
						//indices.push_back(19);	indices.push_back(3);	indices.push_back(7);
						//indices.push_back(19);	indices.push_back(7);	indices.push_back(20);
						//indices.push_back(7);	indices.push_back(10);	indices.push_back(20);
						//indices.push_back(21);	indices.push_back(23);	indices.push_back(22);
						//indices.push_back(23);	indices.push_back(24);	indices.push_back(22);
						////legs
						//indices.push_back(23);	indices.push_back(25);	indices.push_back(24);
						//indices.push_back(25);	indices.push_back(26);	indices.push_back(24);
						//indices.push_back(25);	indices.push_back(0);	indices.push_back(26);
						//indices.push_back(0);	indices.push_back(27);	indices.push_back(26);
						//indices.push_back(0);	indices.push_back(1);	indices.push_back(27);
						//indices.push_back(28);	indices.push_back(31);	indices.push_back(29);
						//indices.push_back(28);	indices.push_back(30);	indices.push_back(31);
						//indices.push_back(30);	indices.push_back(32);	indices.push_back(33);
						//indices.push_back(30);	indices.push_back(33);	indices.push_back(31);
						//indices.push_back(32);	indices.push_back(1);	indices.push_back(33);
						//indices.push_back(33);	indices.push_back(27);	indices.push_back(1);
						//indices.push_back(11);	indices.push_back(35);	indices.push_back(34);
						//indices.push_back(11);	indices.push_back(34);	indices.push_back(9);
						////head
						//indices.push_back(9);	indices.push_back(34);	indices.push_back(12);
						//indices.push_back(34);	indices.push_back(36);	indices.push_back(12);
						//indices.push_back(34);	indices.push_back(35);	indices.push_back(36);
						//indices.push_back(35);	indices.push_back(37);	indices.push_back(38);
						//indices.push_back(35);	indices.push_back(38);	indices.push_back(36);
						////continue from 38
						////hands
						//indices.push_back(39);	indices.push_back(13);	indices.push_back(14);
						//indices.push_back(39);	indices.push_back(14);	indices.push_back(40);
						//indices.push_back(41);	indices.push_back(17);	indices.push_back(42);
						//indices.push_back(17);	indices.push_back(18);	indices.push_back(42);
						vector<vec2> tex;
						tex.push_back(vec2(0.357056, 0.417677));
						tex.push_back(vec2(0.63436, 0.424716));
						tex.push_back(vec2(0.281281, 0.654486));
						tex.push_back(vec2(0.668834, 0.651864));
						tex.push_back(vec2(0.344362, 0.668123));
						tex.push_back(vec2(0.483297, 0.704922));
						tex.push_back(vec2(0.606656, 0.666727));
						tex.push_back(vec2(0.363513, 0.704208));
						tex.push_back(vec2(0.481398, 0.742544));
						tex.push_back(vec2(0.594513, 0.703739));
						tex.push_back(vec2(0.445423, 0.730845));
						tex.push_back(vec2(0.512891, 0.73174));
						tex.push_back(vec2(0.163252, 0.925081));
						tex.push_back(vec2(0.055907, 0.893054));
						tex.push_back(vec2(0.260317, 0.808323));
						tex.push_back(vec2(0.152971, 0.776296));
						tex.push_back(vec2(0.94095, 0.878834));
						tex.push_back(vec2(0.839852, 0.916624));
						tex.push_back(vec2(0.831066, 0.762144));
						tex.push_back(vec2(0.729968, 0.799934));
						tex.push_back(vec2(0.329756, 0.00389565));
						tex.push_back(vec2(0.426757, 0));
						tex.push_back(vec2(0.340259, 0.0597313));
						tex.push_back(vec2(0.43726, 0.0558356));
						tex.push_back(vec2(0.306178, 0.261854));
						tex.push_back(vec2(0.50028, 0.25486));
						tex.push_back(vec2(0.496964, 0.387029));
						tex.push_back(vec2(0.67537, 0.00663857));
						tex.push_back(vec2(0.57814, 0.00542157));
						tex.push_back(vec2(0.676887, 0.0588956));
						tex.push_back(vec2(0.579657, 0.0576786));
						tex.push_back(vec2(0.703275, 0.260904));
						tex.push_back(vec2(0.50992, 0.249145));
						tex.push_back(vec2(0.475392, 0.778904));
						tex.push_back(vec2(0.411582, 0.79506));
						tex.push_back(vec2(0.533196, 0.799108));
						tex.push_back(vec2(0.402573, 0.8496));
						tex.push_back(vec2(0.524187, 0.853647));
						tex.push_back(vec2(0.088382, 1));
						tex.push_back(vec2(0, 0.977442));
						tex.push_back(vec2(1, 0.962063));
						tex.push_back(vec2(0.913081, 0.986313));


						vector<vec3> temp_posb;
						generate_body_vertices(&body.trackedbody.at(0), &temp_posb, app_posb);
						app_posb = temp_posb;


						GLuint VB;

						glGenVertexArrays(1, &VAObody);
						glBindVertexArray(VAObody);

						glGenBuffers(1, &VBbody);
						glBindBuffer(GL_ARRAY_BUFFER, VBbody);
						glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * temp_posb.size(), temp_posb.data(), GL_DYNAMIC_DRAW);
						vertexcount = temp_posb.size();
						glEnableVertexAttribArray(0);
						glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
						glGenBuffers(1, &VB);
						glBindBuffer(GL_ARRAY_BUFFER, VB);
						glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * tex.size(), tex.data(), GL_DYNAMIC_DRAW);
						glEnableVertexAttribArray(1);
						glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
						glGenBuffers(1, &VB);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VB);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), indices.data(), GL_STATIC_DRAW);
						body_size = indices.size();
						glBindVertexArray(0);




						string resourceDirectory = "../resources";
						// Initialize mesh.




						int width, height, channels;
						char filepath[1000];

						//texture 1
						string str = resourceDirectory + "/skeleton.jpg";
						strcpy(filepath, str.c_str());
						unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureSkeleton = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

						str = resourceDirectory + "/shead.png";
						strcpy(filepath, str.c_str());
						data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureSkeletonHead = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

						str = resourceDirectory + "/red.jpg";
						strcpy(filepath, str.c_str());
						data = stbi_load(filepath, &width, &height, &channels, 4);
						TexRed = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

						str = resourceDirectory + "/skeleton.png";
						strcpy(filepath, str.c_str());
						data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureSkeletonH = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

						str = resourceDirectory + "/heart2.png";
						strcpy(filepath, str.c_str());
						data = stbi_load(filepath, &width, &height, &channels, 4);
						TexHeart = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


						str = resourceDirectory + "/fur.jpg";		strcpy(filepath, str.c_str());		data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureSkin[0] = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						str = resourceDirectory + "/snake.jpg";		strcpy(filepath, str.c_str());		data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureSkin[1] = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						str = resourceDirectory + "/zebra.jpg";		strcpy(filepath, str.c_str());		data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureSkin[2] = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						str = resourceDirectory + "/chameleon.jpg";		strcpy(filepath, str.c_str());		data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureSkin[3] = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						str = resourceDirectory + "/chameleon2.jpg";		strcpy(filepath, str.c_str());		data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureSkin[4] = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						str = resourceDirectory + "/chameleon3.jpg";		strcpy(filepath, str.c_str());		data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureSkin[5] = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						str = resourceDirectory + "/gecko.jpg";		strcpy(filepath, str.c_str());		data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureSkin[6] = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


						str = resourceDirectory + "/lines.jpg";
						strcpy(filepath, str.c_str());
						data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureLines = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


						//texture 2
						str = resourceDirectory + "/butterfly.png";
						strcpy(filepath, str.c_str());
						data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureButterfly = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						//t3
						str = resourceDirectory + "/firering_a.jpg";
						strcpy(filepath, str.c_str());
						data = stbi_load(filepath, &width, &height, &channels, 4);
						TextureAlpha = generate_texture2D(GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, GL_CLAMP_TO_BORDER, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


						//texture array	
						glGenTextures(1, &TextureArray);
						glActiveTexture(GL_TEXTURE2);
						glBindTexture(GL_TEXTURE_2D_ARRAY, TextureArray);
						glTexStorage3D(GL_TEXTURE_2D_ARRAY, 7, GL_RGBA8, 512, 512, 25);
						std::vector<unsigned char> buffer(512 * 512 * 4 * 25);
						int sizepicoffset = 0;
						char txt[1000];
						for (int ii = 0; ii < 25; ii++)
						{
							sprintf(txt, "../resources/firering_%.3d.png", ii + 1);
							data = stbi_load(txt, &width, &height, &channels, 4);
							if (data == NULL)
							{
								cout << "data is null" << endl;
								return;
							}
							int sizepic = width * height * 4;
							memcpy(&buffer[sizepicoffset], data, sizepic);
							sizepicoffset = sizepic;
							glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
								0,                     //Mipmap number
								0, 0, ii,                 //xoffset, yoffset, zoffset
								width, height, 1,                 //width, height, depth
								GL_RGBA,                //format
								GL_UNSIGNED_BYTE,      //type
								data);
						}
						glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
						glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
						glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glGenVertexArrays(1, &VAOrect);
	glBindVertexArray(VAOrect);

	vector<vec3> pos;




	pos.push_back(vec3(-1, -1, 0));
	pos.push_back(vec3(-1, 1, 0));
	pos.push_back(vec3(1, 1, 0));
	pos.push_back(vec3(-1, -1, 0));
	pos.push_back(vec3(1, 1, 0));
	pos.push_back(vec3(1, -1, 0));

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 6, pos.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindVertexArray(0);

	//[TWOTEXTURES]
	GLuint TexLoc;
	//set the 2 textures to the correct samplers in the fragment shader:
	glUseProgram(prog->pid);
	TexLoc = glGetUniformLocation(prog->pid, "tex");	glUniform1i(TexLoc, 0);
	TexLoc = glGetUniformLocation(prog->pid, "tex2");	glUniform1i(TexLoc, 1);
	TexLoc = glGetUniformLocation(prog->pid, "texA");	glUniform1i(TexLoc, 2);
	TexLoc = glGetUniformLocation(prog->pid, "texarr");	glUniform1i(TexLoc, 3);

	glUseProgram(postprog->pid);
	TexLoc = glGetUniformLocation(postprog->pid, "tex");	glUniform1i(TexLoc, 0);
	TexLoc = glGetUniformLocation(postprog->pid, "texmask");	glUniform1i(TexLoc, 1);

	glUseProgram(progbody->pid);
	TexLoc = glGetUniformLocation(progbody->pid, "tex");	glUniform1i(TexLoc, 0);
	TexLoc = glGetUniformLocation(progbody->pid, "tex2");	glUniform1i(TexLoc, 1);


	generate_framebuffers();

	//*******************
	glGenVertexArrays(1, &VAO_postprocrect);
	glBindVertexArray(VAO_postprocrect);
	glGenBuffers(1, &VBrect);
	glBindBuffer(GL_ARRAY_BUFFER, VBrect);

	// front
	int verccount = 0;

	rectpos.push_back(vec3(-1.0, -1.0, 0.0));
	rectpos.push_back(vec3(1.0, -1.0, 0.0));
	rectpos.push_back(vec3(-1.0, 1.0, 0.0));
	rectpos.push_back(vec3(1.0, -1.0, 0.0));
	rectpos.push_back(vec3(1.0, 1.0, 0.0));
	rectpos.push_back(vec3(-1.0, 1.0, 0.0));

	glBufferData(GL_ARRAY_BUFFER, rectpos.size() * sizeof(vec3), rectpos.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	vector<vec2> recttex;

	recttex.push_back(vec2(0.0, 0.0));
	recttex.push_back(vec2(1.0, 0.0));
	recttex.push_back(vec2(0.0, 1.0));
	recttex.push_back(vec2(1.0, 0.0));
	recttex.push_back(vec2(1.0, 1.0));
	recttex.push_back(vec2(0.0, 1.0));

	//actually memcopy the data - only do this once
	glBufferData(GL_ARRAY_BUFFER, recttex.size() * sizeof(vec2), recttex.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindVertexArray(0);
	glGenVertexArrays(1, &VAO_rect);
	glBindVertexArray(VAO_rect);
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, rectpos.size() * sizeof(vec3), rectpos.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, recttex.size() * sizeof(vec2), recttex.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindVertexArray(0);

	//make an SSBO
	create_SSBO();
}

void update_postproc_rect()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBrect);
	glBufferSubData(GL_ARRAY_BUFFER, 0, rectpos.size() * sizeof(vec3), rectpos.data());
}
//General OGL initialization - set OGL state here
void init(const std::string& resourceDirectory)
{
#ifdef RELEASEVERSION
	runmode = RUNFIRE;
#else
	runmode = RUN_NOFIRE;
#endif

	ifstream f("pos.dat");
	if (f.is_open() == true)
	{
		f >> modelpos.x;
		f >> modelpos.y;
		f >> modelpos.z;
		f >> modelscale.x;
		f >> modelscale.y;
		f >> modelscale.z;
		f >> camfov;
		f.close();
	}
	GLSL::checkVersion();
#ifdef NOKINECT
	//modelpos.z = -1;
#endif

	// Set background color.

	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//next function defines how to mix the background color with the transparent pixel in the foreground. 
	//This is the standard:
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Initialize the GLSL program.
	prog = std::make_shared<Program>();
	prog->setVerbose(true);
	if (!prog->buildProgram(resourceDirectory + "/shader_vertex.glsl", resourceDirectory + "/shader_fragment.glsl"))
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
	prog->addUniform("P");
	prog->addUniform("V");
	prog->addUniform("M");
	prog->addUniform("firescale");
	prog->addUniform("texsplit");
	prog->addUniform("totaltime");
	prog->addAttribute("vertPos");
	prog->addAttribute("vertTex");

	progbut = std::make_shared<Program>();
	progbut->setVerbose(true);
	if (!progbut->buildProgram(resourceDirectory + "/but_vertex.glsl", resourceDirectory + "/but_fragment.glsl"))
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
	progbut->addUniform("P");
	progbut->addUniform("V");
	progbut->addUniform("M");
	progbut->addUniform("texsplit");
	progbut->addUniform("totaltime");
	progbut->addUniform("redmul");
	progbut->addUniform("greenmul");
	progbut->addUniform("bluemul");
	progbut->addAttribute("vertPos");
	progbut->addAttribute("vertTex");


	progbody = std::make_shared<Program>();
	progbody->setVerbose(true);
	if (!progbody->buildProgram(resourceDirectory + "/but_vertex.glsl", resourceDirectory + "/shader_body.glsl"))
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
	progbody->addUniform("texblend");
	progbody->addUniform("P");
	progbody->addUniform("V");
	progbody->addUniform("M");
	progbody->addUniform("texsplit");
	progbody->addUniform("totaltime");
	progbody->addUniform("redmul");
	progbody->addUniform("greenmul");
	progbody->addUniform("bluemul");
	progbody->addAttribute("vertPos");
	progbody->addAttribute("vertTex");


	postprog = std::make_shared<Program>();
	postprog->setVerbose(true);
	if (!postprog->buildProgram(resourceDirectory + "/vert.glsl", resourceDirectory + "/frag.glsl"))
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
	postprog->addAttribute("vertPos");
	postprog->addAttribute("vertTex");

	screenproc = std::make_shared<Program>();
	screenproc->setVerbose(true);
	if (!screenproc->buildProgram(resourceDirectory + "/vert.glsl", resourceDirectory + "/fragscreen.glsl"))
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
	screenproc->addAttribute("vertPos");
	screenproc->addAttribute("vertTex");

	//load the compute shader
	std::string ShaderString = readFileAsString(resourceDirectory + "/compute.glsl");
	const char* shader = ShaderString.c_str();
	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &shader, nullptr);

	GLint rc;
	CHECKED_GL_CALL(glCompileShader(computeShader));
	CHECKED_GL_CALL(glGetShaderiv(computeShader, GL_COMPILE_STATUS, &rc));
	if (!rc)	//error compiling the shader file
	{
		GLSL::printShaderInfoLog(computeShader);
		std::cout << "Error compiling fragment shader " << std::endl;
		exit(1);
	}

	computeProgram = glCreateProgram();
	glAttachShader(computeProgram, computeShader);
	glLinkProgram(computeProgram);
	glUseProgram(computeProgram);

	GLuint block_index = 0;
	block_index = glGetProgramResourceIndex(computeProgram, GL_SHADER_STORAGE_BLOCK, "shader_data");
	GLuint ssbo_binding_point_index = 2;
	glShaderStorageBlockBinding(computeProgram, block_index, ssbo_binding_point_index);


}

void compute()
{
	GLuint block_index = 0;
	block_index = glGetProgramResourceIndex(computeProgram, GL_SHADER_STORAGE_BLOCK, "shader_data");
	GLuint ssbo_binding_point_index = 0;
	glShaderStorageBlockBinding(computeProgram, block_index, ssbo_binding_point_index);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_GPU_id);
	glUseProgram(computeProgram);
	//activate atomic counter
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicsBuffer);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomicsBuffer);

	glDispatchCompute((GLuint)2, (GLuint)1, 1);				//start compute shader
	//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}
//***************************************************************************
int render_render_fire_to_screen_FBO(double frametime, mat4 P, mat4 V)
{

	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOcolor, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, FBOmask, 0);
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);

	glDisable(GL_DEPTH_TEST);


	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

	float aspect = width / (float)height;
	glViewport(0, 0, width, height);

	// Clear framebuffer.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 M;
	M = glm::mat4(1);


	glm::mat4 TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	M = TransZ * S;
	// Draw the box using GLSL.

	//send the matrices to the shaders



	int realwidth, realheight;
	glfwGetFramebufferSize(windowManager->getHandle(), &realwidth, &realheight);

	glViewport(0, 0, realwidth, realheight);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	screenproc->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOcolorbut);
	glBindVertexArray(VAO_rect);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	screenproc->unbind();




									prog->bind();

									vec2 texoff = vec2(0, 0);
									float tick = phaseprogresstotaltime * 25.;
									int texframe_x = (int)tick % 8;
									int texframe_y = (int)tick / 8;
									texoff.x = (float)texframe_x;
									texoff.y = (float)texframe_y;

									float firescale = 100. - pow(phaseprogresstotaltime, 0.06) * 92;
									if (firescale <= 0)
									{
										//reset
										firescale = 0.1;
										//roll_dice();
									}
									glUniform1f(prog->getUniform("firescale"), firescale);
									glUniform2fv(prog->getUniform("texsplit"), 1, &texoff.x);
									glUniform1f(prog->getUniform("totaltime"), phaseprogresstotaltime);
									glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P[0][0]);
									glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
									M = glm::translate(glm::mat4(1.0f), modelpos) * glm::scale(glm::mat4(1.0f), modelscale);
									glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);



									glBindVertexArray(VAObody);
									glActiveTexture(GL_TEXTURE0);		glBindTexture(GL_TEXTURE_2D, 0);
									glActiveTexture(GL_TEXTURE1);		glBindTexture(GL_TEXTURE_2D, 0);
									glActiveTexture(GL_TEXTURE2);		glBindTexture(GL_TEXTURE_2D, TextureAlpha);
									glActiveTexture(GL_TEXTURE3);		glBindTexture(GL_TEXTURE_2D_ARRAY, TextureArray);

									glDrawElements(GL_TRIANGLES, (int)body_size, GL_UNSIGNED_SHORT, (const void*)0);

									glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
									//glBindVertexArray(VAOrect);


									//for (int ii = 0; ii < JointType_Count; ii++)
									//	{
									//	vec3 t = body.joint_positions[ii];
									//	//t.z *= -1;
									//	mat4 Mr = M * translate(mat4(1), t * 1.0f) * scale(mat4(1), vec3(0.01, 0.01, 0.01));

									//	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &Mr[0][0]);
									//	glDrawArrays(GL_TRIANGLES, 0, 6);
									//	}
									prog->unbind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, FBOcolor);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, FBOmask);
	glGenerateMipmap(GL_TEXTURE_2D);
	/*static bool first = true;
	if (first)
		{
		glBindTexture(GL_TEXTURE_2D, FBOcolor);

		std::vector<unsigned char> buffer(width * height * 4);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
		char txt[1000];
		sprintf(txt, "outp.png", frame);
		stbi_write_png(txt, width, height, 4, buffer.data(), 0);
		first = false;
		}*/

	return 0;

}

/****DRAW
This is the most important function in your program - this is where you
will actually issue the commands to draw any geometry you have set up to
draw
********/
void render_rect(mat4 P, mat4 V, GLuint texture, mat4 Mrect, vec4 animation = vec4(1, 1, 0, 0))
{
	//head-----------------------------------------------------------------------------------------------
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	progbut->bind();
	glUniform1f(progbut->getUniform("totaltime"), phaseprogresstotaltime);
	glUniformMatrix4fv(progbut->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(progbut->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glActiveTexture(GL_TEXTURE0);		
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO_rect);
	mat4 M = glm::translate(glm::mat4(1.0f), modelpos) * glm::scale(glm::mat4(1.0f), modelscale);

	mat4 Sc = scale(mat4(1), vec3(0.2, 0.2, 0.2));
	//	mat4 Rz = rotate(mat4(1), butterfly[ii].rotz, vec3(0, 0, 1));
	vec4 texoff = animation;
	vec3 coloroff_red = vec3(1, 0, 0);
	vec3 coloroff_green = vec3(0, 1, 0);
	vec3 coloroff_blue = vec3(0, 0, 1);
	glUniform4fv(progbut->getUniform("texsplit"), 1, &texoff.x);
	glUniform3fv(progbut->getUniform("redmul"), 1, &coloroff_red.x);
	glUniform3fv(progbut->getUniform("bluemul"), 1, &coloroff_green.x);
	glUniform3fv(progbut->getUniform("greenmul"), 1, &coloroff_blue.x);
	mat4 Mr = M * Mrect * Sc;
	glUniformMatrix4fv(progbut->getUniform("M"), 1, GL_FALSE, &Mr[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	progbut->unbind();
}
//-------------------------------------------------------------------------------------------------
int render_body_to_FBO(double frametime, mat4 P, mat4 V)
{

	static float totaltime = 0.0;
	totaltime += frametime;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, fbbut);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOcolorbut, 0);
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, buffers);

	glDisable(GL_DEPTH_TEST);


	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);


	float aspect = width / (float)height;
	glViewport(0, 0, width, height);

	// Clear framebuffer.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create the matrix stacks - please leave these alone for now

	glm::mat4 M; //View, Model and Perspective matrix

	M = glm::mat4(1);


	glm::mat4 TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	M = TransZ * S;
	// Draw the box using GLSL.
						if (scenemode != SCENE_BUTTERFLY)
						{
							switch (scenemode)
							{
							default:
								break;
							case SCENE_SCELETONHEART:
							{

								float tileprogress = totaltime * 16.;
								int tx = (int)tileprogress % 4;
								int ty = (int)tileprogress / 4;
								vec4 texoff = vec4(4, 4, tx, ty);
								vec3 a = body.trackedbody.at(0).new_get_joint(FORECASTFACT, K4ABT_JOINT_SHOULDER_LEFT);
								vec3 b = body.trackedbody.at(0).new_get_joint(FORECASTFACT, K4ABT_JOINT_SPINE_CHEST);
								vec3 pos;
								pos.x = a.x * 0.2 + b.x * 0.8;
								pos.y = a.y * 0.7 + b.y * 0.3;
								pos.z = a.z;
								mat4 MrectHeart = translate(mat4(1), pos) * rotate(mat4(1), 3.14159265f, vec3(0, 1, 0)) * scale(mat4(1), vec3(0.4, 0.4, 0.4));
								render_rect(P, V, TexHeart, MrectHeart, texoff);



								mat4 MrectHead = translate(mat4(1), body.trackedbody.at(0).new_get_joint(FORECASTFACT, K4ABT_JOINT_NOSE) * 0.6f + body.trackedbody.at(0).new_get_joint(FORECASTFACT, K4ABT_JOINT_HEAD) * 0.6f) * scale(mat4(1), vec3(0.61, 0.61, 0.61));
								texoff = vec4(1, 1, 0, 0);
								render_rect(P, V, TextureSkeletonHead, MrectHead, texoff);
								redtone = vec3(1, 0, 0);
								greentone = vec3(0, 1, 0);
								bluetone = vec3(0, 0, 1);
							}
							break;
							}

							progbody->bind();
							M = glm::translate(glm::mat4(1.0f), modelpos) * glm::scale(glm::mat4(1.0f), modelscale);
							glUniform1f(progbody->getUniform("totaltime"), phaseprogresstotaltime);
							glUniform1f(progbody->getUniform("texblend"), 0);
							glUniformMatrix4fv(progbody->getUniform("P"), 1, GL_FALSE, &P[0][0]);
							glUniformMatrix4fv(progbody->getUniform("V"), 1, GL_FALSE, &V[0][0]);
							glUniformMatrix4fv(progbody->getUniform("M"), 1, GL_FALSE, &M[0][0]);
							//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

							glUniform3fv(progbody->getUniform("redmul"), 1, &redtone.x);
							glUniform3fv(progbody->getUniform("greenmul"), 1, &greentone.x);
							glUniform3fv(progbody->getUniform("bluemul"), 1, &bluetone.x);

							glBindVertexArray(VAObody);

							switch (scenemode)
							{
							default:
							case SCENE_LINES:
								glActiveTexture(GL_TEXTURE0);		glBindTexture(GL_TEXTURE_2D, TextureLines);
								break;
							case SCENE_SCELETON:
								glActiveTexture(GL_TEXTURE0);		glBindTexture(GL_TEXTURE_2D, TextureSkeleton);
								break;
							case SCENE_SCELETONHEART:
								glActiveTexture(GL_TEXTURE0);		glBindTexture(GL_TEXTURE_2D, TextureSkeletonH);
								break;
							case SCENE_FUR:
							{
								static GLuint tex1 = -1, tex2 = -1;
								static vec3 next_redtone = vec3(1, 0, 0);
								static vec3 next_greentone = vec3(0, 1, 0);
								static vec3 next_bluetone = vec3(0, 0, 1);
								vec3 actual_redtone = redtone;
								vec3 actual_greentone = greentone;
								vec3 actual_bluetone = bluetone;

								if (fur_phase_total_time < 0.001)
								{
									tex1 = rand() % FURMAXTEX;
									tex2 = rand() % FURMAXTEX;
									next_redtone = normalize(vec3(frand(), frand(), frand()));
									next_greentone = normalize(vec3(frand(), frand(), frand()));
									next_bluetone = normalize(vec3(frand(), frand(), frand()));
								}
								else if (fur_phase_total_time > FURCHANGETIME)
								{
									fur_phase_total_time = 0.001;
									tex1 = tex2;
									tex2 = rand() % FURMAXTEX;
									redtone = next_redtone;
									greentone = next_greentone;
									bluetone = next_bluetone;
									next_redtone = normalize(vec3(frand(), frand(), frand()));
									next_greentone = normalize(vec3(frand(), frand(), frand()));
									next_bluetone = normalize(vec3(frand(), frand(), frand()));
									actual_redtone = redtone;
									actual_greentone = greentone;
									actual_bluetone = bluetone;
								}
								else if (fur_phase_total_time > FURBLENDTIME)
								{
									float blend = fur_phase_total_time - FURBLENDTIME;
									blend /= FURCHANGETIME - FURBLENDTIME;
									if (blend > 1.0)	blend = 1.0;
									glUniform1f(progbody->getUniform("texblend"), blend);
									actual_redtone = mix(redtone, next_redtone, blend);
									actual_greentone = mix(greentone, next_greentone, blend);
									actual_bluetone = mix(bluetone, next_bluetone, blend);
								}

								fur_phase_total_time += frametime;
								glUniform3fv(progbody->getUniform("redmul"), 1, &actual_redtone.x);
								glUniform3fv(progbody->getUniform("greenmul"), 1, &actual_greentone.x);
								glUniform3fv(progbody->getUniform("bluemul"), 1, &actual_bluetone.x);
								/*	glUniform3fv(progbody->getUniform("redmul"), 1, &redtone.x);
									glUniform3fv(progbody->getUniform("bluemul"), 1, &greentone.x);
									glUniform3fv(progbody->getUniform("greenmul"), 1, &bluetone.x);*/
								glActiveTexture(GL_TEXTURE0);		glBindTexture(GL_TEXTURE_2D, TextureSkin[tex1]);
								glActiveTexture(GL_TEXTURE1);		glBindTexture(GL_TEXTURE_2D, TextureSkin[tex2]);
							}
							break;
							}
							//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							glDrawElements(GL_TRIANGLES, (int)body_size, GL_UNSIGNED_SHORT, (const void*)0);
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							progbody->unbind();

#ifndef RELEASEVERSION
		for (int ii = 0; ii < K4ABT_JOINT_COUNT; ii++)
		{
			mat4 Mrect = translate(mat4(1), body.trackedbody.at(0).new_get_joint(FORECASTFACT, ii)) * scale(mat4(1), vec3(0.05, 0.05, 0.05));
			vec4 texoff = vec4(1, 1, 0, 0);
			render_rect(P, V, TexRed, Mrect, texoff);
		}
#endif
						}
						else
						{
							float forecastfact = FORECASTFACT;
							//send the matrices to the shaders
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							progbut->bind();
							glUniform1f(progbut->getUniform("totaltime"), phaseprogresstotaltime);
							glUniformMatrix4fv(progbut->getUniform("P"), 1, GL_FALSE, &P[0][0]);
							glUniformMatrix4fv(progbut->getUniform("V"), 1, GL_FALSE, &V[0][0]);
							glUniformMatrix4fv(progbut->getUniform("M"), 1, GL_FALSE, &M[0][0]);
							//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, TextureButterfly);
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							//glBindVertexArray(VAOrect);
							glBindVertexArray(VAO_rect);
							M = glm::translate(glm::mat4(1.0f), modelpos) * glm::scale(glm::mat4(1.0f), modelscale);

							for (int ii = 0; ii < butterflyactual; ii++)
							{
								vec3 pos = mix(body.trackedbody.at(0).new_get_joint(forecastfact, butterfly[ii].iA), body.trackedbody.at(0).new_get_joint(forecastfact, butterfly[ii].iB), butterfly[ii].rationAB);
								mat4 Sc = scale(mat4(1), vec3(butterfly[ii].scale));
								mat4 Rz = rotate(mat4(1), butterfly[ii].rotz, vec3(0, 0, 1));
								vec3 t = body.trackedbody.at(0).new_get_joint(forecastfact, ii);
								if (butterfly[ii].startanim >= (-1))
									butterfly[ii].startanim -= frametime;
								vec4 texoff = vec4(4, 4, 0, 0);
								if (butterfly[ii].startanim <= 0)
								{
									float animprogress = -butterfly[ii].startanim * 2.;
									if (animprogress >= 1.)
									{
										animprogress = 1.;
										texoff = vec4(4, 4, 3, 3);
									}
									else
									{
										float tileprogress = animprogress * 16.;
										int tx = (int)tileprogress % 4;
										int ty = (int)tileprogress / 4;
										texoff = vec4(4, 4, tx, ty);
									}
								}
								glUniform4fv(progbut->getUniform("texsplit"), 1, &texoff.x);
								glUniform3fv(progbut->getUniform("redmul"), 1, &butterfly[ii].red.x);
								glUniform3fv(progbut->getUniform("bluemul"), 1, &butterfly[ii].blue.x);
								glUniform3fv(progbut->getUniform("greenmul"), 1, &butterfly[ii].green.x);

								mat4 Mr = M * translate(mat4(1), pos) * Rz * Sc;
								glUniformMatrix4fv(progbut->getUniform("M"), 1, GL_FALSE, &Mr[0][0]);
								glDrawArrays(GL_TRIANGLES, 0, 6);
							}
							progbut->unbind();
						}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, FBOcolorbut);
	glGenerateMipmap(GL_TEXTURE_2D);

	/*	static bool first = true;
		if (first)
			{
			glBindTexture(GL_TEXTURE_2D, FBOcolorbut);

			std::vector<unsigned char> buffer(width * height * 4);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
			char txt[1000];
			sprintf(txt, "outp.png");
			stbi_write_png(txt, width, height, 4, buffer.data(), 0);
			first = false;
			}*/

	return 0;

}
//*************************************************************************************************************
void render_to_screen(bool black) // aka render to framebuffer
{

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	int realwidth, realheight;
	glfwGetFramebufferSize(windowManager->getHandle(), &realwidth, &realheight);

	glViewport(0, 0, realwidth, realheight);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	postprog->bind();
	if (black)
	{
		glActiveTexture(GL_TEXTURE0);		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0);		glBindTexture(GL_TEXTURE_2D, FBOcolor);
		glActiveTexture(GL_TEXTURE1);		glBindTexture(GL_TEXTURE_2D, FBOmask);
	}
	glBindVertexArray(VAO_postprocrect);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	postprog->unbind();

}