/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_AXIS_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_AXIS_H

#include "mesh/mesh_render_buffers.h"
#include "uniforms/drawable_axis_uniforms.h"

#include <vclib/algorithms/mesh/create.h>
#include <vclib/meshes/tri_mesh.h>
#include <vclib/render/drawable/drawable_object.h>
#include <vclib/space/core/matrix.h>

#include <vclib/bgfx/context.h>

#include <bgfx/bgfx.h>

namespace vcl {

class DrawableAxis : public DrawableObject
{
    inline static const std::pair<vcl::TriMesh, vcl::TriMesh> AXIS_MESHES =
        vcl::createAxisDisjoint<vcl::TriMesh>();

    bool mVisible = false;

    const vcl::Color mColors[3] = {
        vcl::Color::Red,
        vcl::Color::Green,
        vcl::Color::Blue};

    vcl::Matrix44f mMatrices[3] = {
        vcl::Matrix44f::Zero(),
        vcl::Matrix44f::Zero(),
        vcl::Matrix44f::Zero()};

    MeshRenderBuffers<vcl::TriMesh> mArrowBuffers[2]; // 0: cylinder, 1: cone

    bgfx::ProgramHandle mProgram =
        Context::instance().programManager().getProgram(
            VclProgram::DRAWABLE_AXIS);

    mutable DrawableAxisUniforms mUniforms;

public:
    DrawableAxis(double size = 1);

    DrawableAxis(const DrawableAxis& other) :
        mVisible(other.mVisible),
        mUniforms(other.mUniforms)
    {
        for (uint i = 0; i < 3; i++) {
            mMatrices[i] = other.mMatrices[i];
        }

        createAxis();
    }

    DrawableAxis(DrawableAxis&& other) { swap(other); }

    ~DrawableAxis() = default;

    DrawableAxis& operator=(DrawableAxis other)
    {
        swap(other);
        return *this;
    }

    void swap(DrawableAxis& other)
    {
        using std::swap;
        swap(mVisible, other.mVisible);
        swap(mUniforms, other.mUniforms);
        for (uint i = 0; i < 3; i++) {
            swap(mMatrices[i], other.mMatrices[i]);
        }
        for (uint i = 0; i < 2; i++) {
            mArrowBuffers[i].swap(other.mArrowBuffers[i]);
        }
    }

    friend void swap(DrawableAxis& a, DrawableAxis& b) { a.swap(b); }

    void setSize(double size);

    // DrawableObject interface

    void draw(uint viewId) const override;

    Box3d boundingBox() const override { return Box3d(); }

    std::shared_ptr<DrawableObject> clone() const override
    {
        return std::make_shared<DrawableAxis>(*this);
    }

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }

private:
    void updateMatrices(double size);

    void createAxis();
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_AXIS_H
