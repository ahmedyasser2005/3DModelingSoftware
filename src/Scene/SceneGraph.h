#pragma once

#include "Node.h"
#include <DirectXMath.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Renderer;
struct Mesh;

class SceneGraph final
{
  public:
    SceneGraph() noexcept;
    ~SceneGraph() noexcept;
    SceneGraph(const SceneGraph&) = delete;
    SceneGraph& operator=(const SceneGraph&) = delete;
    SceneGraph(SceneGraph&&) = delete;
    SceneGraph& operator=(SceneGraph&&) = delete;

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

    std::unique_ptr<Node> m_Root = nullptr;
    Renderer* m_Renderer = nullptr;
    DirectX::XMFLOAT4X4 m_ViewProjMatrix = {};
};