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

#define CLASS_PTR(FrameBuffer)
class FrameBuffer;
using FrameBufferUPtr = std::unique_ptr<FrameBuffer>;
using FrameBufferSPtr = std::shared_ptr<FrameBuffer>;
using FrameBufferWPtr = std::weak_ptr<FrameBuffer>;

#define CLASS_PTR(Material)
class Material;
using MaterialUPtr = std::unique_ptr<Material>;
using MaterialSPtr = std::shared_ptr<Material>;
using MaterialWPtr = std::weak_ptr<Material>;

#define CLASS_PTR(Mesh)
class Mesh;
using MeshUPtr = std::unique_ptr<Mesh>;
using MeshSPtr = std::shared_ptr<Mesh>;
using MeshWPtr = std::weak_ptr<Mesh>;

#define CLASS_PTR(Model)
class Model;
using ModelUPtr = std::unique_ptr<Model>;
using ModelSPtr = std::shared_ptr<Model>;
using ModelWPtr = std::weak_ptr<Model>;

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

glm::vec3   GetAttenuationCoeff(float distance) {
    const auto linear_coeff = glm::vec4(
        8.4523112e-05,
        4.4712582e+00,
        -1.8516388e+00,
        3.3955811e+01
    );
    const auto quad_coeff = glm::vec4(
        -7.6103583e-04,
        9.0120201e+00,
        -1.1618500e+01,
        1.0000464e+02
    );

    float kc = 1.0f;
    float d = 1.0f / distance;
    auto dvec = glm::vec4(1.0f, d, d*d, d*d*d);
    float kl = glm::dot(linear_coeff, dvec);
    float kq = glm::dot(quad_coeff, dvec);
    
    return glm::vec3(kc, glm::max(kl, 0.0f), glm::max(kq*kq, 0.0f));
}

#endif