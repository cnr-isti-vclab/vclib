// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <iostream>

#include <vclib/render/canvas.h>

#include <vclib/bgfx/programs/load_program.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/render/drawers/plain_drawer.h>
#include <vclib/render/render_app.h>
#include <vclib/space/core/color.h>

// A simple Vertex struct for our quad
struct Vertex {
    float pos[2];
    uint32_t abgr;
};

// Define 4 vertices for a quad
static const Vertex vertices[] = {
    {{-0.5f, -0.5f}, vcl::Color(vcl::Color::Red).abgr()},
    {{0.5f, -0.5f}, vcl::Color(vcl::Color::Green).abgr()},
    {{-0.5f, 0.5f}, vcl::Color(vcl::Color::Blue).abgr()},
    {{0.5f, 0.5f}, vcl::Color(vcl::Color::Yellow).abgr()},
};

// Define 6 indices for the 2 triangles of the quad
static const uint16_t indices[] = {
    0, 1, 2, // First triangle
    1, 3, 2  // Second triangle
};

/*!
 * \brief A custom drawer that initializes a BGFX vertex buffer and
 * an index buffer to draw a colored quad using custom shaders.
 */
template<typename DerivedRenderApp>
class QuadDrawer : public vcl::PlainDrawer<DerivedRenderApp>
{
public:
    QuadDrawer(vcl::uint width = 1024, vcl::uint height = 768) {}

    ~QuadDrawer()
    {
        if (bgfx::isValid(m_vbh)) bgfx::destroy(m_vbh);
        if (bgfx::isValid(m_ibh)) bgfx::destroy(m_ibh);
        if (bgfx::isValid(m_program)) bgfx::destroy(m_program);
    }

    void onInit(vcl::uint viewId) override
    {
        // Set clear color and depth
        vcl::Color backgroundColor = vcl::Color::Black;
        bgfx::setViewClear(
            viewId,
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            backgroundColor.rgba(),
            1.0f,
            0);

        // Define the vertex layout
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

        // Create Vertex Buffer
        m_vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(vertices, sizeof(vertices)), layout);

        // Create Index Buffer
        m_ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(indices, sizeof(indices)));

        // Load shaders compiled by CMake via vclib_target_add_shaders()
        m_program = vcl::loadProgram(
            "shaders/vs_vertex_shader.sc", "shaders/fs_fragment_shader.sc");

        bgfx::touch(viewId);
    }

    void onResize(vcl::uint width, vcl::uint height) override {}

    void onDrawContent(vcl::uint viewId) override
    {
        // Submit vertex and index buffers for rendering
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        bgfx::submit(viewId, m_program);
    }

    void onDraw(vcl::uint viewId) override { onDrawContent(viewId); }

private:
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    bgfx::ProgramHandle m_program;
};

int main(int argc, char** argv)
{
    std::cout << "=== VCLib Standalone Render Example: Custom Shader ==="
              << std::endl;

    // Define a Window Type using GLFW as Window Manager, a Canvas, and our QuadDrawer
    using WindowGLFW = vcl::RenderApp<vcl::glfw::WindowManager, vcl::Canvas, QuadDrawer>;

    // Create and show the window
    WindowGLFW window("VCLib Custom Shader Quad Example");
    window.show();

    return 0;
}
