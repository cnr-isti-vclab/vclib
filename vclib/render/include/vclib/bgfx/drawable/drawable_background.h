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
#include <vclib/io/image/hdr/load.h>

#include <bgfx/bgfx.h>

namespace vcl {

class DrawableBackground : public vcl::DrawableObject
{
    bool mVisible        = true;

    vcl::VertexBuffer mVertexBuffer;
    vcl::IndexBuffer mIndexBuffer;

    std::unique_ptr<TextureUnit> mTextureUnit;

    static const uint vertexNumber = 8;
    static const uint indexNumber = 36;

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
        mIndexBuffer.swap(other.mIndexBuffer);
        swap(mTextureUnit, other.mTextureUnit);
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

        float verts[vertexNumber * 3] = {
            -1, -1, -1,
            1, -1, -1,
            1,  1, -1,
            -1,  1, -1,
            -1, -1,  1,
            1, -1,  1,
            1,  1,  1,
            -1,  1,  1,
        };

        auto [vertices, releaseFnVert] =
            getAllocatedBufferAndReleaseFn<float>(vertexNumber * 3);

        std::copy(verts, verts + vertexNumber * 3, vertices);

        mVertexBuffer.create(
            vertices, 
            vertexNumber,
            Position,
            3,           // attributes per vertex
            FLOAT,
            false,       // data is normalized
            releaseFnVert
        );

        uint32_t idxs[indexNumber] = {
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

        auto [indices, releaseFnIdx] =
            getAllocatedBufferAndReleaseFn<uint32_t>(indexNumber);

        std::copy(idxs, idxs + indexNumber, indices);

        mIndexBuffer.create(
            indices, 
            indexNumber,
            true,          // data is 32 bit
            releaseFnIdx
        );

        bimg::ImageContainer *cubemap = loadCubemapFromHdr(VCLIB_ASSETS_PATH "/pisa.hdr");
        std::string samplerName = "s_tex0";

        uint cubemapSize = bimg::imageGetSize(
            nullptr,
            cubemap->m_width,
            cubemap->m_height,
            1,
            true,
            false,
            1,
            bimg::TextureFormat::RGBA32F
        ) / 4; // in uints

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<uint>(cubemapSize);

        std::copy((uint*)(cubemap->m_data), ((uint*)(cubemap->m_data)) + cubemapSize, buffer);

        auto tu = std::make_unique<TextureUnit>();
        tu->set(
            buffer,
            Point2i(cubemap->m_width, cubemap->m_height),
            samplerName,
            false, // TODO: add mips when and WHERE needed
            BGFX_SAMPLER_UVW_CLAMP,
            bgfx::TextureFormat::RGBA32F,
            true,
            releaseFn
        );
        mTextureUnit = std::move(tu);
    }

    void draw(const DrawObjectSettings& settings) const override
    {
        using enum VertFragProgram;

        ProgramManager& pm = Context::instance().programManager();

        uint64_t state = BGFX_STATE_WRITE_MASK;

        vcl::Matrix44f model = vcl::Matrix44f::Identity();

        mTextureUnit->bind(8);

        mVertexBuffer.bindVertex(0);
        mIndexBuffer.bind(0, indexNumber);

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
