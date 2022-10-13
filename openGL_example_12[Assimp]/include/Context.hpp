#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "Common.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "Image.hpp"
#include "Texture.hpp"
#include "Model.hpp"
#include <imgui.h>

CLASS_PTR(Context);
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
    ProgramUPtr         m_simpleProgram;

    MeshUPtr            m_box;
    ModelUPtr           m_model;
    TextureUPtr         m_texture1;
    TextureUPtr         m_texture2;

    glm::vec4           m_clearColor { glm::vec4(0.1f, 0.2f, 0.3f, 0.0f) };
    bool                m_cameraControl { false };
    glm::vec2           m_prevMousePos { glm::vec2(0.0f) };
    GLfloat             m_cameraPitch { 0.0f };
    GLfloat             m_cameraYaw { 0.0f };
    glm::vec3           m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
    glm::vec3           m_cameraFront { glm::vec3(0.0f, -1.0f, 0.0f) };
    glm::vec3           m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };
    
    // light parameter
    struct Light {
        glm::vec3   position { glm::vec3(2.0f, 2.0f, 2.0f) };
        glm::vec3   direction { glm::vec3(-1.0f, -1.0f, -1.0f) };
        glm::vec2   cutoff { glm::vec2(20.0f, 5.0f) };
        float       distance { 32.0f };
        glm::vec3   ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
        glm::vec3   diffuse { glm::vec3(0.5f, 0.5f, 0.5f) };
        glm::vec3   specular { glm::vec3(1.0f, 1.0f, 1.0f) };
    };
    Light       m_light;
    bool        m_flashLightMode { false };

    struct Material {
        TextureUPtr diffuse;
        TextureUPtr specular;
        float       shininess { 32.0f };
    };
    Material    m_material;

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
    if (ImGui::Begin("ui window", 0, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor)))
            glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
        ImGui::Separator();
        ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
        ImGui::DragFloat("camera yaw", &m_cameraYaw, 0.5f);
        ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
        ImGui::Separator();
        if (ImGui::Button("reset camera")) {
            m_cameraYaw = 0.0f;
            m_cameraPitch = 0.0f;
            m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("Light Position", glm::value_ptr(this->m_light.position), 0.01f);
            ImGui::DragFloat3("Light Direction", glm::value_ptr(this->m_light.direction), 0.01f);
            ImGui::DragFloat2("Light Cutoff", glm::value_ptr(this->m_light.cutoff), 0.5f, 0.0f, 180.0f);
            ImGui::DragFloat("Light Distance", &this->m_light.distance, 0.1f, 0.0f, 3000.0f);
            ImGui::ColorEdit3("Ambient", glm::value_ptr(this->m_light.ambient));
            ImGui::ColorEdit3("Diffuse", glm::value_ptr(this->m_light.diffuse));
            ImGui::ColorEdit3("Specular", glm::value_ptr(this->m_light.specular));
            ImGui::Separator();
            ImGui::Checkbox("Flash Light Mode", &this->m_flashLightMode);
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat("Shininess", &m_material.shininess, 1.0f, 1.0f, 256.0f);
        }
    }
    ImGui::End();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(this->m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(this->m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
                    glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    glm::mat4   projection = glm::perspective(glm::radians(45.0f),
                                            static_cast<float>(this->m_width) / static_cast<float>(this->m_height),
                                            0.01f, 100.0f);

    glm::mat4   view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);


    // Flash Light mode;
    glm::vec3   lightPos = this->m_light.position;
    glm::vec3   lightDir = this->m_light.direction;
    if (this->m_flashLightMode)
    {
        lightPos = this->m_cameraPos;
        lightDir = this->m_cameraFront;
    }
    else
    {
        // Light Cube 생성
        auto    lightModelTransform = glm::translate(glm::mat4(1.0), this->m_light.position)
                                    * glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
        m_simpleProgram->Use();
        m_simpleProgram->SetUniform("Color", glm::vec4(m_light.ambient + m_light.diffuse, 1.0f));
        m_simpleProgram->SetUniform("transform", projection * view * lightModelTransform);
        this->m_box->Draw(this->m_program.get());
    }

    // Color Cubes 생성
    m_program->Use();
    m_program->SetUniform("viewPos", this->m_cameraPos);

    m_program->SetUniform("light.position", lightPos);
    m_program->SetUniform("light.direction", lightDir);
    m_program->SetUniform("light.cutoff", 
                        glm::vec2(cosf(glm::radians(this->m_light.cutoff[0])),
                                cosf(glm::radians(this->m_light.cutoff[0] + this->m_light.cutoff[1]))));
    m_program->SetUniform("light.attenuation", GetAttenuationCoeff(m_light.distance));
    m_program->SetUniform("light.ambient", this->m_light.ambient);
    m_program->SetUniform("light.diffuse", this->m_light.diffuse);
    m_program->SetUniform("light.specular", this->m_light.specular);

    m_program->SetUniform("material.diffuse", 0);
    m_program->SetUniform("material.specular", 1);
    m_program->SetUniform("material.shininess", this->m_material.shininess);
    
    glActiveTexture(GL_TEXTURE0);
    m_material.diffuse->Bind();
    glActiveTexture(GL_TEXTURE1);
    m_material.specular->Bind();

    auto modelTransform = glm::mat4(1.0f);
    auto transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_model->Draw(this->m_program.get());
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

    const float cameraRotSpeed = 0.08f;
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
    this->m_box = Mesh::CreateBox();
    this->m_model = Model::Load(".\\model\\backpack.obj");
    if (!this->m_model)
        return (false);

    // Shader, Program 생성
	this->m_program = Program::Create(".\\shader\\shader.vs", ".\\shader\\shader.fs");
    if (!this->m_program)
        return (false);

    // Texture 설정
    m_material.diffuse = Texture::CreateFromImage(Image::CreateSingleColorImage(4, 4,
                                                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)).get());
    m_material.specular = Texture::CreateFromImage(Image::CreateSingleColorImage(4, 4,
                                                glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)).get());
	
    this->m_simpleProgram = Program::Create(".\\shader\\simple.vs", ".\\shader\\simple.fs");
    if (!this->m_simpleProgram)
        return (false);

    // 배경 Clear 색상 지정
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    return (true);
};

#endif