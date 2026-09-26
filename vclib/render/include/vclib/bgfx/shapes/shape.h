// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SHAPES_SHAPE_H
#define VCL_BGFX_SHAPES_SHAPE_H

#include "uniforms/shape_uniforms.h"

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers.h>
#include <vclib/bgfx/shapes/uniforms/shape_uniforms.h>

#include <vclib/algorithms/mesh.h>
#include <vclib/meshes.h>
#include <vclib/space/core.h>

#include <bgfx/bgfx.h>

namespace vcl {

/**
 * @brief Base class for rendering simple 3D shapes.
 *
 * The Shape class manages the GPU buffers (via MeshRenderBuffers) for a
 * given triangle mesh and provides convenient methods to draw it with a
 * flat color or a picking ID.
 *
 * It is designed to be lightweight and used inside other drawables or gizmos.
 * Shapes are movable but not copyable to prevent accidental duplication
 * of GPU resources.
 */
class Shape
{
    MeshRenderBuffers<vcl::TriMesh> mBuffers;

public:
    static const uint64_t DEFAULT_DRAW_STATE =
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
        BGFX_STATE_DEPTH_TEST_LEQUAL | BGFX_STATE_MSAA;

    Shape() = default;

    /**
     * @brief Constructs a Shape from a given TriMesh.
     */
    Shape(vcl::TriMesh mesh)
    {
        using MRI = MeshRenderInfo;

        vcl::updatePerVertexNormals(mesh);

        MRI::BuffersBitSet btf = {
            MRI::Buffers::VERTICES,
            MRI::Buffers::VERT_NORMALS,
            MRI::Buffers::TRIANGLES};

        mBuffers = MeshRenderBuffers<vcl::TriMesh>(mesh, btf);
    }

    // Disable copy
    Shape(const Shape&)            = delete;
    Shape& operator=(const Shape&) = delete;

    // Enable move
    Shape(Shape&& other) { swap(other); }

    Shape& operator=(Shape&& other)
    {
        swap(other);
        return *this;
    }

    void swap(Shape& other)
    {
        using std::swap;
        mBuffers.swap(other.mBuffers);
    }

    friend void swap(Shape& a, Shape& b) { a.swap(b); }

    /**
     * @brief Draws the shape with the specified transform and color.
     *
     * @param[in] viewId: The view ID for the BGFX draw call.
     * @param[in] color: The color to draw the shape with.
     * @param[in] transform: The transformation matrix to apply to the shape
     * (default is identity).
     * @param[in] state: The BGFX render state flags (default is
     * DEFAULT_DRAW_STATE).
     */
    void draw(
        uint             viewId,
        const Color&     color,
        const Matrix44f& transform = Matrix44f::Identity(),
        uint64_t         state     = DEFAULT_DRAW_STATE)
    {
        using enum VertFragProgram;

        ProgramManager& pm = Context::instance().programManager();

        ShapeUniforms::setColor(color);
        ShapeUniforms::bind();

        mBuffers.bindVertexBuffers(MeshRenderSettings());
        mBuffers.bindIndexBuffers(MeshRenderSettings());

        bgfx::setTransform(transform.data());
        bgfx::setState(state);
        bgfx::submit(viewId, pm.getProgram<SHAPE>());
    }

    /**
     * @brief Draws the shape for picking.
     */
    void drawId(
        uint             viewId,
        uint32_t         id,
        const Matrix44f& transform = Matrix44f::Identity())
    {
        using enum VertFragProgram;

        ProgramManager& pm = Context::instance().programManager();

        uint64_t state =
            0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
            BGFX_STATE_DEPTH_TEST_LEQUAL |
            BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ZERO);

        mBuffers.bindVertexBuffers(MeshRenderSettings());
        mBuffers.bindIndexBuffers(MeshRenderSettings());

        ShapeUniforms::setId(id);
        ShapeUniforms::bind();

        bgfx::setState(state);
        bgfx::setTransform(transform.data());

        bgfx::submit(viewId, pm.getProgram<SHAPE_ID>());
    }
};

} // namespace vcl

#endif // VCL_BGFX_SHAPES_SHAPE_H
