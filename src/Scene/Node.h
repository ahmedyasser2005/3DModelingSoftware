#pragma once
#include "Renderer/Mesh.h"
#include <DirectXMath.h>
#include <memory>
#include <string>
#include <vector>

struct Node
{
    Node() noexcept;
    ~Node() noexcept;

    std::string Name = "Node";
    std::unique_ptr<Mesh> Mesh;

    DirectX::XMFLOAT3 Position{ 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 Rotation{ 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 Scale{ 1.0f, 1.0f, 1.0f };

    DirectX::XMFLOAT4X4 LocalTransform = {};
    DirectX::XMFLOAT4X4 GlobalTransform = {};

    std::vector<std::unique_ptr<Node>> Children;
};