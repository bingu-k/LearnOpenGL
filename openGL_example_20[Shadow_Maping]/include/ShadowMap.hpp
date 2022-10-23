#ifndef SHADOWMAP_HPP
#define SHADOWMAP_HPP

#include "Texture.hpp"

CLASS_PTR(ShadowMap);
class ShadowMap {
public:
    static ShadowMapUPtr    Create(int width, int height);
    ~ShadowMap();

    const uint32_t      Get() const { return (m_frameBuffer); };
    void                Bind() const;
    const TextureSPtr    GetShadowMap() const { return (m_shadowMap); };

private:
    uint32_t    m_frameBuffer { 0 };
    TextureSPtr m_shadowMap;

    ShadowMap() {};
    bool    Init(int width, int height);
};

ShadowMapUPtr   ShadowMap::Create(int width, int height) {
    auto shadowMap = ShadowMapUPtr(new ShadowMap());
    if (!shadowMap->Init(width, height))
        return (nullptr);
    return (std::move(shadowMap));
}

ShadowMap::~ShadowMap() {
    if (m_frameBuffer)
        glDeleteFramebuffers(1, &m_frameBuffer);
}

void    ShadowMap::Bind() const
{ glBindFramebuffer(GL_FRAMEBUFFER, this->m_frameBuffer); }

bool    ShadowMap::Init(int width, int height)
{
    glGenFramebuffers(1, &m_frameBuffer);
    Bind();

    m_shadowMap = Texture::Create(width, height, GL_DEPTH_COMPONENT, GL_FLOAT);
    m_shadowMap->SetFilter(GL_NEAREST, GL_NEAREST);
    m_shadowMap->SetWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    m_shadowMap->SetBorderColor(glm::vec4(1.0f));
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                        m_shadowMap->Get(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    auto    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        putError("failed to complete shadow map framebuffer: " + status);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return (false);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return (true);
}

#endif