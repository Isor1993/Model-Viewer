# Model Viewer

C++/OpenGL 3D model viewer — assignment 2 of module 5-101. Renders a
textured model with a movable orbit camera, one directional light, a
cubemap skybox and a toon shader.

Design decisions and build log live in the harness repo
(`Projekte/Model_Viewer/`), not here. Built with Visual Studio 2026
(open `ModelViewer.sln`, build x64). All external dependencies ship
inside `Libraries/` — no package manager needed:

- GLFW 3.4 (window and input, precompiled binaries)
- GLAD (OpenGL 4.6 Core function loader, generated source)
- GLM (math, header-only)
- stb_image (image loading, single header)
