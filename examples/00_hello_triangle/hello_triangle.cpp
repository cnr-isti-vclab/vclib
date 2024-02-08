#include "hello_triangle.h"

#include <vclib/ext/bgfx/shader_programs/load_program.h>

HelloTriangle::HelloTriangle() : vcl::qbgf::CanvasWidget()
{
    bgfx::setViewClear(
        0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xff0000ff, 1.0f, 0);

    setupWhatToDraw();
    setupHowToDraw();
}

HelloTriangle::~HelloTriangle()
{
    bgfx::destroy(vbh);
    bgfx::destroy(program);
}

void HelloTriangle::setupWhatToDraw()
{
    bgfx::VertexLayout layout;

    layout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .end();

    vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(positions, sizeof(positions)), layout);
}

void HelloTriangle::setupHowToDraw()
{
    program = vcl::bgf::loadProgram(
        "shaders/vs_vertex_shader", "shaders/fs_fragment_shader");
}

void HelloTriangle::draw()
{
    bgfx::setVertexBuffer(0, vbh);

    bgfx::submit(viewID(), program);
}
