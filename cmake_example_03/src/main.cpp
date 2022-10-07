#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const GLuint	WINDOW_WIDTH=960;
const GLuint	WINDOW_HEIGHT=520;

int	putError(std::string str)
{
	std::cerr << str << std::endl;
	return (-1);
};

void	OnFramebufferSizeChange(GLFWwindow* window, int width, int height)
{
	std::cout << "FrameBuffer size changed: (" << width << ", " << height << ")" << std::endl;
	glViewport(0, 0, width, height);
};

void	OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << "key: " << key << "\tscancode: " << scancode;
	std::cout << "\taction: " <<	(action == GLFW_PRESS ? "Pressed" : 
								(action == GLFW_RELEASE ? "RELEASE" :
								(action == GLFW_REPEAT ? "REPEAT" : "UNKNOWN")))
			<< "\tmods: " << (mods & GLFW_MOD_CONTROL ? "C" :
							(mods & GLFW_MOD_SHIFT ? "S" :
							(mods & GLFW_MOD_ALT ? "A" : "---"))) << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
};

int	main(void)
{
	std::cout << "Start Program" << std::endl;

	std::cout << "Initialize glfw" << std::endl;
	if (!glfwInit())
	{
		const char*	description = nullptr;
		glfwGetError(&description);
		return (putError("Failed to initialize glfw: " + std::string(description)));
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	std::cout << "Create glfw window" << std::endl;
	GLFWwindow*	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return (putError("Failed to create glfw window"));
	}
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		return (putError("Failed to initialize glad"));
	}
	auto	glVersion = glGetString(GL_VERSION);
	std::cout << "OpenGL context version: " << glVersion << std::endl;

	OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
	glfwSetKeyCallback(window, OnKeyEvent);

	std::cout << "Start main loop" << std::endl;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.0f, 0.1f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return (0);
};