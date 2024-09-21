#include "Mesh.h"

#include "imgui/imgui.h"

#include <unordered_map>
#include <sstream>

#include "Surface.h"


ModelException::ModelException(int line, const char* file, std::string note) noexcept
    :
    ModException(line, file),
    note(std::move(note))
{}

const char* ModelException::what() const noexcept
{
    std::ostringstream oss;
    oss << ModException::what() << std::endl
        << "[Note] " << GetNote();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* ModelException::GetType() const noexcept
{
    return "Model Exception";
}

const std::string& ModelException::GetNote() const noexcept
{
    return note;
}


//=======================================================|||
//                                                       |||
//                          MESH                         |||
//                                                       |||
//=======================================================|||

Mesh::Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs)
{
    AddBind(Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    for (auto& pb : bindPtrs)
    {
        AddBind(std::move(pb));
    }

    AddBind(std::make_shared<Bind::TransformCbuf>(gfx, *this));
}

void Mesh::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept
{
    DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
    Drawable::Draw(gfx);
}

DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
    return DirectX::XMLoadFloat4x4(&transform);
}


///=======================================================|||
///                                                       |||
///                          NODE                         |||
///                                                       |||
///=======================================================|||


Node::Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& trans)
    :
    id(id),
    meshPtrs(std::move(meshPtrs)),
    name(name)
{
    DirectX::XMStoreFloat4x4(&transform, trans);
    DirectX::XMStoreFloat4x4(&appliedTransform, DirectX::XMMatrixIdentity());
}

void Node::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept
{
    const auto built =
        DirectX::XMLoadFloat4x4(&appliedTransform) *
        DirectX::XMLoadFloat4x4(&transform) *
        accumulatedTransform;

    for (const auto p : meshPtrs)
    {
        p->Draw(gfx, built);
    }

    for (const auto& p : childPtrs)
    {
        p->Draw(gfx, built);
    }

}

void Node::AddChild(std::unique_ptr<Node> pChild) noexcept
{
    assert(pChild);
    childPtrs.push_back(std::move(pChild));
}

void Node::ShowTree(Node*& pSelectedNode) const noexcept
{

    const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();

    const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
        | ((GetId() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
        | ((childPtrs.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);


    const auto expanded = ImGui::TreeNodeEx(
        (void*)(intptr_t)GetId(), node_flags, name.c_str());

    // processing for selecting node
    if (ImGui::IsItemClicked())
    {

        pSelectedNode = const_cast<Node*>(this);
    }

    if (expanded)
    {
        for (const auto& pChild : childPtrs)
        {
            pChild->ShowTree(pSelectedNode);
        }
        ImGui::TreePop();
    }


}


void Node::SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept
{
    DirectX::XMStoreFloat4x4(&appliedTransform, transform);
}


int Node::GetId() const noexcept
{
    return id;
}

///========================================================|||
///                                                        |||
///                          MODEL                         |||
///                                                        |||
///========================================================|||


class ModelWindow
{
public:
    void Show(Graphics& gfx, const char* windowName, const Node& root) noexcept
    {
        windowName = windowName ? windowName : "Model";

        int nodeIndexTracker = 0;

        if (ImGui::Begin(windowName))
        {
            ImGui::Columns(2, nullptr, true);
            root.ShowTree(pSelectedNode);

            ImGui::NextColumn();
            if (pSelectedNode != nullptr)
            {
                auto& transform = transforms[pSelectedNode->GetId()];
                ImGui::Text("Orientation");
                ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
                ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
                ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);
                ImGui::Text("Position");
                ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f);
                ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f);
                ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f);
                
                if (!pSelectedNode->ControlWND(gfx, skinMaterial))
                {
                    pSelectedNode->ControlWND(gfx, ringMaterial);
                }
            }
        }
        ImGui::End();
    }


    DirectX::XMMATRIX GetTransform() const noexcept
    {
        assert(pSelectedNode != nullptr);
        const auto& transform = transforms.at(pSelectedNode->GetId());

        return
            DirectX::XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *
            DirectX::XMMatrixTranslation(transform.x, transform.y, transform.z);
    }

    Node* GetSelectedNode() const noexcept
    {
        return pSelectedNode;
    }

private:

    Node* pSelectedNode;

    struct TransformParameters
    {
        float roll = 0.0f;
        float pitch = 0.0f;
        float yaw = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };

    Node::PSMaterialConstantFullmonte skinMaterial; 
    Node::PSMaterialConstantNotex ringMaterial;

    std::unordered_map<int, TransformParameters> transforms;
};


Model::Model(Graphics& gfx, const std::string fileName)
    :
    pWindow(std::make_unique<ModelWindow>())
{
    Assimp::Importer imp;
    const auto pScene = imp.ReadFile(fileName.c_str(),
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ConvertToLeftHanded |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace
    );

    if (pScene == nullptr)
    {
        throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
    }

    for (size_t i = 0; i < pScene->mNumMeshes; i++)
    {
        meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials));
    }

    int nextId = 0;
    pRoot = ParseNode(nextId, *pScene->mRootNode);
}

Model::~Model() noexcept {}

void Model::Draw(Graphics& gfx) const
{

    if (auto node = pWindow->GetSelectedNode())
    {
        node->SetAppliedTransform(pWindow->GetTransform());
    }
    pRoot->Draw(gfx, DirectX::XMMatrixIdentity());


}

void Model::ShowWindow(Graphics& gfx, const char* windowName) noexcept
{
    pWindow->Show(gfx, windowName, *pRoot);
}


std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh,
    const aiMaterial* const* pMaterials)
{
    using DVS::VertexLayout;
    using namespace Bind;

    using namespace std::string_literals;


    std::vector<std::shared_ptr<Bindable>> bindablePtrs;

    bool hasSpecMap = false;
    bool hasNMap = false;
    bool hasDiffMap = false;
    bool hasAlphaGloss = false;

    float shininess = 2.0f;
    DirectX::XMFLOAT4 specColor = { 0.18f,0.18f,0.18f,1.0f };
    DirectX::XMFLOAT4 diffColor = { 0.45f,0.45f,0.85f,1.0f };

    //const auto base = "Resources\\Models\\nano_textured\\"s;
    const auto base = "Resources\\Models\\Goblin\\"s;

    if (mesh.mMaterialIndex > 0)
    {

        auto& material = *pMaterials[mesh.mMaterialIndex];
        aiString texFileName;


        ///------DIFFUSE_MAP------///

        if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
        {
            bindablePtrs.push_back(Texture::Resolve(gfx, base + texFileName.C_Str()));
            hasDiffMap = true;
        }
        else
        {
            material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffColor));
        }


        ///------SPECULAR_MAP------///

        if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
        {
            auto tex = Texture::Resolve(gfx, base + texFileName.C_Str(), 1);
            hasAlphaGloss = tex->HasAlpha();
            bindablePtrs.push_back(std::move(tex));

            hasSpecMap = true;
        }
        else
        {
            material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(specColor));
        }

        if (!hasAlphaGloss)
        {
            material.Get(AI_MATKEY_SHININESS, shininess);
        }

        ///------NORMAL_MAP------///

        if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
        {
            auto tex = Texture::Resolve(gfx, base + texFileName.C_Str(), 2);
            hasAlphaGloss = tex->HasAlpha();
            bindablePtrs.push_back(std::move(tex));

            hasNMap = true;
        }


        if (hasDiffMap || hasSpecMap || hasNMap)
        {
            bindablePtrs.push_back(Bind::Sampler::Resolve(gfx));
        }
    }

    const auto meshTag = base + "%" + mesh.mName.C_Str();

    const float scale = 6.0f;

    if (hasDiffMap && hasSpecMap && hasNMap)
    {
        DVS::VertexBuffer vbuf(std::move(
            VertexLayout{}
            .Append(VertexLayout::Position3D)
            .Append(VertexLayout::Normal)
            .Append(VertexLayout::Tangent)
            .Append(VertexLayout::Bitangent)
            .Append(VertexLayout::Texture2D)));

        for (unsigned int i = 0; i < mesh.mNumVertices; i++)
        {
            vbuf.EmplaceBack(
                DirectX::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
                *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]),
                *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mTangents[i]),
                *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mBitangents[i]),
                *reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
            );
        }

        std::vector<unsigned short> indices;
        indices.reserve(mesh.mNumFaces * 3);

        for (unsigned int i = 0; i < mesh.mNumFaces; i++)
        {
            const auto& face = mesh.mFaces[i];
            assert(face.mNumIndices == 3);
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }

        bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

        bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

        auto pvs = VertexShader::Resolve(gfx, "PhongTBNVS.cso");
        auto pvsbc = pvs->GetBytecode();
        bindablePtrs.push_back(std::move(pvs));

        bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPS_SpecMap_NMap.cso"));

        bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

       /* struct PSMaterialConstantFullmonte
        {
            BOOL hasNMap = TRUE;
            BOOL hasSpecMap = TRUE;
            BOOL hasGlossMap;
            float specularPower;
            DirectX::XMFLOAT3 specularColor = { 1.0f,1.0f,1.0f }; 
            float specularMapWeight = 1.0f;
        } pmc;*/

        Node::PSMaterialConstantFullmonte pmc;
        pmc.specularPower = shininess;
        pmc.hasGlossMap = hasAlphaGloss ? TRUE : FALSE;

        //bindablePtrs.push_back(PixelConstantBuffer<PSMaterialConstantFullmonte>::Resolve(gfx, pmc, 1u));
        bindablePtrs.push_back(PixelConstantBuffer<Node::PSMaterialConstantFullmonte>::Resolve(gfx, pmc, 1u));
    
    }
    else if (hasDiffMap && hasNMap)
    {
        DVS::VertexBuffer vbuf(std::move(
            VertexLayout{}
            .Append(VertexLayout::Position3D)
            .Append(VertexLayout::Normal)
            .Append(VertexLayout::Tangent)
            .Append(VertexLayout::Bitangent)
            .Append(VertexLayout::Texture2D)
        ));

        for (unsigned int i = 0; i < mesh.mNumVertices; i++)
        {
            vbuf.EmplaceBack(
                DirectX::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
                *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]),
                *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mTangents[i]),
                *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mBitangents[i]),
                *reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
            );
        }

        std::vector<unsigned short> indices;
        indices.reserve(mesh.mNumFaces * 3);
        for (unsigned int i = 0; i < mesh.mNumFaces; i++)
        {
            const auto& face = mesh.mFaces[i];
            assert(face.mNumIndices == 3);
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }

        bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

        bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

        auto pvs = VertexShader::Resolve(gfx, "PhongTBNVS.cso");
        auto pvsbc = pvs->GetBytecode();
        bindablePtrs.push_back(std::move(pvs));

        bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPS_SpecMap_NMap.cso"));

        bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

        struct PSMaterialConstantDiffnorm
        {
            float specularIntensity /*= 0.18f*/;
            float specularPower;
            BOOL  normalMapEnabled = TRUE;
            float padding[1];
        } pmc;
        pmc.specularPower = shininess;
        pmc.specularIntensity = (specColor.x + specColor.y + specColor.z) / 3.0f;

        bindablePtrs.push_back(PixelConstantBuffer<PSMaterialConstantDiffnorm>::Resolve(gfx, pmc, 1u));
    }
    else if (hasDiffMap)
    {
        DVS::VertexBuffer vbuf(std::move(
            VertexLayout{}
            .Append(VertexLayout::Position3D)
            .Append(VertexLayout::Normal)
            .Append(VertexLayout::Texture2D)
        ));

        for (unsigned int i = 0; i < mesh.mNumVertices; i++)
        {
            vbuf.EmplaceBack(
                DirectX::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
                *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]),
                *reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
            );
        }

        std::vector<unsigned short> indices;
        indices.reserve(mesh.mNumFaces * 3);
        for (unsigned int i = 0; i < mesh.mNumFaces; i++)
        {
            const auto& face = mesh.mFaces[i];
            assert(face.mNumIndices == 3);
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }

        bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

        bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

        auto pvs = VertexShader::Resolve(gfx, "PhongVS.cso");
        auto pvsbc = pvs->GetBytecode();
        bindablePtrs.push_back(std::move(pvs));

        bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPS.cso"));

        bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

        struct PSMaterialConstantDiffuse
        {
            float specularIntensity /*= 0.18f*/;
            float specularPower;
            float padding[2];
        } pmc;
        pmc.specularPower = shininess;
        pmc.specularIntensity = (specColor.x + specColor.y + specColor.z) / 3.0f;

        bindablePtrs.push_back(PixelConstantBuffer<PSMaterialConstantDiffuse>::Resolve(gfx, pmc, 1u));
    }
    else if (!hasDiffMap && !hasNMap && !hasSpecMap)
    {
        DVS::VertexBuffer vbuf(std::move(
            VertexLayout{}
            .Append(VertexLayout::Position3D)
            .Append(VertexLayout::Normal)
        ));

        for (unsigned int i = 0; i < mesh.mNumVertices; i++)
        {
            vbuf.EmplaceBack(
                DirectX::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
                *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i])
            );
        }

        std::vector<unsigned short> indices;
        indices.reserve(mesh.mNumFaces * 3);
        for (unsigned int i = 0; i < mesh.mNumFaces; i++)
        {
            const auto& face = mesh.mFaces[i];
            assert(face.mNumIndices == 3);
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }

        bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

        bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

        auto pvs = VertexShader::Resolve(gfx, "PhongVS_NOTEX.cso");
        auto pvsbc = pvs->GetBytecode();
        bindablePtrs.push_back(std::move(pvs));

        bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPS_NOTEX.cso"));

        bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

        Node::PSMaterialConstantNotex pmc;

        pmc.specularPower = shininess;
        pmc.specularColor = specColor;
        pmc.materialColor = diffColor;

        bindablePtrs.push_back(PixelConstantBuffer<Node::PSMaterialConstantNotex>::Resolve(gfx, pmc, 1u));
    }
    else
    {
        throw std::runtime_error("terrible combination of textures in material smh");
    }


    return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));

}


std::unique_ptr<Node> Model::ParseNode(int& nextID, const aiNode& node) noexcept
{
    const auto transform = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(
        reinterpret_cast<const DirectX::XMFLOAT4X4*>(&node.mTransformation)
    ));

    std::vector<Mesh*> curMeshPtrs;
    curMeshPtrs.reserve(node.mNumMeshes);

    for (size_t i = 0; i < node.mNumMeshes; i++)
    {
        const auto meshIndex = node.mMeshes[i];
        curMeshPtrs.push_back(meshPtrs.at(meshIndex).get());
    }

    auto pNode = std::make_unique<Node>(nextID++, node.mName.C_Str(), std::move(curMeshPtrs), transform);

    for (size_t i = 0; i < node.mNumChildren; i++)
    {
        pNode->AddChild(ParseNode(nextID, *node.mChildren[i]));
    }

    return pNode;
}


