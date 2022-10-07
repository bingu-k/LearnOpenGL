#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "Common.hpp"
#include "Shader.hpp"
#include "Program.hpp"

CLASS_PTR(Context)
class Context
{
public:
    static ContextUPtr  Create(void);
    void    Render(void);

private:
    ProgramUPtr m_program;

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

    glUseProgram(m_program->Get());
    glDrawArrays(GL_POINT, 0, 1);
};

bool    Context::init(void)
{
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
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    uint32_t    VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    return (true);
};

#endif