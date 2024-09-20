#include "Mesh.h"

#include "imgui/imgui.h"

#include <unordered_map>


#include "Surface.h"


///=======================================================|||
///                                                       |||
///                          MESH                         |||
///                                                       |||
///=======================================================|||

Mesh::Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs)
{
    AddBind(std::make_shared < Bind::Topology >(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    for (auto& p : bindPtrs)
    {
       
        AddBind(std::move(p));
        
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
    void Show(const char* windowName, const Node& root) noexcept
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

    std::unordered_map<int, TransformParameters> transforms;
};


Model::Model(Graphics& gfx, const std::string fileName)
    :
    pWindow(std::make_unique<ModelWindow>())
{
    Assimp::Importer imp;
    const auto pScene = imp.ReadFile(fileName.c_str(),
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices
    );

    for (size_t i = 0; i < pScene->mNumMeshes; i++)
    {
        meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials));
    }

    int nextID = 0;
    pRoot = ParseNode(nextID, *pScene->mRootNode);
}

Model::~Model() noexcept {}

void Model::Draw(Graphics& gfx) const
{
    //const auto transform = DirectX::XMMatrixRotationRollPitchYaw(pos.roll, pos.pitch, pos.yaw) *
    //    DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

    //pRoot->Draw(gfx, transform);

    if (auto node = pWindow->GetSelectedNode())
    {
        node->SetAppliedTransform(pWindow->GetTransform());
    }
    pRoot->Draw(gfx, DirectX::XMMatrixIdentity());


}

void Model::ShowWindow(const char* windowName) noexcept
{
    pWindow->Show(windowName, *pRoot);
}


std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh, 
                                       const aiMaterial* const* pMaterials)
{
    using DVS::VertexLayout;

    DVS::VertexBuffer vbuf(std::move(VertexLayout{}
        .Append(VertexLayout::Position3D)
        .Append(VertexLayout::Normal)
        .Append(VertexLayout::Texture2D) ));

    
    for (unsigned int i = 0; i < mesh.mNumVertices; i++)
    {
        //vbuf.EmplaceBack(* reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mVertices[i]),
        //                 * reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]));

        //DirectX::XMFLOAT3 scaledVertex = {
        //mesh.mVertices[i].x * 0.2f,
        //mesh.mVertices[i].y * 0.2f,
        //mesh.mVertices[i].z * 0.2f
        //};

        vbuf.EmplaceBack(
            *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mVertices[i]),
            *reinterpret_cast<DirectX::XMFLOAT3*>(&mesh.mNormals[i]),
            *reinterpret_cast<DirectX::XMFLOAT2*>(&mesh.mTextureCoords[0][i]));
            
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

    std::vector<std::shared_ptr<Bind::Bindable>> bindablePtrs;

    bool hasSpecMap = false;
    float shininess = 35.0f;

    if (mesh.mMaterialIndex > 0)
    {
        using namespace std::string_literals;
        const auto base = "Resources\\Models\\nano_textured\\"s;

        auto& material = *pMaterials[mesh.mMaterialIndex];
        aiString texFileName;
        material.GetTexture(aiTextureType_DIFFUSE, 0 , &texFileName);
        bindablePtrs.push_back(std::make_shared<Bind::Texture>(gfx, Surface::FromFile(base + texFileName.C_Str())));
        
       if( material.GetTexture( aiTextureType_SPECULAR,0,&texFileName ) == aiReturn_SUCCESS )
       {   
           bindablePtrs.push_back( std::make_shared<Bind::Texture>( gfx,Surface::FromFile( base + texFileName.C_Str() ),1 ) );
           hasSpecMap = true;
       }
       else
       {
           material.Get(AI_MATKEY_SHININESS, shininess);
       }
        
        bindablePtrs.push_back(std::make_shared<Bind::Sampler>(gfx));
    }

    bindablePtrs.push_back(std::make_shared<Bind::VertexBuffer>(gfx, vbuf));

    bindablePtrs.push_back(std::make_shared<Bind::IndexBuffer>(gfx, indices));

    auto pvs = std::make_shared<Bind::VertexShader>(gfx, "PhongVS.cso");
    auto pvsbc = pvs->GetBytecode();
    bindablePtrs.push_back(std::move(pvs));

    //bindablePtrs.push_back(std::make_unique<Bind::PixelShader>(gfx, L"PhongPS.cso"));

    bindablePtrs.push_back(std::make_shared<Bind::InputLayout>(gfx, vbuf.GetLayout().GetD3DLayout(), pvsbc));

    if (hasSpecMap)
    {
        bindablePtrs.push_back(std::make_shared<Bind::PixelShader>(gfx, L"PhongPS_SpecMap.cso"));
    }
    else
    {
        bindablePtrs.push_back(std::make_shared<Bind::PixelShader>(gfx, L"PhongPS.cso"));

        struct PSMaterialConstant
        {
            float specularIntensity = 0.8f;
            float specularPower;
            float padding[2];
        } pmc;

        pmc.specularPower = shininess;

        bindablePtrs.push_back(std::make_shared<Bind::PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc, 1u));
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
