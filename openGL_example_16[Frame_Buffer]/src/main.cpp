#include "../include/Context.hpp"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void	OnFramebufferSizeChange(GLFWwindow* window, int width, int height)
{
	std::cout << "FrameBuffer size changed: (" << width << ", " << height << ")" << std::endl;
	auto	context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
	context->Reshape(width, height);
};

void	OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
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

void OnCharEvent(GLFWwindow* window, unsigned int ch)
{
    ImGui_ImplGlfw_CharCallback(window, ch);
}

void	OnCursorPos(GLFWwindow* window, double x, double y)
{
	auto	context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
	context->MouseMove(x, y);
};

void	OnMouseButton(GLFWwindow* window, int button, int action, int modifier)
{
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier);
	auto	context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
	double	x, y;
	glfwGetCursorPos(window, &x, &y);
	context->MouseButton(button, action, x, y);
}

void OnScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

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

	// IMGUI 초기화
    auto imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiContext);
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();

	// Context 생성[Program 생성{Shader 불러와서 붙이기}]
	ContextUPtr	context = Context::Create();
	if (!context)
	{
		glfwTerminate();
		return (putError("Failed to create Context"));	
	}
	glfwSetWindowUserPointer(window, context.get());

	// Custom Callback Function 설정
	OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
	glfwSetKeyCallback(window, OnKeyEvent);
    glfwSetCharCallback(window, OnCharEvent);
	glfwSetCursorPosCallback(window, OnCursorPos);
	glfwSetMouseButtonCallback(window, OnMouseButton);
    glfwSetScrollCallback(window, OnScroll);

	// ImGui Callback Function install[custom에서 조절했기 때문에 필요하지 않다.]
	// ImGui_ImplGlfw_InstallCallbacks(window);

	// Rendering 시작
	std::cout << "Start main loop" << std::endl;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		context->ProcessInput(window);
		context->Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
	// 안전하게 메모리 해제
	context.reset();

    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imguiContext);

	glfwTerminate();
	return (0);
};