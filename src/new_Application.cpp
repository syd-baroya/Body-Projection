#include "new_Application.h"
#include <glad/glad.h>
#include <iostream>
#include "ShaderLibrary.hpp"


void Application::init(int argc, char** argv)
{
	config.startup_state = StartupState(argc, argv);
	initGLFW();
	initGL();
	initShaders();
	initScene();
}

void Application::run()
{
}

void Application::cleanup()
{
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
	ShaderLibrary::getInstance().loadFromShaderCollection("/shaders.txt");
}

void Application::initScene()
{
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
	if (*carg == "-dev" || *carg == "--developer") {
		this->developer_mode = true;
	}
	return std::next(carg);
}

std::vector<std::string>::iterator StartupState::parseInput(std::vector<std::string>::iterator carg, const std::vector<std::string>& args)
{
	input_paths.push_back(*carg);
	return std::next(carg);
}


static void error_callback(int error, const char* description) { std::cerr << "glfw error: " << description << std::endl; };
static void key_callback(GLFWwindow* _window, int key, int scancode, int action, int mods) { };

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) { };
static void resize_callback(GLFWwindow* window, int in_width, int in_height) { };