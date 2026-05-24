# 3D Modeling Software Engine

**CSE382 Computer Graphics — Spring 2025/2026**
**Project 2: Advanced Surface and Automotive/Industrial Modeling System**

---

## Overview

A real-time, CPU-rasterized 3D modeling engine built from scratch in C++23, targeting the workflow of modern industrial surface design and parametric modeling tools. The engine implements core computer graphics algorithms manually — rasterization primitives, parametric curves and surfaces, mesh data structures, and subdivision — without relying on any hardware graphics API for geometry processing. DirectX 11 is used exclusively as a presentation layer to blit the software-rendered framebuffer to the screen.

The architecture is cleanly separated into a **platform layer** (Win32), a **presentation layer** (DirectX 11), a **software renderer** (CPU framebuffer), a **scene graph** (hierarchical transforms), and a **geometric modeling layer** (curves, surfaces, meshes).

---

## Screenshots

| Initial Canvas Demo | Wireframe Cube (Front) | Wireframe Cube (Orbiting) | Wireframe Cube (Zoomed Out) |
|---|---|---|---|
| ![Canvas](docs/imgs/2026-5-21/1.png) | ![Front](docs/imgs/2026-05-24/1.png) | ![Orbit](docs/imgs/2026-05-24/2.png) | ![Zoom](docs/imgs/2026-05-24/3.png) |

---

## Tech Stack

| Layer | Technology |
|---|---|
| Language | C++23 |
| Build System | CMake 3.25+ with Ninja |
| Platform | Win32 API |
| Presentation | DirectX 11 (framebuffer blit only) |
| GUI | Dear ImGui (DX11 + Win32 backends) |
| Rendering | Custom CPU Software Rasterizer |
| IDE | Visual Studio 2022+ |

---

## Architecture

```
Application
├── Platform Layer
│   ├── Window          — Win32 window, message pump, live resize
│   └── InputHandler    — Keyboard and mouse state tracking
├── Presentation Layer
│   └── DX11Presenter   — Uploads CPU framebuffer to GPU via dynamic texture
├── Renderer
│   ├── Renderer        — PutPixel framebuffer, dual-buffer (canvas + viewport)
│   └── WireframeRasterizer — Projects 3D half-edges to 2D screen lines
├── Draw Primitives (src/Draw/)
│   ├── Line            — Bresenham, Fractional Accumulation, DDA
│   ├── Circle          — Bresenham, Midpoint, Polar, Cartesian
│   └── Ellipse         — Midpoint, Fractional Accumulation, Polar, Cartesian
├── Scene
│   └── SceneGraph      — Hierarchical node tree, SRT transforms, recursive render
├── Camera
│   └── Camera          — Spherical orbit, linear pan, scroll zoom, perspective projection
└── EditorUI
    └── EditorUI        — ImGui wrapper for all editor panels
```

### Key Design Decisions

**Dual-buffer renderer:** The `Renderer` maintains a `m_Canvas` (backing store, only grows) and a `m_Framebuffer` (viewport-sized slice sent to DX11 each frame). This design anticipates future pan/zoom: the viewport can slide over the canvas without reallocation.

**Half-Edge (DCEL) mesh:** The `Mesh` struct uses a Doubly-Connected Edge List. Each `HalfEdge` stores start/end vertices, a twin pointer to its opposite-direction counterpart, next/prev pointers for face traversal, and an incident face pointer. This structure gives O(1) access to all topological adjacency queries needed by subdivision and triangulation algorithms.

**SceneGraph with SRT transforms:** Each `Node` stores Position, Rotation (Euler), and Scale separately. The `Update` traversal computes `LocalTransform = Scale * Rotation * Translation` and accumulates `GlobalTransform = LocalTransform * ParentGlobal` top-down, enabling hierarchical object transformations.

**Camera (Spherical Coordinates):** The camera is defined by `(target, radius, pitch, yaw)`. Left-drag + Shift orbits, Left-drag alone pans (translates the target in view space), and scroll wheel zooms. All recalculate the view and projection matrices only when dirty.

---

## Implemented Features

### Rasterization Primitives

**Line Drawing** (`src/Draw/Line.cpp`)

| Algorithm | Description |
|---|---|
| Bresenham | Integer-only incremental decision variable; fastest |
| Fractional Accumulation | Floating-point slope accumulator; intermediate |
| DDA (Digital Differential Analyzer) | Float increments + round; slowest but simplest |

**Circle Drawing** (`src/Draw/Circle.cpp`)

| Algorithm | Description |
|---|---|
| Bresenham | Integer decision variable `d = 3 - 2r`; fastest |
| Midpoint | Equivalent decision variable, slightly different update form |
| Polar | Parametric `(r·cosθ, r·sinθ)` over first octant |
| Cartesian | Direct `y = √(r²−x²)` computation |

All circle algorithms exploit 8-fold symmetry to plot one octant and mirror across all eight.

**Ellipse Drawing** (`src/Draw/Ellipse.cpp`)

| Algorithm | Description |
|---|---|
| Midpoint | Two-region Bresenham-style; transitions at `px == py` |
| Fractional Accumulation | Slope-based accumulator; handles both regions |
| Polar | Parametric `(rx·cosθ, ry·sinθ)` over first quadrant |
| Cartesian | Direct formula with 4-fold symmetry |

### Mesh Data Structure

The **Half-Edge / DCEL** (`src/Renderer/Mesh.h`) is the core mesh representation:

- `Vertex` — position `(x, y, z)` and one incident half-edge
- `HalfEdge` — start/end vertices, twin, next, prev, incident face
- `Face` — one outer component half-edge

This topology enables efficient iteration over vertex one-rings, face edges, and boundary loops — all prerequisites for the subdivision and triangulation algorithms to be implemented.

### 3D Pipeline

- **Perspective projection** via `XMMatrixPerspectiveFovLH` (45° FoV, 0.1–1000 far plane)
- **Model-View-Projection** composed per node from its `GlobalTransform`
- **Clip-space culling** in `WireframeRasterizer::ProjectToScreen` — edges with any endpoint outside NDC `[-1,1]³` or behind the near plane are discarded
- **Half-edge deduplication** — the rasterizer skips a half-edge if `edge.get() >= edge->twin`, ensuring each geometric edge is drawn exactly once
- **Hierarchical transforms** via the SceneGraph `Update` traversal

### Camera System

- **Orbit** (Shift + Left-drag): spherical coordinate yaw/pitch with ±85° pitch clamp to prevent gimbal lock
- **Pan** (Left-drag): translates target in camera-space right/up, with speed scaled by `radius * 0.0015f` for intuitive feel at any zoom level
- **Zoom** (scroll wheel): exponential feel via `zoomSpeed = radius * 0.1f`, clamped to a minimum radius of 0.2

---

## Roadmap (Project 2 Requirements)

### Curves and Surfaces
- [ ] Bernstein basis polynomials
- [ ] Bézier curves (De Casteljau algorithm + matrix form)
- [ ] Bézier surface generation and rasterization
- [ ] Uniform B-Splines
- [ ] Non-uniform B-Splines
- [ ] De Boor algorithm
- [ ] NURBS curves and surfaces
- [ ] Spline surface rasterization

### Mesh and Modeling
- [x] Half-edge (DCEL) mesh data structure
- [ ] Polygon mesh construction and interactive editing
- [ ] Polygon triangulation (convex and concave)
- [ ] Catmull-Clark subdivision
- [ ] Loop subdivision
- [ ] Basic CSG (Constructive Solid Geometry) operations
- [ ] Volumetric modeling demonstrations
- [ ] Basic sculpting / mesh deformation tools

### Transformations and Scene
- [x] Hierarchical scene graph with SRT transforms
- [x] 3D perspective camera (orbit, pan, zoom)
- [ ] Interactive control point editing
- [ ] Mesh import / export

### Engine and UI
- [x] CPU software rasterizer with dual-buffer design
- [x] DirectX 11 presentation layer
- [x] Live window resize with canvas preservation
- [x] Dear ImGui editor UI integration
- [ ] Wireframe / shaded visualization toggle
- [ ] Algorithm comparison visualization panel

---

## Building

### Prerequisites

- Windows 10 or 11
- Visual Studio 2022 or later (Desktop Development with C++ workload)
- CMake 3.25+

### CMake Presets (Recommended)

```bash
# Configure
cmake --preset x64-debug

# Build
cmake --build --preset x64-debug

# Run
./out/build/x64-debug/3DModelingSoftware.exe
```

### Available Presets

| Preset | Platform | Config |
|---|---|---|
| `x64-debug` | x64 | Debug |
| `x64-release` | x64 | Release |
| `x86-debug` | x86 | Debug |
| `x86-release` | x86 | Release |

---

## Controls

| Input | Action |
|---|---|
| Left Mouse Drag | Pan camera (translate target) |
| Shift + Left Mouse Drag | Orbit camera (rotate around target) |
| Scroll Wheel | Zoom in / out |
| Escape | Exit application |

---

## Project Structure

```
3DModelingSoftware/
├── src/
│   ├── Main.cpp
│   ├── Application/        — App loop, event handling, resize
│   ├── Platform/           — Win32 window, input handler
│   ├── Presenter/          — DirectX 11 framebuffer presenter
│   ├── Renderer/           — CPU rasterizer, mesh, wireframe rasterizer
│   ├── Draw/               — Line, circle, ellipse primitives
│   ├── Scene/              — Scene graph and node hierarchy
│   ├── Camera/             — Spherical orbit camera
│   └── EditorUI/           — Dear ImGui wrapper
├── ext/
│   └── imgui/              — Dear ImGui source (submodule)
├── docs/
│   └── imgs/               — Screenshots
├── CMakeLists.txt
├── CMakePresets.json
└── README.md
```

---

## Documentation

Generate Doxygen documentation:

```bash
doxygen Doxyfile
```

Output will be in `docs/doxygen/`.

---

## License

Licensed under the **MIT License** — see [LICENSE](LICENSE) for details.

---

## Authors

- **Ahmed Yasser**
- **Saif Amer**

*CSE382 Computer Graphics — Spring 2025/2026*