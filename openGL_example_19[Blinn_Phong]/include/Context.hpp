#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "Common.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "Image.hpp"
#include "Texture.hpp"
#include "FrameBuffer.hpp"
#include "CubeTexture.hpp"
#include "Mesh.hpp"
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
    ProgramUPtr     m_program;
    ProgramUPtr     m_simpleProgram;
    ProgramUPtr     m_textureProgram;
    ProgramUPtr     m_postProgram;
    
    ProgramUPtr     m_gammaProgram;
    GLfloat         m_gamma { 1.0f };

    // Cube Map
    CubeTextureUPtr m_cubeTexture;
    ProgramUPtr     m_skyboxProgram;
    ProgramUPtr     m_envMapProgram;

    // Grass
    TextureSPtr             m_grassTexture;
    ProgramUPtr             m_grassProgram;
    std::vector<glm::vec3>  m_grassPos;
    BufferUPtr              m_grassPosBuffer;
    VertexLayoutUPtr        m_grassInstance;

    MeshUPtr        m_box;
    MeshUPtr        m_plane;

    MaterialSPtr    m_planeMaterial;
    MaterialSPtr    m_box1Material;
    MaterialSPtr    m_box2Material;

    TextureSPtr     m_windowTexture;

    glm::vec4   m_clearColor { glm::vec4(0.1f, 0.2f, 0.3f, 0.0f) };
    bool        m_cameraControl { false };
    glm::vec2   m_prevMousePos { glm::vec2(0.0f) };
    GLfloat     m_cameraPitch { -20.0f };
    GLfloat     m_cameraYaw { 0.0f };
    glm::vec3   m_cameraFront { glm::vec3(0.0f, -1.0f, 0.0f) };
    glm::vec3   m_cameraPos { glm::vec3(0.0f, 2.5f, 8.0f) };
    glm::vec3   m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };

    // Frame Buffer
    FrameBufferUPtr m_framebuffer;
    
    // light parameter
    struct Light {
        glm::vec3   position { glm::vec3(1.0f, 4.0f, 4.0f) };
        glm::vec3   direction { glm::vec3(-1.0f, -1.0f, -1.0f) };
        glm::vec2   cutoff { glm::vec2(120.0f, 5.0f) };
        float       distance { 128.0f };
        glm::vec3   ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
        glm::vec3   diffuse { glm::vec3(0.8f, 0.8f, 0.8f) };
        glm::vec3   specular { glm::vec3(1.0f, 1.0f, 1.0f) };
    };
    Light   m_light;
    bool    m_flashLightMode { false };
    bool    m_blinn { false };

    GLuint  m_width { WINDOW_WIDTH };
    GLuint  m_height { WINDOW_HEIGHT };

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
        ImGui::DragFloat("Gamma", &this->m_gamma, 0.01f, 0.0f, 2.0f);
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
            ImGui::Separator();
            ImGui::Checkbox("blinn Mode", &this->m_blinn);
        }
        ImGui::Separator();
        float   aspectRatio = m_width/m_height;
        ImGui::Image((ImTextureID)m_framebuffer->GetColorAttachment()->Get(),
                    ImVec2(150 * aspectRatio, 150));
    }
    ImGui::End();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);

    m_cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(this->m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(this->m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
                    glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    glm::mat4   projection = glm::perspective(glm::radians(45.0f),
                                            static_cast<float>(this->m_width) / static_cast<float>(this->m_height),
                                            0.1f, 100.0f);
    glm::mat4   view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

    // Sky Box
    auto    skyboxModelTransform = glm::translate(glm::mat4(1.0f), m_cameraPos) *
                                    glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));
    m_skyboxProgram->Use();
    m_cubeTexture->Bind();
    m_skyboxProgram->SetUniform("skybox", 0);
    m_skyboxProgram->SetUniform("transform", projection * view * skyboxModelTransform);
    m_box->Draw(m_skyboxProgram.get());

    // Flash Light Mode;
    glm::vec3   lightPos = this->m_light.position;
    glm::vec3   lightDir = this->m_light.direction;
    if (this->m_flashLightMode)
    {
        lightPos = this->m_cameraPos;
        lightDir = this->m_cameraFront;
    }

    // Lighting ??????
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
    m_program->SetUniform("blinn", (this->m_blinn ? 0 : 1));

    // Plane ??????
    auto    modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)) *
                            glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
    auto    transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_planeMaterial->SetToProgram(m_program.get());
    m_box->Draw(m_program.get());

    // Box1 ??????
    modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.75f, -4.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_box1Material->SetToProgram(m_program.get());
    m_box->Draw(m_program.get());

    // Box2 ??????
    modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.75f, 2.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_box2Material->SetToProgram(m_program.get());
    m_box->Draw(m_program.get());
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
    glViewport(0, 0,this->m_width, this->m_height);

    this->m_framebuffer = FrameBuffer::Create(Texture::Create(width, height, GL_RGBA));
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
    // Multisample Anti-Aliasing
    glEnable(GL_MULTISAMPLE);
    this->m_box = Mesh::CreateBox();
    this->m_plane = Mesh::CreatePlane();

    // Shader, Program ??????
	this->m_program = Program::Create(".\\shader\\lighting.vs", ".\\shader\\lighting.fs");
    if (!this->m_program)
        return (false);
    this->m_simpleProgram = Program::Create(".\\shader\\simple.vs", ".\\shader\\simple.fs");
    if (!this->m_simpleProgram)
        return (false);
    this->m_textureProgram = Program::Create(".\\shader\\texture.vs", ".\\shader\\texture.fs");
    if (!m_textureProgram)
        return (false);
    this->m_postProgram = Program::Create(".\\shader\\texture.vs", ".\\shader\\invert.fs");
    if (!m_postProgram)
        return (false);
    this->m_gammaProgram = Program::Create(".\\shader\\texture.vs", ".\\shader\\gamma.fs");
    if (!m_gammaProgram)
        return (false);

    // Sky Box
    auto    CubeR = Image::Load("./image/skybox/right.jpg", false);
    auto    CubeL = Image::Load("./image/skybox/left.jpg", false);
    auto    CubeU = Image::Load("./image/skybox/top.jpg", false);
    auto    CubeD = Image::Load("./image/skybox/bottom.jpg", false);
    auto    CubeF = Image::Load("./image/skybox/front.jpg", false);
    auto    CubeB = Image::Load("./image/skybox/back.jpg", false);
    this->m_cubeTexture = CubeTexture::CreateFromImages({
            CubeR.get(), 
            CubeL.get(), 
            CubeU.get(), 
            CubeD.get(), 
            CubeF.get(), 
            CubeB.get()
    });
    this->m_skyboxProgram = Program::Create("./shader/skybox.vs", "./shader/skybox.fs");
    if (!m_skyboxProgram)
        return (false);
    this->m_envMapProgram = Program::Create("./shader/envmap.vs", "./shader/envmap.fs");
    if (!m_envMapProgram)
        return (false);

    // Grass
    this->m_grassTexture = Texture::CreateFromImage(Image::Load("./image/grass.png").get());
    this->m_grassProgram = Program::Create("./shader/grass.vs", "./shader/grass.fs");
    this->m_grassPos.resize(10000);
    for (size_t idx = 0; idx < m_grassPos.size(); ++idx)
    {
        m_grassPos[idx].x = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * 5.0f;
        m_grassPos[idx].z = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * 5.0f;
        m_grassPos[idx].y = glm::radians((float)rand() / (float)RAND_MAX * 360.0f);
    }
    
    // ???????????? Instancing : VertexShader?????? pos, normal ?????? ???????????? ????????? ??????????????? ???.
    m_grassInstance = VertexLayout::Create();
    m_grassInstance->Bind();
    m_plane->GetVertexBuffer()->Bind();
    m_grassInstance->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
    m_grassInstance->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
    m_grassInstance->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));

    m_grassPosBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW,
                                        m_grassPos.data(), sizeof(glm::vec3),
                                        m_grassPos.size());
    m_grassPosBuffer->Bind();
    m_grassInstance->SetAttrib(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glVertexAttribDivisor(3, 1);
    m_plane->GetIndexBuffer()->Bind();

    // Texture ??????
    TextureSPtr darkGrayTexture = Texture::CreateFromImage(
                                    Image::CreateSingleColorImage(4, 4,
                                    glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)).get());
    TextureSPtr grayTexture = Texture::CreateFromImage(
                                    Image::CreateSingleColorImage(4, 4,
                                    glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)).get());

    this->m_windowTexture = Texture::CreateFromImage(
                                    Image::Load("./image/blending_transparent_window.png").get());

    m_planeMaterial = Material::Create();
    m_planeMaterial->diffuse = Texture::CreateFromImage(Image::Load(".\\image\\marble.jpg").get());
    m_planeMaterial->specular = grayTexture;
    m_planeMaterial->shininess = 4.0f;

    m_box1Material = Material::Create();
    m_box1Material->diffuse = Texture::CreateFromImage(Image::Load(".\\image\\container.jpg").get());
    m_box1Material->specular = darkGrayTexture;
    m_box1Material->shininess = 16.0f;

    m_box2Material = Material::Create();
    m_box2Material->diffuse = Texture::CreateFromImage(Image::Load(".\\image\\container2.png").get());
    m_box2Material->specular = Texture::CreateFromImage(Image::Load(".\\image\\container2_specular.png").get());;
    m_box2Material->shininess = 64.0f;

    // ?????? Clear ?????? ??????
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    return (true);
};

#endif