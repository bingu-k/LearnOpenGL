#ifndef SHADER_HPP
#define SHADER_HPP

#include "Common.hpp"

CLASS_PTR(Shader);
class Shader
{
public:
    static  ShaderUPtr  CreateFromFile(std::string filename, GLenum shaderType);

    ~Shader(void);
    uint32_t    Get() const {return this->m_shader; };
private:
    uint32_t    m_shader { 0 };

    Shader(void) {};
    bool    LoadFile(const std::string& filename, GLenum shaderType);
};

Shader::~Shader(void)
{
    if (this->m_shader)
        glDeleteShader(this->m_shader);
}

ShaderUPtr  Shader::CreateFromFile(std::string filename, GLenum shaderType)
{
    ShaderUPtr  shader = ShaderUPtr(new Shader());
    if (!shader->LoadFile(filename, shaderType))
        return (nullptr);
    return (std::move(shader));
}

bool    Shader::LoadFile(const std::string& filename, GLenum shaderType)
{
    auto    result = LoadTextFile(filename);
    if (!result.has_value())
        return (false);
    
    std::string&    code = result.value();
    const char*     codePtr = code.c_str();
    int32_t         codeLength = static_cast<int32_t>(code.length());

    this->m_shader = glCreateShader(shaderType);
    glShaderSource(this->m_shader, 1, (const GLchar* const*)&codePtr, &codeLength);
    glCompileShader(this->m_shader);

    int success = 0;
    glGetShaderiv(this->m_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char    infoLog[1024];
        glGetShaderInfoLog(this->m_shader, 1024, nullptr, infoLog);
        putError("Failed to Compile Shader: " + filename);
        putError("Reason: " + std::string(infoLog));
        return (false);
    }
    return (true);
}

#endif