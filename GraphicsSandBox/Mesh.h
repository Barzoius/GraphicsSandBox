#pragma once

#include "Drawable.h"
#include "BindableObjects.h"
#include "VertexSystem.h"



#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <optional>


class ModelException : public ModException
{
public:
    ModelException(int line, const char* file, std::string note) noexcept;
    const char* what() const noexcept override;
    const char* GetType() const noexcept override;
    const std::string& GetNote() const noexcept;
private:
    std::string note;
};

class Mesh : public Drawable
{
public:
    Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs);
    void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
    mutable DirectX::XMFLOAT4X4 transform;
};



class Node
{
    friend class Model;


public:
    struct PSMaterialConstantFullmonte 
    { 
        BOOL  normalMapEnabled = TRUE; 
        BOOL  specularMapEnabled = TRUE; 
        BOOL  hasGlossMap = FALSE; 
        float specularPower = 3.1f; 
        DirectX::XMFLOAT3 specularColor = { 0.75f,0.75f,0.75f };
        float specularMapWeight = 0.671f;
    };

public:
    Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) ;
    void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;

    void SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept;

    int GetId() const noexcept;

    void ShowTree(Node*& pSelectedNode) const noexcept;


    void ControlWND(Graphics& gfx, PSMaterialConstantFullmonte& c);

private:
    void AddChild(std::unique_ptr<Node> pChild) noexcept;
   
private:
    std::string name;
    int id;

    std::vector<std::unique_ptr<Node>> childPtrs;
    std::vector<Mesh*> meshPtrs;

    DirectX::XMFLOAT4X4 transform;
    DirectX::XMFLOAT4X4 appliedTransform;
};



class Model
{
public:
    Model(Graphics& gfx, const std::string fileName);
    void Draw(Graphics& gfx) const;
    void ShowWindow(Graphics& gfx,const char* windowName = nullptr) noexcept;

    ~Model() noexcept;

private:
    static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, 
                                           const aiMaterial* const* pMaterial); // nonconst pointer to a const pointer to a an array of const aiMaterial

    std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node) noexcept;
private:
    std::unique_ptr<Node> pRoot;
    std::vector<std::unique_ptr<Mesh>> meshPtrs;

    std::unique_ptr<class ModelWindow> pWindow;
};


