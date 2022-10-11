#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "Common.hpp"
#include "Shader.hpp"

CLASS_PTR(Program)
class Program
{
public:
    static ProgramUPtr  Create(const std::vector<ShaderSPtr>& shaders);
    static ProgramUPtr  Create(const std::string& vertexShaderFilename
                            , const std::string& fragmentShaderFilename);

    ~Program(void);
    uint32_t    Get(void) const
    { return (this->m_program); };
    void        Use(void) const
    { glUseProgram(m_program); };
    void        SetUniform(const std::string& name, int value) const
    { glUniform1i(glGetUniformLocation(m_program, name.c_str()), value); };
    void        SetUniform(const std::string& name, float value) const
    { glUniform1f(glGetUniformLocation(m_program, name.c_str()), value); };
    void        SetUniform(const std::string& name, const glm::mat4& value) const
    { glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); };
    void        SetUniform(const std::string& name, const glm::vec2& value) const
    { glUniform2fv(glGetUniformLocation(m_program, name.c_str()), 1, glm::value_ptr(value)); };
    void        SetUniform(const std::string& name, const glm::vec3& value) const
    { glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, glm::value_ptr(value)); };
    void        SetUniform(const std::string& name, const glm::vec4& value) const
    { glUniform4fv(glGetUniformLocation(m_program, name.c_str()), 1, glm::value_ptr(value)); };
private:
    uint32_t    m_program { 0 };

    Program(void) {};
    bool    Link(const std::vector<ShaderSPtr>& shaders);
};

ProgramUPtr Program::Create(const std::vector<ShaderSPtr>& shaders)
{
    ProgramUPtr program = ProgramUPtr(new Program());
    if (!program->Link(shaders))
        return (nullptr);
    return (std::move(program));
};
ProgramUPtr  Program::Create(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
    ShaderSPtr	vertexShader = Shader::CreateFromFile(".\\shader\\shader.vs", GL_VERTEX_SHADER);
	ShaderSPtr	fragmentShader = Shader::CreateFromFile(".\\shader\\shader.fs", GL_FRAGMENT_SHADER);
	if (!vertexShader || !fragmentShader)
        return (nullptr);
    std::cout << "Vertex Shader id: " << vertexShader->Get() << std::endl;
	std::cout << "Fragment Shader id: " << fragmentShader->Get() << std::endl;

    return (std::move(Create({vertexShader, fragmentShader})));
};

Program::~Program(void)
{
    if (this->m_program)
        glDeleteProgram(this->m_program);
};

bool    Program::Link(const std::vector<ShaderSPtr>& shaders)
{
    this->m_program = glCreateProgram();
    for (auto& shader : shaders)
        glAttachShader(this->m_program, shader->Get());
    glLinkProgram(this->m_program);

    int success = 0;
    glGetProgramiv(this->m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char    infoLog[1024];
        glGetProgramInfoLog(this->m_program, 1024, nullptr, infoLog);
        putError("Failed to link program: " + std::string(infoLog));
        return (false);
    }
    return (true);
};

#endif