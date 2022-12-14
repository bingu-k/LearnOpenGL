#ifndef MODEL_HPP
#define MODEL_HPP

#include "Common.hpp"
#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

CLASS_PTR(Model);
class Model
{
public:
    static  ModelUPtr   Load(const std::string& filename);

    int     GetMeshCount(void) const
    { return (static_cast<int>(this->m_meshes.size())); };
    MeshSPtr    GetMesh(int index) const
    { return (m_meshes[index]) ;};
    void        Draw(const Program* program) const;
private:
    std::vector<MeshSPtr>       m_meshes;
    std::vector<MaterialSPtr>   m_materials;

    Model() {};
    bool    LoadByAssimp(const std::string& filename);
    void    ProcessNode(aiNode* node, const aiScene* scene);
    void    ProcessMesh(aiMesh* mesh, const aiScene* scene);
};

ModelUPtr   Model::Load(const std::string& filename)
{
    ModelUPtr   model = ModelUPtr(new Model());
    if (!model->LoadByAssimp(filename))
        return (nullptr);
    return (std::move(model));
};

void    Model::Draw(const Program* program) const
{
    for (auto& mesh : this->m_meshes)
        mesh->Draw(program);
};

bool    Model::LoadByAssimp(const std::string& filename)
{
    Assimp::Importer    importer;
    auto    scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        putError("Failed to load model: " + filename);
        return (false);
    }

    auto    dirname = filename.substr(0, filename.find_last_of("\\"));
    auto    LoadTexture = [&](aiMaterial* material, aiTextureType type) -> TextureSPtr
    {
        if (material->GetTextureCount(type) <= 0)
            return nullptr;
        aiString    filepath;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath);
        auto    image = Image::Load(dirname + "\\" + std::string(filepath.C_Str()));
        if (!image)
            return nullptr;
        return (Texture::CreateFromImage(image.get()));
    };

    for (uint32_t i = 0; i < scene->mNumMaterials; ++i)
    {
        auto    material = scene->mMaterials[i];
        auto    glMaterial = Material::Create();
        glMaterial->diffuse = LoadTexture(material, aiTextureType_DIFFUSE);
        glMaterial->specular = LoadTexture(material, aiTextureType_SPECULAR);
        m_materials.push_back(std::move(glMaterial));
    }

    ProcessNode(scene->mRootNode, scene);
    return (true);
};

void    Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (uint32_t idx = 0; idx < node->mNumMeshes; ++idx)
    {
        // ?????? ????????? ???????????? ?????????.
        uint32_t    meshIdx = node->mMeshes[idx];
        // ???????????? ????????? SCENE?????? ?????????.
        aiMesh*     mesh = scene->mMeshes[meshIdx];
        ProcessMesh(mesh, scene);
    }

    // ??????????????? ????????? ????????? ???????????? ???????????? ????????????.
    for (uint32_t idx = 0; idx < node->mNumChildren; ++idx)
        ProcessNode(node->mChildren[idx], scene);
};

void    Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // std::cout << "Process Mesh: " << std::string(mesh->mName.C_Str())
    //             << ", #Vert: " << mesh->mNumVertices
    //             << ", #face: " << mesh->mNumFaces << std::endl;

    // ???????????? Mesh?????? Vertices??? ?????? ??????.
    std::vector<Vertex> vertices;
    vertices.resize(mesh->mNumVertices);
    for (uint32_t idx = 0; idx < mesh->mNumVertices; ++idx)
    {
        auto&  vertex = vertices.at(idx);
        vertex.position = glm::vec3(mesh->mVertices[idx].x, mesh->mVertices[idx].y, mesh->mVertices[idx].z);
        vertex.normal = glm::vec3(mesh->mNormals[idx].x, mesh->mNormals[idx].y, mesh->mNormals[idx].z);
        vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][idx].x, mesh->mTextureCoords[0][idx].y);
    }

    // ???????????? Mesh?????? Indices??? ?????? ??????.
    std::vector<uint32_t>   indices;
    indices.resize(mesh->mNumFaces * 3);
    for (uint32_t idx = 0; idx < mesh->mNumFaces; ++idx)
    {
        indices[3 * idx] = mesh->mFaces[idx].mIndices[0];
        indices[3 * idx + 1] = mesh->mFaces[idx].mIndices[1];
        indices[3 * idx + 2] = mesh->mFaces[idx].mIndices[2];
    }

    // ???????????? Mesh?????? ?????? ????????? ????????? ?????? Mesh??? ??????
    MeshSPtr    glMesh = Mesh::Create(vertices, indices, GL_TRIANGLES);
    if (mesh->mMaterialIndex >= 0)
        glMesh->SetMaterial(m_materials[mesh->mMaterialIndex]);
    this->m_meshes.push_back(std::move(glMesh));
};

#endif