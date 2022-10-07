#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Common.hpp"

CLASS_PTR(Buffer)
class Buffer
{
public:
    static BufferUPtr CreateWithData(uint32_t bufferType, uint32_t usage,
                                    const void* data, size_t  dataSize);

    ~Buffer();
    uint32_t    Get(void) const
    { return (this->m_buffer); };
    void        Bind(void) const
    { glBindBuffer(this->m_bufferType, this->m_buffer); };
private:
    uint32_t    m_buffer{0}, m_bufferType{0}, m_usage{0};

    Buffer(void) {};
    bool    init(uint32_t bufferType, uint32_t usage,
                const void* data, size_t  dataSize);
};

BufferUPtr  Buffer::CreateWithData(uint32_t bufferType, uint32_t usage,
                                    const void* data, size_t  dataSize)
{
    BufferUPtr  buffer = BufferUPtr(new Buffer());
    if (!buffer->init(bufferType, usage, data, dataSize))
        return (nullptr);
    return (std::move(buffer));
};

Buffer::~Buffer()
{
    if (this->m_buffer)
        glDeleteBuffers(1, &this->m_buffer);
};

bool    Buffer::init(uint32_t bufferType, uint32_t usage, 
                    const void* data, size_t  dataSize)
{
    this->m_bufferType = bufferType;
    this->m_usage = usage;
    glGenBuffers(1, &this->m_buffer);
    Bind();
    glBufferData(this->m_bufferType, dataSize, data, this->m_usage);
    return (true);
};

#endif