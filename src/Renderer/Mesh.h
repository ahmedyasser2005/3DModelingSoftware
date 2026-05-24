#pragma once
#include <memory>
#include <vector>

// Doubly-Connected Edge List (DCEL), AKA Half-edge,
// It uses "half-edges" that point in opposite directions to allow easy traversal of a mesh's topology.

struct HalfEdge;
struct Face;

struct Vertex
{
    HalfEdge* incidentEdge = nullptr;
    float     x = 0.0f, y = 0.0f, z = 0.0f;
};

struct Face
{
    HalfEdge* outerComponent = nullptr;
};

struct HalfEdge
{
    Vertex*   start        = nullptr;
    Vertex*   end          = nullptr;
    HalfEdge* twin         = nullptr;
    Face*     incidentFace = nullptr;
    HalfEdge* next         = nullptr;
    HalfEdge* prev         = nullptr;
};

struct Mesh
{
    std::vector<std::unique_ptr<Vertex>>   vertices  = {};
    std::vector<std::unique_ptr<HalfEdge>> halfEdges = {};
    std::vector<std::unique_ptr<Face>>     faces     = {};
};
