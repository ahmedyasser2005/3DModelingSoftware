#include "Mesh.h"
#include <memory>

// NOTE: This is AI generated test function. Will be deleted soon.
std::unique_ptr<Mesh> CreatePrimitiveCube(float size)
{
    auto  mesh = std::make_unique<Mesh>();
    float h    = size * 0.5f;

    // 8 Vertices of a cube
    DirectX::XMFLOAT3 rawVertices[] = { { -h, -h, -h }, { h, -h, -h }, { h, h, -h }, { -h, h, -h },
                                        { -h, -h, h },  { h, -h, h },  { h, h, h },  { -h, h, h } };

    for (const auto& v : rawVertices)
    {
        auto vertex = std::make_unique<Vertex>();
        vertex->x   = v.x;
        vertex->y   = v.y;
        vertex->z   = v.z;
        mesh->vertices.push_back(std::move(vertex));
    }

    // Helper to generate paired directed half-edges cleanly
    auto addEdgePair = [&](size_t startIdx, size_t endIdx)
    {
        auto e1 = std::make_unique<HalfEdge>();
        auto e2 = std::make_unique<HalfEdge>();

        e1->start = mesh->vertices[startIdx].get();
        e1->end   = mesh->vertices[endIdx].get();

        e2->start = mesh->vertices[endIdx].get();
        e2->end   = mesh->vertices[startIdx].get();

        e1->twin = e2.get();
        e2->twin = e1.get();

        // Assign incident links back to vertices if uninitialized
        if (!mesh->vertices[startIdx]->incidentEdge)
            mesh->vertices[startIdx]->incidentEdge = e1.get();
        if (!mesh->vertices[endIdx]->incidentEdge)
            mesh->vertices[endIdx]->incidentEdge = e2.get();

        mesh->halfEdges.push_back(std::move(e1));
        mesh->halfEdges.push_back(std::move(e2));
    };

    // Define the 12 structural structural edges of a standard cube
    addEdgePair(0, 1);
    addEdgePair(1, 2);
    addEdgePair(2, 3);
    addEdgePair(3, 0); // Front Face
    addEdgePair(4, 5);
    addEdgePair(5, 6);
    addEdgePair(6, 7);
    addEdgePair(7, 4); // Back Face
    addEdgePair(0, 4);
    addEdgePair(1, 5);
    addEdgePair(2, 6);
    addEdgePair(3, 7); // Connecting Pillars

    return mesh;
}