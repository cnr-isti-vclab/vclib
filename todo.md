# VCLib-Render ToDo list

 - [ ] Check why OpenGL backend does not work
 - [ ] The first created window should be the one passed to bgfx init (right now, the context constructor creates always a fake one);
 - [ ] Make bgfx mandatory with the module - remove everything that requires only bgfx from ext
 - [ ] Manage textures in DrawableMesh
 - [ ] Draw cross in DrawableTrackball
 - [ ] Better management of canvas and viewer classes - qt and glfw share a lot of code
 - [ ] Library shaders should be built as headers
 - [ ] Try to make a small Qt-Wayland example work
 - [ ] Qt console logger for MainWindow
 - [ ] Mesh info rendered as text with bgfx
 - [ ] Include ImGui??
 - [ ] Draw lines trough shader (https://github.com/mhalber/Lines)
 - [ ] Manage polygonal meshes by DrawableMesh
 - [ ] Manage wireframe and points depth in shaders
 - [ ] Implement raytracing to get the triangle clicked by the mouse
 - [ ] Customizable background for canvas
