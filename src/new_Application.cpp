#include "new_Application.h"


static void error_callback(int error, const char* description) { std::cerr << "glfw error: " << description << std::endl; };
static void key_callback(GLFWwindow* _window, int key, int scancode, int action, int mods) { };

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) { };
static void resize_callback(GLFWwindow* window, int in_width, int in_height) { };

void Application::init(int argc, char** argv)
{
	config.startup_state = StartupState(argc, argv);
	initGLFW();
	initGL();
	initShaders();
	initProgs();
	initGeom();
	initScene();
}

void Application::run()
{
	RenderSystem& render_system = RenderSystem::getInstance();

	KinectSystem& kinect_system = KinectSystem::getInstance();

	Time globalTime;


	kinect_system.InitializeDefaultSensor();
	render_system.init(_window);

	//std::vector<Entity*> new_entities(entities);

	//new_entities.push_back(&camera);

	int active_scene = 0;
	int active_anim = 0;
	double frametime = 0;

	// Loop until the user closes the window.

	while (!glfwWindowShouldClose(_window))

	{
		frametime = globalTime.getElapsedTime();
		//active_scene = rand() % scene_comps.size();
		//active_anim = rand() % anim_comps.size();

		TrackedBodyEntity* tb = body_entities.at(0);

		kinect_system.process(frametime, tb);

		render_system.process(&scene_comps.at(active_scene), &anim_comps.at(active_anim), body_entities, fbo_entities, frame_buffers, getCurrScreenSize(), frametime, false);



		// Swap front and back buffers.

		glfwSwapBuffers(_window);

		// Poll for and process events.

		glfwPollEvents();

	}
}

void Application::cleanup()
{
	KinectSystem& kinect_system = KinectSystem::getInstance();
	kinect_system.CloseSensor();
	glfwHideWindow(_window);
	glfwDestroyWindow(_window);
	glfwTerminate();
}

bool Application::shouldExit() const
{
	return false;
}

void Application::initGL()
{
	// Initialize GLAD
	if (!gladLoadGL())
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		exit(1);
	}
	glGetError();
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void Application::initGLFW()
{
	glfwSetErrorCallback(error_callback);

	// Initialize glfw library
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed!" << std::endl;
		exit(0);
	}

	//request the highest possible version of OGL - important for mac
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Create a windowed mode window and its OpenGL context.
	_window = glfwCreateWindow(config.w_width, config.w_height, "eye of metamorphosis", nullptr, nullptr);
	if (!_window)
	{
		glfwTerminate();
		exit(0);
	}

	glfwMakeContextCurrent(_window);

	// Set vsync
	glfwSwapInterval(1);

	glfwSetKeyCallback(_window, key_callback);
	glfwSetMouseButtonCallback(_window, mouse_callback);
	glfwSetFramebufferSizeCallback(_window, resize_callback);

}

void Application::initShaders()
{
	ShaderLibrary::getInstance().loadFromShaderCollection("../resources/shaders.txt");
}

void Application::initScene()
{
	/*
	* add fire animation
	*/
	SimpleTexture2D fire_tex; 
	fire_tex.setFile("../resources/firering_a.jpg");

	TextureArray fire_tex_array;
	fire_tex_array.setFile("../resources/firering_%.3d.png");
	
	AnimationComponent fire_anim(fire_tex, fire_tex_array);
	fire_anim.init();

	//SCENE_LINES
	SimpleTexture2D line_tex;
	line_tex.setFile("../resources/lines.jpg");

	SceneComponent scene_lines(line_tex);

	//SCENE_SCELETON
	SimpleTexture2D skeleton_tex;
	skeleton_tex.setFile("../resources/skeleton.jpg");
	
	SceneComponent scene_skeleton(skeleton_tex);

	
	//SCENE_FUR
	SimpleTexture2D fur_tex0;
	fur_tex0.setFile("../resources/fur.jpg");
	SimpleTexture2D fur_tex1;
	fur_tex1.setFile("../resources/snake.jpg");
	SimpleTexture2D fur_tex2;
	fur_tex2.setFile("../resources/zebra.jpg");
	SimpleTexture2D fur_tex3;
	fur_tex3.setFile("../resources/chameleon.jpg");
	SimpleTexture2D fur_tex4;
	fur_tex4.setFile("../resources/chameleon2.jpg");
	SimpleTexture2D fur_tex5;
	fur_tex5.setFile("../resources/chameleon3.jpg");
	SimpleTexture2D fur_tex6;
	fur_tex6.setFile("../resources/gecko.jpg");

	SimpleTexture2D fur_tex[] = {
		fur_tex0,
		fur_tex1,
		fur_tex2,
		fur_tex3,
		fur_tex4,
		fur_tex5,
		fur_tex6
	};
	FurScene scene_fur(fur_tex);



	
	anim_comps.push_back(fire_anim);
	scene_comps.push_back(scene_lines);
	scene_comps.push_back(scene_skeleton);
	scene_comps.push_back(scene_fur);

	
}

void Application::initGeom()
{
	/***BODY WITHOUT CLAVICLES***/
//torso w/o clavicles
	TexturedGeomComponent tex_geom_comp(GL_DYNAMIC_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW);

	std::vector<GLushort>& indices = tex_geom_comp.getMutableElements();

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
	indices.push_back(38);	indices.push_back(12);	indices.push_back(13);
	indices.push_back(38);	indices.push_back(13);	indices.push_back(39);
	indices.push_back(40);	indices.push_back(16);	indices.push_back(41);
	indices.push_back(16);	indices.push_back(17);	indices.push_back(41);



	vector<vec2>& tex = tex_geom_comp.getMutableTextures();
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

	ivec2 screensize = getCurrScreenSize();
	TrackedBodyEntity* tracked_body = new TrackedBodyEntity();
	DrawableComponent* dc_tb = tracked_body->addComponent<DrawableComponent>(screensize.x, screensize.y);
	TexturedGeomComponent* gc_tb = tracked_body->addComponent<TexturedGeomComponent>(tex_geom_comp);
	tracked_body->generateBodyVertices();
	dc_tb->init();
	gc_tb->init();

	TexturedMeshEntity* rect = new TexturedMeshEntity();
	TexturedMeshEntity* post_proc_rect = new TexturedMeshEntity();

	rect->setProgName("screenproc");
	post_proc_rect->setProgName("postprog");

	//TexturedGeomComponent tex_geom_comp2(GL_STATIC_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW);
	//vector<vec3>& rect_pos = tex_geom_comp2.getMutableVertices();
	//std::vector<GLushort>& rect_elems = tex_geom_comp2.getMutableElements();
	//vector<vec2>& rect_tex = tex_geom_comp2.getMutableTextures();

	//rect_pos.push_back(vec3(-1, -1, 0));
	//rect_pos.push_back(vec3(1, -1, 0));
	//rect_pos.push_back(vec3(-1, 1, 0));
	//rect_pos.push_back(vec3(1, 1, 0));
	//rect_tex.push_back(vec2(0, 0));
	//rect_tex.push_back(vec2(1, 0));
	//rect_tex.push_back(vec2(0, 1));
	//rect_tex.push_back(vec2(1, 1));
	//rect_elems.push_back(0);
	//rect_elems.push_back(1);
	//rect_elems.push_back(2);
	//rect_elems.push_back(1);
	//rect_elems.push_back(3);
	//rect_elems.push_back(2);

	TexturedGeomComponent* gc_rect = rect->addComponent<TexturedGeomComponent>(GL_STATIC_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW);
	vector<vec3>& rect_pos = gc_rect->getMutableVertices();
	std::vector<GLushort>& rect_elems = gc_rect->getMutableElements();
	vector<vec2>& rect_tex = gc_rect->getMutableTextures();
	rect_pos.push_back(vec3(-1, -1, 0));
	rect_pos.push_back(vec3(1, -1, 0));
	rect_pos.push_back(vec3(-1, 1, 0));
	rect_pos.push_back(vec3(1, 1, 0));
	rect_tex.push_back(vec2(0, 0));
	rect_tex.push_back(vec2(1, 0));
	rect_tex.push_back(vec2(0, 1));
	rect_tex.push_back(vec2(1, 1));
	rect_elems.push_back(0);
	rect_elems.push_back(1);
	rect_elems.push_back(2);
	rect_elems.push_back(1);
	rect_elems.push_back(3);
	rect_elems.push_back(2);
	TexturedGeomComponent* gc_rect2 = post_proc_rect->addComponent<TexturedGeomComponent>(GL_STATIC_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW);
	rect_pos = gc_rect2->getMutableVertices();
	rect_elems = gc_rect2->getMutableElements();
	rect_tex = gc_rect2->getMutableTextures();
	rect_pos.push_back(vec3(-1, -1, 0));
	rect_pos.push_back(vec3(1, -1, 0));
	rect_pos.push_back(vec3(-1, 1, 0));
	rect_pos.push_back(vec3(1, 1, 0));
	rect_tex.push_back(vec2(0, 0));
	rect_tex.push_back(vec2(1, 0));
	rect_tex.push_back(vec2(0, 1));
	rect_tex.push_back(vec2(1, 1));
	rect_elems.push_back(0);
	rect_elems.push_back(1);
	rect_elems.push_back(2);
	rect_elems.push_back(1);
	rect_elems.push_back(3);
	rect_elems.push_back(2);
	gc_rect->init();
	gc_rect2->init();

	generateFramebuffers();

	//ssbo.create_SSBO(ssbo_CPUMEM);

	body_entities.push_back(tracked_body);
	fbo_entities.emplace("rect", rect);
	fbo_entities.emplace("post_proc_rect", post_proc_rect);

}

void Application::initProgs() {
	Program* progfire = ShaderLibrary::getInstance().getPtr("progfire");
	progfire->addUniform("P");
	progfire->addUniform("V");
	progfire->addUniform("M");
	progfire->addUniform("firescale");
	progfire->addUniform("texsplit");
	progfire->addUniform("totaltime");
	progfire->addAttribute("vertPos");
	progfire->addAttribute("vertTex");

	//Program* progbut = ShaderLibrary::getInstance().getPtr("progbut");
	//progbut->addUniform("P");
	//progbut->addUniform("V");
	//progbut->addUniform("M");
	//progbut->addUniform("texsplit");
	//progbut->addUniform("totaltime");
	//progbut->addUniform("redmul");
	//progbut->addUniform("greenmul");
	//progbut->addUniform("bluemul");
	//progbut->addAttribute("vertPos");
	//progbut->addAttribute("vertTex");

	Program* progbody = ShaderLibrary::getInstance().getPtr("progbody");
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

	Program* postprog = ShaderLibrary::getInstance().getPtr("postprog");
	postprog->addAttribute("vertPos");
	postprog->addAttribute("vertTex");

	Program* screenproc = ShaderLibrary::getInstance().getPtr("screenproc");
	screenproc->addAttribute("vertPos");
	screenproc->addAttribute("vertTex");

	//[TWOTEXTURES]
	GLuint TexLoc;
	//set the 2 textures to the correct samplers in the fragment shader:
	glUseProgram(progfire->getPID());
	TexLoc = glGetUniformLocation(progfire->getPID(), "tex");	glUniform1i(TexLoc, 0);
	TexLoc = glGetUniformLocation(progfire->getPID(), "tex2");	glUniform1i(TexLoc, 1);
	TexLoc = glGetUniformLocation(progfire->getPID(), "texA");	glUniform1i(TexLoc, 2);
	TexLoc = glGetUniformLocation(progfire->getPID(), "texarr");	glUniform1i(TexLoc, 3);

	glUseProgram(postprog->getPID());
	TexLoc = glGetUniformLocation(postprog->getPID(), "tex");	glUniform1i(TexLoc, 0);
	TexLoc = glGetUniformLocation(postprog->getPID(), "texmask");	glUniform1i(TexLoc, 1);

	glUseProgram(progbody->getPID());
	TexLoc = glGetUniformLocation(progbody->getPID(), "tex");	glUniform1i(TexLoc, 0);
	TexLoc = glGetUniformLocation(progbody->getPID(), "tex2");	glUniform1i(TexLoc, 1);
}

void Application::generateFramebuffers()
{
	ivec2 screen_size = getCurrScreenSize();
	Framebuffer* fb = new Framebuffer();
	fb->bind();
	FramebufferObject FBOcolor(screen_size.x, screen_size.y);
	FramebufferObject FBOmask(screen_size.x, screen_size.y);
	Renderbuffer depth_rb;
	FBOcolor.initParams();
	FBOmask.initParams();
	depth_rb.initParams();
	fb->attach(FBOcolor, "FBOcolor", GL_COLOR_ATTACHMENT0, 0);
	fb->attach(FBOmask, "FBOmask", GL_COLOR_ATTACHMENT1, 0);
	fb->attach(depth_rb, GL_DEPTH_ATTACHMENT);
	fb->unbind();

	Framebuffer* fbbut = new Framebuffer();
	fbbut->bind();
	FramebufferObject FBOcolorbut(screen_size.x, screen_size.y);
	Renderbuffer depth_rbbut;
	FBOcolorbut.initParams();
	depth_rbbut.initParams();
	fbbut->attach(FBOcolorbut, "FBOcolorbut", GL_COLOR_ATTACHMENT0, 0);
	fbbut->attach(depth_rbbut, GL_DEPTH_ATTACHMENT);
	fbbut->unbind();

	frame_buffers.emplace("fb", fb);
	frame_buffers.emplace("fbbut", fbbut);

}

bool Application::isFullscreen()
{
	return glfwGetWindowMonitor(_window) != nullptr;
}
void Application::setFullScreen(bool fullscreen)
{
	if (fullscreen_mode == fullscreen)
		return;

	if (fullscreen)
	{
		// backup windwo position and window size
		glfwGetWindowPos(_window, &_wndPos[0], &_wndPos[1]);
		glfwGetWindowSize(_window, &_wndSize[0], &_wndSize[1]);

		// get reolution of monitor
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		// swithc to full screen
		glfwSetWindowMonitor(_window, _monitor, 0, 0, mode->width, mode->height, 0);
		fullscreen_mode = true;
	}
	else
	{
		// restore last window size and position
		glfwSetWindowMonitor(_window, nullptr, _wndPos[0], _wndPos[1], _wndSize[0], _wndSize[1], 0);
		fullscreen_mode = false;
	}


}

glm::ivec2 Application::getCurrScreenSize()
{
	int width, height;
	glfwGetFramebufferSize(_window, &width, &height);

	return glm::ivec2(width, height);
}

StartupState::StartupState(int argc, char** argv)
{
	if (argc > 1) {
		parseArgs(argc, argv);
	}
}

void StartupState::parseArgs(int argc, char** argv)
{
	std::vector<std::string> args((&argv[1]), &(argv[argc]));
	std::vector<std::string>::iterator carg = args.begin();
	while (carg != args.end()) {
		if (carg->at(0) == '-') {
			carg = parseOption(carg, args);
		}
		else {
			carg = parseInput(carg, args);
		}
	}
}

std::vector<std::string>::iterator StartupState::parseOption(std::vector<std::string>::iterator carg, const std::vector<std::string>& args)
{
	if (*carg == "-dev" || *carg == "--developer") 
		this->developer_mode = true;
	return std::next(carg);
}

std::vector<std::string>::iterator StartupState::parseInput(std::vector<std::string>::iterator carg, const std::vector<std::string>& args)
{
	input_paths.push_back(*carg);
	return std::next(carg);
}


