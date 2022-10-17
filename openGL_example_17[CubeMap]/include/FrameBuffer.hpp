#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "Common.hpp"
#include "Texture.hpp"

CLASS_PTR(FrameBuffer);
class FrameBuffer
{
public:
    static FrameBufferUPtr  Create(const TextureSPtr colorAttachment);
    static void             BindToDefault();
    ~FrameBuffer();

    const uint32_t      Get() const { return (this->m_FrameBuffer); };
    const TextureSPtr   GetColorAttachment() const { return (this->m_colorAttachment); };
    void                Bind() const;
private:
    uint32_t    m_FrameBuffer {0}, m_DepthStencilBuffer {0};
    TextureSPtr m_colorAttachment;

    FrameBuffer() {};
    bool    InitWithColorAttachment(const TextureSPtr colorAttachment);
};

FrameBufferUPtr FrameBuffer::Create(const TextureSPtr colorAttachment)
{
    FrameBufferUPtr frameBuffer = FrameBufferUPtr(new FrameBuffer());
    if (!(frameBuffer->InitWithColorAttachment(colorAttachment)))
        return (nullptr);
    return (std::move(frameBuffer));
};

void    FrameBuffer::BindToDefault()
{ glBindFramebuffer(GL_FRAMEBUFFER, 0); };

void    FrameBuffer::Bind() const
{ glBindFramebuffer(GL_FRAMEBUFFER, this->m_FrameBuffer); };

FrameBuffer::~FrameBuffer()
{
    if (m_DepthStencilBuffer)
        glDeleteRenderbuffers(1, &m_DepthStencilBuffer);
    if (m_FrameBuffer)
        glDeleteFramebuffers(1, &m_FrameBuffer);
};

bool    FrameBuffer::InitWithColorAttachment(const TextureSPtr colorAttachment)
{
    m_colorAttachment = colorAttachment;
    glGenFramebuffers(1, &this->m_FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_FrameBuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                        colorAttachment->Get(), 0);
    
    glGenRenderbuffers(1, &this->m_DepthStencilBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, this->m_DepthStencilBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                        colorAttachment->GetWidth(), colorAttachment->GetHeight());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, this->m_DepthStencilBuffer);
    auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result != GL_FRAMEBUFFER_COMPLETE) {
        putError("failed to create FrameBuffer: " + result);
        return false;
    }
    BindToDefault();
    return true;
};

#endif