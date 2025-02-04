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

#ifndef VCL_BGFX_DRAWABLE_LINES_POLYLINES_TEXTURE_BASED_POLYLINES_H
#define VCL_BGFX_DRAWABLE_LINES_POLYLINES_TEXTURE_BASED_POLYLINES_H

#include <vclib/bgfx/drawable/lines/common/lines.h>

#include <vclib/bgfx/context.h>

namespace vcl::lines {

class TextureBasedPolylines : public Lines
{
    static const inline std::vector<float>    VERTICES =
        {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
    static const inline std::vector<uint> INDICES = {0, 3, 1, 0, 2, 3};

    bgfx::ProgramHandle mLinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_TEXTURE_BASED_VSFS);

    bgfx::ProgramHandle mJoinesPH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_TEXTURE_BASED_JOINS_VSFS);

    bgfx::ProgramHandle mComputeTexturePH =
        Context::instance().programManager().getProgram(
            VclProgram::POLYLINES_TEXTURE_BASED_CS);

    uint mMaxTextureSize;
    uint mPointsSize = 0;

    bgfx::VertexBufferHandle        mVerticesBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle         mIndexesBH  = BGFX_INVALID_HANDLE;
    bgfx::DynamicVertexBufferHandle mPointsBH   = BGFX_INVALID_HANDLE;

    bgfx::IndirectBufferHandle mSegmentsIndirectBH = BGFX_INVALID_HANDLE;
    bgfx::IndirectBufferHandle mJoinesIndirectBH   = BGFX_INVALID_HANDLE;

    bgfx::TextureHandle mSegmentsTextureBH = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle mJoinesTextureBH   = BGFX_INVALID_HANDLE;

    bgfx::UniformHandle mComputeDataUH = BGFX_INVALID_HANDLE;

public:
    TextureBasedPolylines() { checkCaps(); }

    TextureBasedPolylines(
        const std::vector<LinesVertex>& points,
        const uint maxTextureSize = bgfx::getCaps()->limits.maxTextureSize);

    TextureBasedPolylines(const TextureBasedPolylines& other) = delete;

    TextureBasedPolylines(TextureBasedPolylines&& other);

    ~TextureBasedPolylines();

    TextureBasedPolylines& operator=(const TextureBasedPolylines& other) =
        delete;

    TextureBasedPolylines& operator=(TextureBasedPolylines&& other);

    void swap(TextureBasedPolylines& other);

    void draw(uint viewId) const;

    void update(const std::vector<LinesVertex>& points);

private:
    void generateTextureBuffer();

    void allocateTextureBuffer();

    void allocatePointsBuffer();

    void allocateVerticesBuffer();

    void allocateIndexesBuffer();

    void setPointsBuffer(const std::vector<LinesVertex>& points);

    void checkCaps() const
    {
        const bgfx::Caps* caps = bgfx::getCaps();
        const bool computeSupported = bool(caps->supported & BGFX_CAPS_COMPUTE);
        const bool indirectSupported =
            bool(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
        const bool instancingSupported =
            bool(caps->supported & BGFX_CAPS_INSTANCING);
        const bool textureSupported =
            bool(caps->supported & BGFX_CAPS_TEXTURE_2D_ARRAY);

        if (!(instancingSupported && computeSupported && indirectSupported &&
              textureSupported)) {
            throw std::runtime_error(
                "Instancing or compute or indirect or texture are not "
                "supported");
        }
    }
};

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_POLYLINES_TEXTURE_BASED_POLYLINES_H
