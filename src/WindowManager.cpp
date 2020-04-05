
#include "WindowManager.h"
#include "GLSL.h"

#include <iostream>


void error_callback(int error, const char *description)
{
	std::cerr << description << std::endl;
}

WindowManager * WindowManager::instance = nullptr;

WindowManager::WindowManager()
{
	if (instance)
	{
		std::cerr << "One instance of WindowManager has already been created, event callbacks of new instance will not work." << std::endl;
	}

	instance = this;
}

WindowManager::~WindowManager()
{
	if (instance == this)
	{
		instance = nullptr;
	}
}
bool WindowManager::IsFullscreen()
	{
	return glfwGetWindowMonitor(windowHandle) != nullptr;
	}
void WindowManager::SetFullScreen(bool fullscreen)
	{
	if (fullscreen_mode == fullscreen)
		return;

	if (fullscreen)
		{
		// backup windwo position and window size
		glfwGetWindowPos(windowHandle, &_wndPos[0], &_wndPos[1]);
		glfwGetWindowSize(windowHandle, &_wndSize[0], &_wndSize[1]);

		// get reolution of monitor
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		// swithc to full screen
		glfwSetWindowMonitor(windowHandle, _monitor, 0, 0, mode->width, mode->height, 0);
		fullscreen_mode = true;
		}
	else
		{
		// restore last window size and position
		glfwSetWindowMonitor(windowHandle, nullptr, _wndPos[0], _wndPos[1], _wndSize[0], _wndSize[1], 0);
		fullscreen_mode = false;
		}


	}
bool WindowManager::init(int const width, int const height)
{
	glfwSetErrorCallback(error_callback);

	// Initialize glfw library
	if (!glfwInit())
	{
		return false;
	}

	//request the highest possible version of OGL - important for mac
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Create a windowed mode window and its OpenGL context.
	windowHandle = glfwCreateWindow(width, height, "eye of metamorphosis", nullptr, nullptr);
	if (! windowHandle)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(windowHandle);

	// Initialize GLAD
	if (!gladLoadGL())
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Set vsync
	glfwSwapInterval(1);

	glfwSetKeyCallback(windowHandle, key_callback);
	glfwSetMouseButtonCallback(windowHandle, mouse_callback);
	glfwSetFramebufferSizeCallback(windowHandle, resize_callback);

	return true;
}

void WindowManager::shutdown()
{
	glfwHideWindow(windowHandle);
	glfwDestroyWindow(windowHandle);
	glfwTerminate();
}

void WindowManager::setEventCallbacks(EventCallbacks * callbacks_in)
{
	callbacks = callbacks_in;
}

GLFWwindow * WindowManager::getHandle()
{
	return windowHandle;
}

void WindowManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (instance && instance->callbacks)
	{
		instance->callbacks->keyCallback(window, key, scancode, action, mods);
	}
}

void WindowManager::mouse_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (instance && instance->callbacks)
	{
		instance->callbacks->mouseCallback(window, button, action, mods);
	}
}

void WindowManager::resize_callback(GLFWwindow * window, int in_width, int in_height)
{
	if (instance && instance->callbacks)
	{
		instance->callbacks->resizeCallback(window, in_width, in_height);
	}
}
