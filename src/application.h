//#pragma once
//
//#include <glad/glad.h>
//#include <time.h>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"
//
//#include "GLSL.h"
//#include "Program.h"
//#include "MatrixStack.h"
//
//#include "WindowManager.h"
//#include "Shape.h"
//#include "new_body.h"
//#include <fstream>
//#include <math.h>
//#include "camera.h"
//
////#define RELEASEVERSION
//#define NOKINECT
//#define PI 3.14159265
//bool fullscreen = true;
//bool firstTime = true;
//
//using namespace std;
//using namespace glm;
//
//enum scenemode_{ SCENE_LINES, SCENE_SCELETON, SCENE_SCELETONHEART, SCENE_BUTTERFLY,SCENE_FUR,SCENE_IDLE };
//enum runmode_ { RUN_NOFIRE, RUN_DEBUGFIRE, RUNFIRE };
//#define FIRESTARTTIME 20.0
//#define FIRESTARTTIMEDEBUG 2.0
//#define COOLDOWNTIME 50.0
//#define COOLDOWNTIMEDEBUG 8.0
//
//
//
//
//camera mycam;
//
//enum correctmode_ { CORR_SHIFT, CORR_SCALE, CORR_SCEWSCALEUP, CORR_SCEWSCALEDOWN, SCALE_TORSO_WIDTH, SCALE_TORSO_HEIGHT, SCALE_ARM, SCALE_LEG, SCALE_FOOT, SCALE_HEAD, SHIFT_CHIN, SHIFT_SIDE_CHIN };
//
//
//#define FURMAXTEX 7
//#ifdef RELEASEVERSION
//#define FURCHANGETIME 11.0
//#define FURBLENDTIME 10.0
//#else
//#define FURCHANGETIME 2.0//11.0
//#define FURBLENDTIME 1.0//10.0
//
//#endif
//#define BUTTERFLYCOUNT 50
//class butterfly_
//{
//public:
//	float rotz;
//	float scale;
//	int iA, iB;
//	float rationAB;
//	float startanim;
//	vec3 red, green, blue;
//};
//
//
//#define ssbo_size 1024
//
//class ssbo_data
//{
//public:
//	ivec4 positions_list[ssbo_size];
//};
//
//class Application : public EventCallbacks
//{
//
//public:
//	scenemode_ scenemode = SCENE_LINES;
//	runmode_ runmode = RUN_NOFIRE;
//	correctmode_ correctmode = CORR_SHIFT;
//	new_body_ body;
//	GLuint VAO_postprocrect, VAO_rect, VBrect;
//	vector<vec3> rectpos;
//	WindowManager* windowManager = nullptr;
//	GLuint fb, FBOcolor, FBOmask, depth_rb, fbbut, FBOcolorbut, depth_rbbut;
//	//random color scheme:
//	vec3 redtone, greentone, bluetone;
//
//	float fur_phase_total_time = 0;
//
//	float  phaseprogresstotaltime = 0;
//
//	butterfly_ butterfly[BUTTERFLYCOUNT];
//	int butterflyactual = 0;
//
//	bool animation = false;
//	// Our shader program
//	std::shared_ptr<Program> prog, postprog, progbut, screenproc, progbody, computeprog;
//
//	// Contains vertex information for OpenGL
//	GLuint VertexArrayID;
//
//	// Data necessary to give our box to OpenGL
//	GLuint VertexBufferID, VertexColorIDBox, IndexBufferIDBox;
//
//	//texture data
//	GLuint Texture, TextureSkeleton, TexRed, TextureSkeletonHead, TextureSkeletonH, TexHeart, TextureLines, TextureSkin[FURMAXTEX];
//	GLuint TextureButterfly, TextureArray, TextureAlpha;
//
//	GLuint VAO, vertexcount, VAOrect, VAObody, VBbody, body_size;
//
//	ssbo_data ssbo_CPUMEM;
//	GLuint ssbo_GPU_id;
//	GLuint computeProgram;
//	GLuint atomicsBuffer;
//
//	vector<vec3> app_posb;
//
//};