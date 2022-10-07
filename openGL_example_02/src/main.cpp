#include "../include/Context.hpp"

int	main(void)
{
	std::cout << "Start Program" << std::endl;

	// GLFW 초기 설정
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

	// GLFW를 이용한 Window생성
	std::cout << "Create glfw window" << std::endl;
	GLFWwindow*	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return (putError("Failed to create glfw window"));
	}
	glfwMakeContextCurrent(window);
	
	// GLAD 초기 설정
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		return (putError("Failed to initialize glad"));
	}
	auto	glVersion = glGetString(GL_VERSION);
	std::cout << "OpenGL context version: " << glVersion << std::endl;

	// Context 생성[Program 생성{Shader 불러와서 붙이기}]
	ContextUPtr	context = Context::Create();
	if (!context)
	{
		glfwTerminate();
		return (putError("Failed to create Context"));	
	}

	// Callback Function 설정
	OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
	glfwSetKeyCallback(window, OnKeyEvent);

	// Rendering 시작
	std::cout << "Start main loop" << std::endl;
	while (!glfwWindowShouldClose(window))
	{
		context->Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// 안전하게 메모리 해제
	context.reset();
	
	glfwTerminate();
	return (0);
};