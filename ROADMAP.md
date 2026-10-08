# BGE → AAA Open-World Engine Roadmap
**Target:** Solo/duo dev, PC-only (Win/macOS/Linux), bgfx renderer, learning-focused, never "finished"
**Scope:** Seamless open-world (GTA/RDR style) — streaming, LOD, world partition, AI, physics, gameplay

---

## Phase 0: Foundation Hardening (Months 1-3)
*Goal: Solid, debuggable base before adding complex systems*

### Core Infrastructure
- [ ] **Logging/Profiling**: Structured logging (spdlog), Tracy/Remotery integration, frame/zone profiling
- [ ] **Memory System**: Arena allocators, stack allocator, memory tracking, leak detection, OOM handling
- [ ] **Asset Pipeline**: Hot-reloadable assets, binary formats (glTF for meshes, KTX2 for textures), asset registry, dependency tracking
- [ ] **Config/Serialization**: TOML/JSON for engine config, binary for runtime data, versioning
- [ ] **Job System**: Thread pool, task graph, fiber/coroutine support (for async loading)
- [ ] **ECS Foundation**: Archetype-based ECS (flecs/EnTT or custom), component registration, query caching

### Rendering (bgfx)
- [ ] **Render Graph**: Frame graph with resource aliasing, transient buffers, async compute
- [ ] **Shader System**: Hot-reload, permutation management, reflection, bindless resources
- [ ] **Debug Rendering**: Lines, shapes, text, gizmos — essential for all later debugging

### Platform
- [ ] **Input System**: Action mapping, gamepad/keyboard/mouse, raw input, vibration
- [ ] **Window/OS**: Fullscreen/exclusive/borderless, HDR, multi-monitor, high-DPI

**Milestone:** Clean frame with profiling, hot-reload shaders/assets, memory stats visible, ECS running 100k entities at 60fps

---

## Phase 1: World & Streaming Core (Months 4-8)
*Goal: Infinite seamless world — the hardest technical challenge*

### World Architecture
- [ ] **World Partition**: Quadtree/octree spatial index, chunk-based streaming, LOD hierarchy
- [ ] **Streaming System**: Priority-based async loading (distance, view frustum, velocity prediction), background job queues, GPU upload streaming
- [ ] **Level of Detail**: Hierarchical LOD (HLOD), impostors, mesh simplification pipeline, texture mip streaming
- [ ] **Origin Rebasing**: Floating-origin for large worlds (double-precision physics, camera-relative rendering)

### Terrain
- [ ] **Heightfield System**: Tiled terrain, splat mapping, virtual texturing for detail maps
- [ ] **Procedural Assist**: Noise-based base terrain, erosion simulation, biome blending
- [ ] **Runtime Editing**: Terrain sculpting, painting, hole punching (caves)

### Vegetation / Instancing
- [ ] **Instanced Rendering**: GPU-driven culling (compute shader frustum/occlusion), LOD crossfade
- [ ] **Procedural Placement**: Poisson disk, biome rules, slope/altitude masks, runtime density scaling
- [ ] **Wind/Animation**: Vertex shader wind, compute-based global wind field

**Milestone:** Fly through 16km²+ world at 60fps, seamless streaming, no loading screens, 1M+ instances

---

## Phase 2: Rendering Pipeline (Months 9-14)
*Goal: Modern PBR pipeline matching 2018-2020 AAA quality*

### Lighting
- [ ] **Deferred/Forward+**: Clustered/light-grid culling, 1000+ dynamic lights
- [ ] **GI**: DDGI / Probe Volumes for dynamic GI, lightmap baking pipeline for static
- [ ] **Shadows**: Cascaded shadow maps (CSM), contact hardening, virtual shadow maps (if bgfx allows)
- [ ] **Reflections**: Screen-space (SSR), planar probes, cubemap array for local reflections

### Materials & Shading
- [ ] **PBR Material System**: Disney BRDF, clearcoat, sheen, anisotropy, subsurface
- [ ] **Material Graph**: Node-based authoring (or compile from glTF PBR), permutation compiler
- [ ] **Layered Materials**: Blend layers (mud, snow, wear), decal projection

### Atmosphere & Volumetrics
- [ ] **Sky/Atmosphere**: Nishita/Hosek-Wilkie, aerial perspective, time-of-day cycle
- [ ] **Clouds**: Volumetric cloud layer (ray-marched), weather transitions
- [ ] **Volumetric Fog/Light Shafts**: Froxel-based, temporal reprojection

### Post-Process
- [ ] **Temporal AA (TAA)**: History rectification, velocity buffer, ghosting reduction
- [ ] **Bloom, DOF, Motion Blur, Lens Effects**: Chromatic aberration, vignette, film grain
- [ ] **Color Grading**: ACES tonemap, LUT support, HDR output (PQ/HLG)

**Milestone:** Photorealistic static scene at 4K/60 on mid-range GPU, all lighting dynamic

---

## Phase 3: Physics & Simulation (Months 15-20)
*Goal: Believable interaction at scale*

### Rigid Body (Jolt Physics / PhysX / custom)
- [ ] **Integration**: Broad/narrow phase, CCD, sleeping, layers/masks
- [ ] **Vehicle Physics**: Raycast/suspension, tire friction (Pacejka), differential, transmission
- [ ] **Character Controller**: Kinematic capsule, slope/step handling, ledge detection, networking-ready

### Soft Bodies / Cloth / Hair (optional, later)
- [ ] **XPBD / PBD**: GPU compute for cloth, ropes, hair

### Destruction / Fracture (optional)
- [ ] **Pre-fractured chunks**, stress-based breaking, debris cleanup

### Audio (miniaudio / SoLoud / custom)
- [ ] **Spatial Audio**: HRTF, occlusion, reverb zones, Doppler
- [ ] **Music System**: Layered adaptive music, crossfading, stingers

**Milestone:** Drive/fly/walk through world with physical interaction, 60fps with 500+ active bodies

---

## Phase 4: AI & Gameplay Systems (Months 21-30)
*Goal: Living world with emergent behavior*

### Navigation
- [ ] **Navmesh**: Recast/Detour, tiled streaming, dynamic obstacles, off-mesh links
- [ ] **Crowd Simulation**: Local avoidance, formation, density-based steering

### Behavior
- [ ] **Behavior Trees**: Visual editor, hot-reload, debugging, per-agent blackboards
- [ ] **HTN/GOAP** (optional): Planners for complex NPCs
- [ ] **Utility AI**: Scoring-based for ambient life

### Gameplay Framework
- [ ] **Entity/Component/Gameplay Tags**: Gameplay ability system (GAS-style), attributes, effects
- [ ] **Quest/Dialogue System**: Graph-based, conditionals, localization-ready
- [ ] **Save/Load**: World state serialization, deterministic replay, version migration

### Multiplayer Foundation (if ever needed)
- [ ] **Netcode**: Snapshot interpolation, client-side prediction, lag compensation (GGPO/ENet)

**Milestone:** 100+ NPCs with daily routines, traffic, ambient life, basic missions playable

---

## Phase 5: Content Pipeline & Tools (Months 31-40)
*Goal: Actually build the world — tools make or break open-world*

### Editor (ImGui-based or separate)
- [ ] **World Editor**: Entity placement, terrain painting, spline roads/rivers, sector streaming preview
- [ ] **Material/Shader Editor**: Node graph, live preview
- [ ] **Animation Graph**: State machine, blend trees, IK preview
- [ ] **Sequence/Cinematic Editor**: Timeline, tracks, camera cuts

### Content Pipeline
- [ ] **CLI Tools**: Mesh processing (meshoptimizer), texture compression (basisu), navmesh build, HLOD generation
- [ ] **World Build Pipeline**: Sector export, streaming manifest, LOD generation, lightmap bake (GPU lightmapper)
- [ ] **Asset Validation**: Naming conventions, budget checks (tris, tex mem, draw calls)

### Procedural Assistance
- [ ] **Road/River Network**: Graph-based, auto-splines, intersection resolution
- [ ] **Building/Prop Placement**: Rule-based (Wave Function Collapse / constraint solver)
- [ ] **Biome/Climate System**: Data-driven, procedural variation

**Milestone:** One person can build a 1km² playable sector in a day

---

## Phase 6: Polish & Scale (Months 41+)
*Never ending — the "AAA" bar keeps moving*

- [ ] **Performance**: GPU-driven rendering, mesh shaders (if API allows), bindless, Nanite-style virtual geometry
- [ ] **Animation**: Motion matching, learned motion controllers, full-body IK
- [ ] **Visuals**: Ray-traced reflections/GI (VK_KHR_ray_tracing / DXR), path-traced cinematics
- [ ] **Accessibility**: Full remapping, colorblind, screen reader, difficulty options
- [ ] **Localization**: ICU, RTL, font fallback, audio dubbing pipeline
- [ ] **Telemetry**: Crash reporting, performance metrics, heatmaps, funnel analysis

---

## Recommended Learning Order (for solo dev)

| Priority | Topic | Resources |
|----------|-------|-----------|
| 1 | **C++ Modern (17/20/23)**, templates, concurrency | CppReference, "C++ Templates" (Vandevoorde) |
| 2 | **Graphics API mental model** (Vulkan/DX12) | "Vulkan Tutorial", "GPU Gems", bgfx source |
| 3 | **Linear Algebra / 3D Math** | "Mathematics for 3D Game Programming" (Dunn) |
| 4 | **ECS Architecture** | flecs/EnTT source, "Entity Component Systems" (Arvidsson) |
| 5 | **Concurrent/Parallel Programming** | "Concurrency in Action" (Williams), folly/moodycamel |
| 6 | **Graphics Techniques** | GPU Pro / GPU Zen series, SIGGRAPH courses |
| 7 | **Physics** | Jolt Physics source, "Game Physics Engine Development" (Millington) |
| 8 | **AI/Navigation** | Recast/Detour, Behavior Tree libs, "AI for Games" (Millington) |
| 9 | **Asset Formats** | glTF spec, KTX2, USD (for pipeline) |
| 10 | **Engine Architecture** | "Game Engine Architecture" (Gregory), Unreal/Godot source |

---

## Minimal Viable "Open World" Demo Checklist
*Build this first — proves the hard tech works before investing years*

- [ ] 4x4km terrain with streaming chunks
- [ ] 100k instanced trees/grass with GPU culling
- [ ] Vehicle physics driving across chunk boundaries
- [ ] Origin rebasing working (no jitter at 10km from origin)
- [ ] Day/night cycle with dynamic shadows
- [ ] Save/load world state (position, time, entities)
- [ ] 60fps on 5-year-old GPU (GTX 1060 / RX 580 class)

---

## Reality Checks

| Risk | Mitigation |
|------|------------|
| **Scope creep** | Hard gate: no new systems until current phase milestone ships |
| **Burnout** | 1 day/week "play" — build a fun toy in the engine, not just tech |
| **Graphics rabbit hole** | Use placeholder art (Kenney, Quixel free) until pipeline works |
| **bgfx limitations** | Accept: no mesh shaders, no RT, limited compute. Plan render backend swap at Phase 5 |
| **Content creation** | Invest in procedural tools early — you cannot hand-place a GTA-sized world solo |

---

## Suggested First 3 Months (Concrete Tasks)

| Week | Focus | Deliverable |
|------|-------|-------------|
| 1-2 | Build system polish | Makefile CI (GitHub Actions), ccache, faster incremental builds |
| 3-4 | Memory + Logging + Profiling | Arena allocator, Tracy integration, memory overlay |
| 5-6 | ECS + Job System | 100k entities moving at 60fps, parallel for-each |
| 7-8 | Asset Pipeline + Hot Reload | glTF loader, texture streaming, shader hot-reload |
| 9-10 | Render Graph + Debug Draw | Frame graph with 3 passes, debug primitives |
| 11-12 | World Partition Prototype | Quadtree streaming, origin rebasing, LOD switching |

---

## Key Architectural Decisions to Make Early

1. **ECS vs OOP** — Archetype ECS (flecs) scales better for open-world
2. **Coordinate System** — Double-precision world, float-relative rendering
3. **Threading Model** — Job system + main thread render submit, or render thread?
4. **Scripting** — Lua (sol2), C# (Embedded Mono/.NET), or none (C++ only)?
5. **Serialization** — Reflection-based (RTTR/meta) or manual (FlatBuffers/Cap'n Proto)?
6. **Editor** — ImGui in-engine vs separate Qt/Dear ImGui native app?

---

*Update this roadmap every 3 months. Delete phases that don't serve the current goal. The best engine is the one that ships a game.*
