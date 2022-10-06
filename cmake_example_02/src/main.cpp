#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

const GLuint	WINDOW_WIDTH=960;
const GLuint	WINDOW_HEIGHT=520;

int	putError(std::string str)
{
	std::cerr << str << std::endl;
	return (-1);
};

void	putString(std::string str)
{ std::cout << str << std::endl; };

int	main(void)
{
	putString("Start Program");

	putString("Initialize glfw");
	if (!glfwInit())
	{
		const char*	description = nullptr;
		glfwGetError(&description);
		return (putError("Failed to initialize glfw: " + std::string(description)));
	}

	putString("Create glfw window");
	GLFWwindow*	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return (putError("Failed to create glfw window"));
	}
	
	putString("Start main loop");
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return (0);
};