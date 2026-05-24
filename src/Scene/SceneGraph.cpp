#include "SceneGraph.h"
#include "Renderer/Mesh.h"
#include "Renderer/Renderer.h"
#include "Renderer/WireframeRasterizer.h"

Node::Node() noexcept  = default;
Node::~Node() noexcept = default;

SceneGraph::SceneGraph() noexcept  = default;
SceneGraph::~SceneGraph() noexcept = default;

void SceneGraph::Initialize(Renderer& renderer)
{
    m_Renderer = &renderer;
    DirectX::XMStoreFloat4x4(&m_ViewProjMatrix, DirectX::XMMatrixIdentity());

    m_Root       = std::make_unique<Node>();
    m_Root->Name = "Root";
}

void SceneGraph::Update(Node* node, const DirectX::XMFLOAT4X4& parentGlobal)
{
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(node->Position.x, node->Position.y, node->Position.z);
    DirectX::XMMATRIX rotation =
        DirectX::XMMatrixRotationRollPitchYaw(node->Rotation.x, node->Rotation.y, node->Rotation.z);
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(node->Scale.x, node->Scale.y, node->Scale.z);

    DirectX::XMMATRIX localMatrix = scaling * rotation * translation;
    DirectX::XMStoreFloat4x4(&node->LocalTransform, localMatrix);

    DirectX::XMMATRIX parentMatrix = DirectX::XMLoadFloat4x4(&parentGlobal);
    DirectX::XMMATRIX globalMatrix = localMatrix * parentMatrix;
    DirectX::XMStoreFloat4x4(&node->GlobalTransform, globalMatrix);

    for (const auto& child : node->Children)
    {
        Update(child.get(), node->GlobalTransform);
    }
}

void SceneGraph::Render()
{
    if (m_Root)
    {
        RenderNode(m_Root.get());
    }
}

void SceneGraph::RenderNode(Node* node)
{
    if (node->Mesh)
    {
        WireframeRasterizer::Draw(*m_Renderer, *node->Mesh, node->GlobalTransform, m_ViewProjMatrix);
    }

    for (const auto& child : node->Children)
    {
        RenderNode(child.get());
    }
}
