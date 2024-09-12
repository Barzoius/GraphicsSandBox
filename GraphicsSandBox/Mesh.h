#pragma once

#include "CRTPDrawable.h"
#include "BindableObjects.h"
#include "VertexSystem.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <optional>


class Mesh : public CRTPDrawable<Mesh>
{
public:
    Mesh(Graphics& gfx, std::vector<std::unique_ptr<Bind::Bindable>> bindPtrs);
    void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;
    DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
    mutable DirectX::XMFLOAT4X4 transform;
};



class Node
{
    friend class Model;
public:
    Node(const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) ;
    void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;
    void ShowTree(int& nodeIndex, std::optional<int>& selectedIndex) const noexcept;

private:
    void AddChild(std::unique_ptr<Node> pChild) noexcept;
private:
    std::string name;

    std::vector<std::unique_ptr<Node>> childPtrs;
    std::vector<Mesh*> meshPtrs;
    DirectX::XMFLOAT4X4 transform;
};



class Model
{
public:
    Model(Graphics& gfx, const std::string fileName);
    void Draw(Graphics& gfx) const;
    void ShowWindow(const char* windowName = nullptr) noexcept;

    ~Model() noexcept;

private:
    static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh);
    std::unique_ptr<Node> ParseNode(const aiNode& node) noexcept;
private:
    std::unique_ptr<Node> pRoot;
    std::vector<std::unique_ptr<Mesh>> meshPtrs;

    std::unique_ptr<class ModelWindow> pWindow;
};