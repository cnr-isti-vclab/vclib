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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_BACKGROUND_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_BACKGROUND_H

#include <vclib/bgfx/context.h>
#include <vclib/render/drawable/drawable_object.h>

#include <bgfx/bgfx.h>

namespace vcl {

class DrawableBackground : public vcl::DrawableObject
{
    bool mVisible        = true;

    vcl::VertexBuffer mVertexBuffer;
    vcl::IndexBuffer mIndexBuffer;

public:
    DrawableBackground() = default;

    DrawableBackground(const DrawableBackground& other) :
            DrawableObject(other),
            mVisible(other.mVisible)
    {
    }

    DrawableBackground(DrawableBackground&& other) { swap(other); }

    ~DrawableBackground() = default;

    DrawableBackground& operator=(DrawableBackground other)
    {
        swap(other);
        return *this;
    }

    void init()
    {
        name() = "Background";
        using enum bgfx::Attrib::Enum;
        using enum vcl::PrimitiveType;

        float vertices[24] = {
            -1, -1, -1,
            1, -1, -1,
            1,  1, -1,
            -1,  1, -1,
            -1, -1,  1,
            1, -1,  1,
            1,  1,  1,
            -1,  1,  1,
        };
        mVertexBuffer.create(
            vertices, 
            8,           // number of vertices
            Position,
            3,           // attributes per vertex
            FLOAT
        );

        uint32_t indices[36] = {
            1, 2, 0,
            2, 3, 0,
            6, 2, 1,
            1, 5, 6,
            6, 5, 4,
            4, 7, 6,
            6, 3, 2,
            7, 3, 6,
            3, 7, 0,
            7, 4, 0,
            5, 1, 0,
            4, 5, 0
        };
        mIndexBuffer.create(indices, 36);
    }

    void swap(DrawableBackground& other)
    {
        using std::swap;
        DrawableObject::swap(other);

        swap(mVisible, other.mVisible);
        mVertexBuffer.swap(other.mVertexBuffer);
        mIndexBuffer.swap(other.mIndexBuffer);
    }

    friend void swap(DrawableBackground& first, DrawableBackground& second)
    {
        first.swap(second);
    }

    // DrawableObject interface

    void draw(const DrawObjectSettings& settings) const override
    {
        using enum VertFragProgram;

        ProgramManager& pm = Context::instance().programManager();

        uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z;

        vcl::Matrix44f model = vcl::Matrix44f::Identity();

        mVertexBuffer.bindVertex(0);
        mIndexBuffer.bind(0, 36);

        bgfx::setState(state);
        bgfx::setTransform(model.data());

        bgfx::submit(settings.viewId, pm.getProgram<DRAWABLE_BACKGROUND_PBR>());
    }

    vcl::Box3d boundingBox() const override
    {
        return vcl::Box3d();
    }

    std::shared_ptr<DrawableObject> clone() const& override
    {
        return std::make_shared<DrawableBackground>(*this);
    }

    std::shared_ptr<DrawableObject> clone() && override
    {
        return std::make_shared<DrawableBackground>(std::move(*this));
    }

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_BACKGROUND_H
