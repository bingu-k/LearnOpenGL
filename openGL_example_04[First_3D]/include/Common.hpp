#ifndef COMMON_HPP
#define COMMON_HPP

#include <memory>
#include <string>
#include <optional>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <vector>

#define CLASS_PTR(Shader)
class Shader;
using ShaderUPtr = std::unique_ptr<Shader>;
using ShaderSPtr = std::shared_ptr<Shader>;
using ShaderWPtr = std::weak_ptr<Shader>;

#define CLASS_PTR(Program)
class Program;
using ProgramUPtr = std::unique_ptr<Program>;
using ProgramSPtr = std::shared_ptr<Program>;
using ProgramWPtr = std::weak_ptr<Program>;

#define CLASS_PTR(Buffer)
class Buffer;
using BufferUPtr = std::unique_ptr<Buffer>;
using BufferSPtr = std::shared_ptr<Buffer>;
using BufferWPtr = std::weak_ptr<Buffer>;

#define CLASS_PTR(VertexLayout)
class VertexLayout;
using VertexLayoutUPtr = std::unique_ptr<VertexLayout>;
using VertexLayoutSPtr = std::shared_ptr<VertexLayout>;
using VertexLayoutWPtr = std::weak_ptr<VertexLayout>;

#define CLASS_PTR(Context)
class Context;
using ContextUPtr = std::unique_ptr<Context>;
using ContextSPtr = std::shared_ptr<Context>;
using ContextWPtr = std::weak_ptr<Context>;

#define CLASS_PTR(Image)
class Image;
using ImageUPtr = std::unique_ptr<Image>;
using ImageSPtr = std::shared_ptr<Image>;
using ImageWPtr = std::weak_ptr<Image>;

#define CLASS_PTR(Texture)
class Texture;
using TextureUPtr = std::unique_ptr<Texture>;
using TextureSPtr = std::shared_ptr<Texture>;
using TextureWPtr = std::weak_ptr<Texture>;

const GLuint	WINDOW_WIDTH=800;
const GLuint	WINDOW_HEIGHT=600;

int	putError(std::string str)
{
	std::cerr << str << std::endl;
	return (-1);
};

std::optional<std::string>  LoadTextFile(const std::string& filename)
{
    std::ifstream   fin(filename);
    if (!fin.is_open())
    {
        putError("Failed to open file: " + filename);
        return {};
    }
    std::stringstream   text;
    text << fin.rdbuf();
    return (text.str());
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

#endif