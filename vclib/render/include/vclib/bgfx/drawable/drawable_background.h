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

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/uniform.h>

#include <vclib/render/drawable/drawable_object.h>

namespace vcl {

class DrawableBackground : public vcl::DrawableObject
{
    bool mVisible        = true;

    vcl::VertexBuffer mVertexBuffer;

    static const uint mVertexNumber = 3;

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

    void swap(DrawableBackground& other)
    {
        using std::swap;
        DrawableObject::swap(other);

        swap(mVisible, other.mVisible);
        mVertexBuffer.swap(other.mVertexBuffer);
    }

    friend void swap(DrawableBackground& first, DrawableBackground& second)
    {
        first.swap(second);
    }

    // DrawableObject interface

    void init() override
    {
        name() = "Background";
        using enum bgfx::Attrib::Enum;
        using enum vcl::PrimitiveType;

        float verts[mVertexNumber * 3] {
            -3, -1,  1,
             1, -1,  1,
             1,  3,  1
        };

        auto [vertices, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(mVertexNumber * 3);

        std::copy(verts, verts + mVertexNumber * 3, vertices);

        mVertexBuffer.create(
            vertices, 
            mVertexNumber,
            Position,
            3,           // attributes per vertex
            FLOAT,
            false,       // data is normalized
            releaseFn
        );

    }

    void draw(const DrawObjectSettings& settings) const override
    {
        using enum VertFragProgram;

        ProgramManager& pm = Context::instance().programManager();

        mVertexBuffer.bindVertex(0);

        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL);

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

    private:

    template<typename T>
    std::pair<T*, bgfx::ReleaseFn> getAllocatedBufferAndReleaseFn(uint size)
    {
        T* buffer = new T[size];

        return std::make_pair(buffer, [](void* ptr, void*) {
            delete[] static_cast<T*>(ptr);
        });
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_BACKGROUND_H
