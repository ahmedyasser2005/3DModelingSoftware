#pragma once
#include <memory>
#include <vector>

// Doubly-Connected Edge List (DCEL), AKA Half-edge,
// It uses "half-edges" that point in opposite directions to allow easy traversal of a mesh's topology.

struct Vertex;
struct HalfEdge;
struct Face;

struct Vertex
{
    float x, y, z;          // Geometric coordinates
    HalfEdge* incidentEdge; // One of the half-edges leaving this vertex
};

struct Face
{
    HalfEdge* outerComponent; // One of the half-edges forming the boundary of this face
};

struct HalfEdge
{
    Vertex* origin;     // Vertex where this half-edge starts
    HalfEdge* twin;     // The half-edge running in the opposite direction
    Face* incidentFace; // The face to the left of this half-edge
    HalfEdge* next;     // The next half-edge around the incident face (counter-clockwise)
    HalfEdge* prev;     // The previous half-edge around the incident face
};

struct Mesh
{
    std::vector<std::unique_ptr<Vertex>> vertices;
    std::vector<std::unique_ptr<HalfEdge>> halfEdges;
    std::vector<std::unique_ptr<Face>> faces;
};