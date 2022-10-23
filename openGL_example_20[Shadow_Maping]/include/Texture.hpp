#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Common.hpp"

CLASS_PTR(Texture);
class Texture
{
public:
    static TextureUPtr  Create(int width, int height,
                            uint32_t format, uint32_t type = GL_UNSIGNED_BYTE);
    static TextureUPtr  CreateFromImage(const Image* image);

    ~Texture();
    const uint32_t  Get() const { return (this->m_texture); };
    int             GetWidth() const { return (this->m_width); };
    int             GetHeight() const { return (this->m_height); };
    int             GetFormat() const { return (this->m_format); };
    uint32_t        GetType() const { return (this->m_type); };

    void    Bind() const
    { glBindTexture(GL_TEXTURE_2D, this->m_texture); };
    void    SetFilter(uint32_t minFilter, uint32_t magFilter) const;
    void    SetWrap(uint32_t sWrap, uint32_t tWrap) const;
    void    SetBorderColor(const glm::vec4& color) const;
private:
    uint32_t    m_texture{0};

    int         m_width {0}, m_height {0};
    uint32_t    m_format { GL_RGBA }, m_type { GL_UNSIGNED_BYTE };

    Texture() {};
    void    CreateTexture(void);
    void    SetTextureFromImage(const Image* image);
    void    SetTextureFormat(int width, int height, uint32_t format, uint32_t type);
};

TextureUPtr Texture::Create(int width, int height, uint32_t format, uint32_t type)
{
    TextureUPtr texture = TextureUPtr(new Texture());
    texture->CreateTexture();
    texture->SetTextureFormat(width, height, format, type);
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    return (std::move(texture));
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

void    Texture::SetWrap(uint32_t sWrap, uint32_t tWrap) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tWrap);
};

void    Texture::SetBorderColor(const glm::vec4& color) const
{ glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color)); };

void    Texture::CreateTexture(void)
{
    glGenTextures(1, &this->m_texture);
    Bind();
    SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
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
    this->m_width = image->GetWidth();
    this->m_height = image->GetHeight();
    this->m_format = format;
    this->m_type = GL_UNSIGNED_BYTE;

    glTexImage2D(GL_TEXTURE_2D, 0, this->m_format, this->m_width, this->m_height, 0,
                this->m_format, this->m_type, image->GetData());
    glGenerateMipmap(GL_TEXTURE_2D);
};

void    Texture::SetTextureFormat(int width, int height, uint32_t format, uint32_t type)
{
    this->m_width = width;
    this->m_height = height;
    this->m_format = format;
    this->m_type = type;

    glTexImage2D(GL_TEXTURE_2D, 0, this->m_format, this->m_width, this->m_height, 0,
                this->m_format, this->m_type, nullptr);
};

#endif