///*
//CPE/CSC 471 Lab base code Wood/Dunn/Eckhardt
//*/
//
//#include "application.h"
//
//
//shared_ptr<Shape> shape;
//
//vec3 modelpos=vec3(0.0500000007,-2.49999881,0), modelscale = vec3(7.60000801,7.60000801,2.0);
//float camfov = 3.1415926 / 4.;
//
//float throat_scale = 1.0f;
//
//float torso_width_scale = 1.2;
//float torso_height_scale = 0.5;
//float arm_thickness_scale = 1.2;
//float leg_thickness_scale = 0.4;
//float foot_thickness_scale = 0.5;
//float head_thickness_scale = 2.0;
//
//float chinstart = 0.5;
//float sidechinstart = 1.35;
//
//
//
//
//
////******************************************************************************************
//enum progmode_ {MODE_STDBY,MODE_GO,MODE_UNKNOWN};
//int main(int argc, char **argv)
//{
//	progmode_ progmode = MODE_GO;
//	std::string resourceDir = "../resources"; // Where the resources are loaded from
//
//	ifstream testfile;
//	string path = resourceDir + "/frag.glsl";
//	testfile.open(path);
//	if (testfile.is_open() == false)
//		{
//		resourceDir = "../../resources";
//		path = resourceDir + "/frag.glsl";
//		testfile.open(path);
//		if (testfile.is_open() == false)
//			{
//			cout << "ERROR, resource folder cannot be found or there is no frag.glsl" << endl;
//			return 0;
//			}
//		}
//
//	if (argc >= 2)
//	{
//		resourceDir = argv[1];
//	}
//	srand(time(0));
//	Application *application = new Application();
//#ifdef NOKINECT
//	application->get_record(&application->body, &application->body.trackedbody);
//#endif // NOKINECT
//
//
//	/* your main will always include a similar set up to establish your window
//		and GL context, etc. */
//	WindowManager * windowManager = new WindowManager();
//	windowManager->init(1280,800);
//	windowManager->setEventCallbacks(application);
//	application->windowManager = windowManager;
//
//	/* This is the code that will likely change program to program as you
//		may need to initialize or set up different data and state */
//	// Initialize scene.
//	application->init(resourceDir);
//	application->initGeom();
//	application->init_atomic();
//
//	// Loop until the user closes the window.
//	mat4 V, P;
//	int width, height;
//	glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
//	P = glm::perspective(camfov, (float)((float)width / (float)height), 0.1f, 1000.0f); //so much type casting... GLM metods are quite funny ones
//
//	double totaltime = 0;
//	double cooldowntime = 0;
//	double time_since_last_body_tracked = 0;
//	double countfps = 0;
//	int frame = 0;
//	windowManager->SetFullScreen(fullscreen);
//
//	//app nicht 2 mal oder oefter ausfuehren
//	const char szUniqueNamedMutex[] = "eyeofmetamorphosis";
//	//HANDLE hHandle = CreateMutex(NULL, TRUE, szUniqueNamedMutex);
//	//if (ERROR_ALREADY_EXISTS == GetLastError())
//	//	{
//	//	cout << "program runs" << endl;
//	//	return(1); // Exit program
//	//	}
//
//	while(! glfwWindowShouldClose(windowManager->getHandle()))
//		{
//		windowManager->SetFullScreen(fullscreen);
//		double frametime = get_last_elapsed_time();
//		
//		countfps += frametime;
//		frame++;
//		if (frame >= 100)
//		{
////#ifndef RELEASEVERSION
////			cout << 1./(countfps / 100.) << endl;
////#endif
//			frame = 0;
//			countfps = 0;
//		}
//		
//		
//		if (application->runmode == RUN_NOFIRE)
//			totaltime = 0;
//		else
//			totaltime += frametime;
//
//
//		float cooldowntime = COOLDOWNTIME;
//		float firestarttime = FIRESTARTTIME;
//		if (application->runmode == RUN_DEBUGFIRE)
//			{
//			firestarttime = FIRESTARTTIMEDEBUG;
//			cooldowntime = COOLDOWNTIMEDEBUG;
//			}
//		
//		if (totaltime > firestarttime + cooldowntime)
//			{
//			application->roll_dice();
//			totaltime = 0;
//			}
//		else if (totaltime > firestarttime)
//			{
//			application->phaseprogresstotaltime += frametime;		
//			}
//
//
//		V = mycam.process(frametime);
//		// Render scene.
//		static int bodytracked = 0;
//		static bool hz30 = true;
//		if(hz30) 
//			bodytracked = application->Update_Kinect(frametime);
//		hz30 = !hz30;
//		if (bodytracked > 0)
//			{
//			//cout << application->body.trackedbody.joint_positions[K4ABT_JOINT_SPINE_CHEST].x << endl;
//			application->render_body_to_FBO(frametime, P, V);
//			application->render_render_fire_to_screen_FBO(frametime, P, V);
//			application->compute();
//			time_since_last_body_tracked = 0;
//			//if (firstTime == true) 
//			//	{
//			//		application->record();
//			//		firstTime = false;
//			//	}
//			}
//		else
//			time_since_last_body_tracked += frametime;
//		bool black = false;
//		if (time_since_last_body_tracked > 1.0)
//			{
//			application->roll_dice();
//			totaltime = 0;
//			black = true;
//			}
//		application->render_to_screen(black);
//		application->get_SSBO_back();
//		application->read_atomic();
//		//system("pause");
//		// Swap front and back buffers.
//		glfwSwapBuffers(windowManager->getHandle());
//		// Poll for and process events.
//		glfwPollEvents();
//		}
//
//	// Quit program.
//	windowManager->shutdown();
//#ifndef NOKINECT
//	application->Close_Kinect();
//#endif
//
//	return 0;
//}
