# VCLib-Render ToDo list

- BGFX
  - [ ] assets and shaders should be compiled in headers that are in paths like
    'vclib/assets/' and 'vclib/shaders/' (right now they are in 'assets/' and 'shaders/')
  - [ ] the first window used by the context to create the bgfx backend context should be
    the first window created by the application (right now it is a dummy window)