The source code of this directory is directly taken from the [bgfx repository](https://github.com/bkaradzic/bgfx), and it is licensed under the BSD 2-Clause License. 

It has been modified with the following goals:
- Remove the dependency on the bgfx example-common library, that is not needed for the purpose of this project and it depends on ImGui.
- Insert the original classes and functions under bgfx namespace, to avoid conflicts with other libraries.
- Use the vclib-render stb-TrueType library instead of the one used by bgfx
