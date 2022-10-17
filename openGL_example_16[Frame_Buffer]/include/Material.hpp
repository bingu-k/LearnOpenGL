#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Texture.hpp"
#include "Program.hpp"

CLASS_PTR(Material);
class Material
{
public:
    static MaterialUPtr Create(void);

    TextureSPtr diffuse;
    TextureSPtr specular;
    float       shininess { 32.0f };

    void    SetToProgram(const Program* program) const;
private:
    Material() {};
};

MaterialUPtr    Material::Create(void)
{ return (MaterialUPtr(new Material())); };

void    Material::SetToProgram(const Program* program) const
{
    int textureCount = 0;
    if (diffuse)
    {
        glActiveTexture(GL_TEXTURE0 + textureCount);
        program->SetUniform("material.diffuse", textureCount);
        diffuse->Bind();
        ++textureCount;
    }
    if (specular)
    {
        glActiveTexture(GL_TEXTURE0 + textureCount);
        program->SetUniform("material.specular", textureCount);
        specular->Bind();
        ++textureCount;
    }
    glActiveTexture(GL_TEXTURE0);
    program->SetUniform("material.shininess", shininess);
};

#endif