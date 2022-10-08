#ifndef VERTEXLAYOUT_HPP
#define VERTEXLAYOUR_HPP

#include "Common.hpp"

CLASS_PTR(VertexLayout)
class VertexLayout
{
public:
    static  VertexLayoutUPtr    Create(void);
    ~VertexLayout();

    uint32_t    Get(void) const
    { return (this->m_VAO); };
    void    Bind(void) const
    { glBindVertexArray(this->m_VAO); };
    void    SetAttrib(uint32_t attribIndex, int count,
                        uint32_t type, bool normalized,
                        size_t stride, uint64_t offset) const;
    void    DisableAttrib(int attribIndex) const;
private:
    uint32_t    m_VAO;

    VertexLayout() {};
    void    init();
};

VertexLayoutUPtr    VertexLayout::Create(void)
{
    VertexLayoutUPtr    vertexLayout = VertexLayoutUPtr(new VertexLayout());
    vertexLayout->init();
    return (std::move(vertexLayout));
};

VertexLayout::~VertexLayout()
{
    if (this->m_VAO)
        glDeleteVertexArrays(1, &this->m_VAO);
};

void    VertexLayout::SetAttrib(uint32_t attribIndex, int count,
                uint32_t type, bool normalized,
                size_t stride, uint64_t offset) const
{
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, count, type, normalized, stride, (const void*)offset);
};

void    VertexLayout::DisableAttrib(int attribIndex) const
{ glDisableVertexAttribArray(attribIndex); };

void    VertexLayout::init(void)
{
    glGenVertexArrays(1, &this->m_VAO);
    Bind();
};

#endif