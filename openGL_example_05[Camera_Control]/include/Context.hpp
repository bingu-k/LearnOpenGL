#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "Common.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "Buffer.hpp"
#include "VertexLayout.hpp"
#include "Image.hpp"
#include "Texture.hpp"

CLASS_PTR(Context)
class Context
{
public:
    static ContextUPtr  Create(void);

    void    Render(void);
    void    ProcessInput(GLFWwindow* window);
    void    Reshape(GLuint width, GLuint height);
    void    MouseMove(double x, double y);
    void    MouseButton(int button, int action, double x, double y);
private:
    ProgramUPtr         m_program;
    VertexLayoutUPtr    m_VAO;
    BufferUPtr          m_VBO, m_EBO;
    TextureUPtr         m_texture1;
    TextureUPtr         m_texture2;

    bool                m_cameraControl { false };
    glm::vec2           m_prevMousePos { glm::vec2(0.0f) };
    GLfloat             m_cameraPitch { 0.0f };
    GLfloat             m_cameraYaw { 0.0f };
    glm::vec3           m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
    glm::vec3           m_cameraFront { glm::vec3(0.0f, -1.0f, 0.0f) };
    glm::vec3           m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };

    GLuint              m_width { WINDOW_WIDTH };
    GLuint              m_height { WINDOW_HEIGHT };


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
    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_program->Use();
    m_program->SetUniform("tex1", 0);
    m_program->SetUniform("tex2", 1);

    m_cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(this->m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(this->m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
                    glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    glm::mat4   projection = glm::perspective(glm::radians(45.0f),
                                            static_cast<float>(this->m_width) / static_cast<float>(this->m_height),
                                            0.01f, 20.0f);

    glm::mat4   view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

    for (glm::vec3& pos : cubePositions)
    {
        glm::mat4   model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model,
                            glm::radians(static_cast<float>(glfwGetTime() * 120.0f)),
                            glm::vec3(1.0f, 0.5f, 0.0f));
        glm::mat4   transform = projection * view * model;
        m_program->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
};

void    Context::ProcessInput(GLFWwindow* window)
{
    if (!this->m_cameraControl)
        return ;
    const float cameraVel = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_cameraPos += cameraVel * m_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_cameraPos -= cameraVel * m_cameraFront;

    glm::vec3   cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_cameraPos += cameraVel * cameraRight;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_cameraPos -= cameraVel * cameraRight;
        
    glm::vec3   cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        m_cameraPos += cameraVel * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        m_cameraPos -= cameraVel * cameraUp;
};

void    Context::Reshape(GLuint width, GLuint height)
{
    this->m_width = width;
    this->m_height = height;
    glViewport(0, 0,this-> m_width, this->m_height);
};

void    Context::MouseMove(double x, double y)
{
    if (!this->m_cameraControl)
        return ;
    glm::vec2   pos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
    glm::vec2   deltaPos = pos - this->m_prevMousePos;

    const float cameraRotSpeed = 0.8f;
    m_cameraYaw -= deltaPos.x * cameraRotSpeed;
    m_cameraPitch -= deltaPos.y * cameraRotSpeed;

    if (m_cameraYaw < 0.0f)
        m_cameraYaw += 360.0f;
    if (m_cameraYaw > 360.0f)
        m_cameraYaw -= 360.0f;

    if (m_cameraPitch > 89.0f)
        m_cameraPitch = 89.0f;
    if (m_cameraPitch < -89.0f)
        m_cameraPitch = -89.0f;

    this->m_prevMousePos = pos;
};

void    Context::MouseButton(int button, int action, double x, double y)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            m_prevMousePos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
            m_cameraControl = true;
        }
        else if (action == GLFW_RELEASE)
            m_cameraControl = false;
    }
};

bool    Context::init(void)
{
    // 기본 Vertex들 생성
    float vertices[] = {
        // Position             Color
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,

        0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,     0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,
    };

    uint32_t indices[] = {
        0,  2,  1,  2,  0,  3,
        4,  5,  6,  6,  7,  4,
        8,  9, 10, 10, 11,  8,
        12, 14, 13, 14, 12, 15,
        16, 17, 18, 18, 19, 16,
        20, 22, 21, 22, 20, 23,
    };

    // Buffer 설정
        // Vertex Array
    m_VAO = VertexLayout::Create();
        // Vertex Buffer
    m_VBO = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW,
                                    vertices, sizeof(float) * std::size(vertices));
        // Vertex Element Buffer
    m_EBO = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices,
                                    sizeof(uint32_t) * std::size(indices));
        // Vertex Attribute layout
    m_VAO->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    m_VAO->SetAttrib(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);

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

    // Image Load
    ImageUPtr   image1 = Image::Load(".\\image\\container.jpg");
    if (!image1)
        return (false);
    this->m_texture1 = Texture::CreateFromImage(image1.get());

    ImageUPtr   image2 = Image::Load(".\\image\\awesomeface.png");
    this->m_texture2 = Texture::CreateFromImage(image2.get());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture1->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());


    return (true);
};

#endif