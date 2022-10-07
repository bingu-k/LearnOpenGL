#ifndef IMAGE_HPP
#define IMAGE_HPP

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Common.hpp"

CLASS_PTR(Image)
class Image
{
public:
    static ImageUPtr    Load(const std::string& filepath);
    static ImageUPtr    Create(int width, int height, int channelCount = 4);

    ~Image();
    const uint8_t*  GetData() const
    { return (this->m_data); };
    const int       GetWidth() const
    { return (this->m_width); };
    const int       GetHeight() const
    { return (this->m_height); };
    const int       GetChannelCount() const
    { return (this->m_channelCount); };

    void    SetCheckImage(int gridX, int gridY);

private:
    int         m_width{0}, m_height{0}, m_channelCount{0};
    uint8_t*    m_data {nullptr};

    Image() {};
    bool    LoadWithStb(const std::string& filepath);
    bool    Allocate(int width, int height, int channelCount);
};

ImageUPtr    Image::Load(const std::string& filepath)
{
    ImageUPtr   image = ImageUPtr(new Image());
    if (!image->LoadWithStb(filepath))
        return (nullptr);
    return (image);
};

ImageUPtr    Image::Create(int width, int height, int channelCount)
{
    auto image = ImageUPtr(new Image());
    if (!image->Allocate(width, height, channelCount))
        return (nullptr);
    return (std::move(image));
};

Image::~Image()
{
    if (this->m_data)
        stbi_image_free(this->m_data);
};

void    Image::SetCheckImage(int gridX, int gridY)
{
    for (int j = 0; j < this->m_height; ++j)
    {
        for (int i = 0; i < this->m_width; ++i)
        {
            int pos = (j * this->m_width + i) * this->m_channelCount;
            bool    even = ((i / gridX) + (j / gridY)) % 2 == 0;
            uint8_t value = even ? 255 : 0;
            for (int k = 0; k < this->m_channelCount; k++)
                this->m_data[pos + k] = value;
            if (this->m_channelCount > 3)
                this->m_data[3] == 255;
        }
    }
};

bool    Image::LoadWithStb(const std::string& filepath)
{
    stbi_set_flip_vertically_on_load(true);
    this->m_data = stbi_load(filepath.c_str(),
                            &this->m_width, &this->m_height,
                            &this->m_channelCount, 0);
    if (!this->m_data)
    {
        putError("Failed to load image: " + filepath);
        return (false);
    }
    return (true);
};

bool    Image::Allocate(int width, int height, int channelCount)
{
    this->m_width = width;
    this->m_height = height;
    this->m_channelCount = channelCount;
    this->m_data = (uint8_t*)malloc(this->m_width * this->m_height * this->m_channelCount);
    return (this->m_data ? true : false);
};

#endif