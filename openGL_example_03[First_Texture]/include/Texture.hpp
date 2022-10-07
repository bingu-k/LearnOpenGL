#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Common.hpp"

CLASS_PTR(Texture)
class Texture
{
public:
    static TextureUPtr  CreateFromImage(const Image* image);

    ~Texture();
    const uint32_t  Get() const
    { return (this->m_texture); };
    void    Bind() const
    { glBindTexture(GL_TEXTURE_2D, this->m_texture); };
    void    SetFilter(uint32_t minFilter, uint32_t magFilter) const;
    void    setWrap(uint32_t sWrap, uint32_t tWrap) const;
private:
    uint32_t    m_texture{0};

    Texture() {};
    void    CreateTexture(void);
    void    SetTextureFromImage(const Image* image);
};

TextureUPtr Texture::CreateFromImage(const Image* image)
{
    TextureUPtr texture = TextureUPtr(new Texture());
    texture->CreateTexture();
    texture->SetTextureFromImage(image);
    return (std::move(texture));
};

Texture::~Texture()
{
    if (this->m_texture)
        glDeleteTextures(1, &this->m_texture);
};

void    Texture::SetFilter(uint32_t minFilter, uint32_t magFilter) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
};

void    Texture::setWrap(uint32_t sWrap, uint32_t tWrap) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tWrap);
};

void    Texture::CreateTexture(void)
{
    glGenTextures(1, &this->m_texture);
    Bind();
    SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
};

void    Texture::SetTextureFromImage(const Image* image)
{
    GLenum format = GL_RGBA;
    switch (image->GetChannelCount())
    {
    default:
        break;
    case 1:
        format = GL_RED;
        break;
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                image->GetWidth(), image->GetHeight(), 0,
                format, GL_UNSIGNED_BYTE, image->GetData());
    glGenerateMipmap(GL_TEXTURE_2D);
};

#endif