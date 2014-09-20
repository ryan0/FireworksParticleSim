#include "setup.h"


void setupWindow(GLFWwindow*& inWindow, int width, int height, const char* name)
{
	GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();

	inWindow = window;
}


void setupFullscreen(GLFWwindow*& inWindow)
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	GLFWwindow* window = glfwCreateWindow(videoMode->width, videoMode->height, "", glfwGetPrimaryMonitor(), NULL);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glewInit();
	
	inWindow = window;
}