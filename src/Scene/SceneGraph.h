#pragma once
#include <DirectXMath.h>
#include <memory>
#include <string>
#include <vector>

class Renderer;
struct Mesh;

struct Node
{
    Node() noexcept;
    ~Node() noexcept;
    Node(const Node&)            = default;
    Node& operator=(const Node&) = default;
    Node(Node&&)                 = default;
    Node& operator=(Node&&)      = default;

    DirectX::XMFLOAT4X4 LocalTransform  = {};
    DirectX::XMFLOAT4X4 GlobalTransform = {};

    std::vector<std::unique_ptr<Node>> Children = {};
    std::unique_ptr<Mesh>              Mesh     = {};

    std::string Name = "Untitled";

    DirectX::XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 Rotation = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 Scale    = { 1.0f, 1.0f, 1.0f };
};

class SceneGraph final
{
  public:
    SceneGraph() noexcept;
    ~SceneGraph() noexcept;
    SceneGraph(const SceneGraph&)            = delete;
    SceneGraph& operator=(const SceneGraph&) = delete;
    SceneGraph(SceneGraph&&)                 = delete;
    SceneGraph& operator=(SceneGraph&&)      = delete;

    void Initialize(Renderer& renderer);

    void Update(Node* node, const DirectX::XMFLOAT4X4& parentGlobal);
    void Render();

    void SetViewProjMatrix(const DirectX::XMFLOAT4X4& viewProj)
    {
        m_ViewProjMatrix = viewProj;
    }
    void SetRoot(std::unique_ptr<Node> root)
    {
        m_Root = std::move(root);
    }
    [[nodiscard]] Node* GetRoot() const noexcept
    {
        return m_Root.get();
    }

  private:
    void RenderNode(Node* node);

    std::unique_ptr<Node> m_Root           = nullptr;
    Renderer*             m_Renderer       = nullptr;
    DirectX::XMFLOAT4X4   m_ViewProjMatrix = {};
};
