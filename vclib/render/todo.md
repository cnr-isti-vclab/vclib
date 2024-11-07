# VCLib-Render ToDo list

- General
  - [ ] qt headers and sources should be moved into vclib/processing/qt inside this module,
    and should be included only if vclib-processing target is available
  - [x] Manage polygonal meshes by DrawableMesh
  - [x] Fix build failure when the build directory is in the same directory as the source
  - [ ] Add the possibility to check if BGFX is available in the system
    - [ ] Devcontainers should have BGFX installed
  - [ ] Move BGFX from submodule to a CMake dependency
- BGFX
  - [x] Library shaders should be built as headers
  - [ ] Check why OpenGL backend does not work when using BGFX (probably the cause is BGFX itself)
  - [ ] assets and shaders should be compiled in headers that are in paths like
    'vclib/assets/' and 'vclib/shaders/' (right now they are in 'assets/' and 'shaders/')
  - [x] the first window used by the context to create the bgfx backend context should be
    the first window created by the application (right now it is a dummy window)
  - [x] Manage textures in DrawableMesh
  - [ ] Mesh info rendered as text with bgfx
  - [ ] Draw cross in DrawableTrackball
  - [ ] Manage wireframe and points depth in shaders
  - [ ] Better management of canvas and viewer classes - qt and glfw share a lot of code
  - [ ] Implement picking (raytracing to get the triangle clicked by the mouse?)
  - [ ] Customizable background for canvas
  - [ ] check crash example 9015 mesh io stl (crashes if the bimba model (m1) is rendered)
  - [ ] Draw lines trough shader - Work in progress
  - [x] check retina resolution on macOS + GLFW
  - [x] fix trackball initial scale using GLFW
  - [x] fix event handling for the trackball using GLFW (metrics)
  - [x] streamline trackball rendering (avoid error-prone explicit updates)
  - [ ] start integration of ImGUI along GLFW
  - [x] fix trackball bug on Qt implementation (pan after wheel zoom)
  - [ ] fix event mapping for GLFW (it should be slightly different from Qt)