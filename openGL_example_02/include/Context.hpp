#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "Common.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "Buffer.hpp"
#include "VertexLayout.hpp"

CLASS_PTR(Context)
class Context
{
public:
    static ContextUPtr  Create(void);
    void    Render(void);

private:
    ProgramUPtr         m_program;
    VertexLayoutUPtr    m_VAO;
    BufferUPtr          m_VBO, m_EBO;

    Context(void) {};
    bool    init(void);
};

ContextUPtr  Context::Create(void)
{
    ContextUPtr context = ContextUPtr(new Context());
    if (!context->init())
        return (nullptr);
    return (std::move(context));
};

void    Context::Render(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
};

bool    Context::init(void)
{
    // 기본 Vertex들 생성
    float vertices[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };
    uint32_t indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    // Buffer 설정
        // Vertex Array
    m_VAO = VertexLayout::Create();
        // Vertex Buffer
    m_VBO = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW,
                                    vertices, sizeof(float) * 12);
        // Vertex Element Buffer
    m_EBO = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices,
                                    sizeof(uint32_t) * 6);
        // Vertex Attribute layout
    m_VAO->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    // Shader 생성
	std::cout << std::filesystem::current_path().string() << std::endl;
	ShaderSPtr	vertexShader = Shader::CreateFromFile(".\\shader\\shader.vs", GL_VERTEX_SHADER);
	ShaderSPtr	fragmentShader = Shader::CreateFromFile(".\\shader\\shader.fs", GL_FRAGMENT_SHADER);
	if (!vertexShader || !fragmentShader)
        return (false);
    std::cout << "Vertex Shader id: " << vertexShader->Get() << std::endl;
	std::cout << "Fragment Shader id: " << fragmentShader->Get() << std::endl;

	// Program 생성
	this->m_program = Program::Create({fragmentShader, vertexShader});
    if (!this->m_program)
        return (false);
	std::cout << "Program Shader id: " << this->m_program->Get() << std::endl;

    // 배경 Clear 색상 지정
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    return (true);
};

#endif