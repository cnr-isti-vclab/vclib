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

#ifndef VCL_BGFX_DRAWABLE_POLYLINES_TEXTURE_BASED_POLYLINES_H
#define VCL_BGFX_DRAWABLE_POLYLINES_TEXTURE_BASED_POLYLINES_H

#include "polyline_settings.h"

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/lines_common/lines.h>

namespace vcl {

class TextureBasedPolylines : public Lines<PolylineSettings>
{
    static const inline std::vector<float> VERTICES =
        {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
    static const inline std::vector<uint> INDICES = {0, 3, 1, 0, 2, 3};

    bgfx::ProgramHandle mLinesPH =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::POLYLINES_TEXTURE>();

    bgfx::ProgramHandle mJointsPH =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::POLYLINES_TEXTURE_JOINTS>();

    bgfx::ProgramHandle mComputeTexturePH =
        Context::instance()
            .programManager()
            .getComputeProgram<ComputeProgram::POLYLINES_TEXTURE>();

    const uint mMaxTextureSize = bgfx::getCaps()->limits.maxTextureSize;

    VertexBuffer mVertices;
    IndexBuffer  mIndices;

    VertexBuffer  mPoints;
    TextureBuffer mSegmentsTexture;
    TextureBuffer mJointsTexture;

    IndirectBuffer mSegmentsIndirect;
    IndirectBuffer mJointsIndirect;
    Uniform mIndirectData = Uniform("u_IndirectData", bgfx::UniformType::Vec4);

public:
    TextureBasedPolylines();

    TextureBasedPolylines(const std::vector<LinesVertex>& points);

    void swap(TextureBasedPolylines& other);

    friend void swap(TextureBasedPolylines& a, TextureBasedPolylines& b)
    {
        a.swap(b);
    }

    void draw(uint viewId) const;

    void setPoints(const std::vector<LinesVertex>& points);

private:
    void checkCaps() const
    {
        const bgfx::Caps* caps      = bgfx::getCaps();
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

    void allocateAndSetPointsBuffer(const std::vector<LinesVertex>& points);

    void allocateAndGenerateTextureBuffer(uint pointSize);
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_POLYLINES_TEXTURE_BASED_POLYLINES_H
