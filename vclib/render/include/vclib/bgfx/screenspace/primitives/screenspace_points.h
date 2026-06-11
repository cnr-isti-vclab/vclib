/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_POINTS_H
#define VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_POINTS_H

#include "uniforms/screenspace_points_uniforms.h"

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/programs/compute_loader.h>
#include <vclib/bgfx/programs/vert_frag_loader.h>

namespace vcl {

class ScreenSpacePoints
{
    inline static const VertexBuffer NULL_VERTEX_BUFFER;

public:
    enum class PointsColor {
        PER_POINT, // Select color from point buffer color
        GENERAL,   // Select color from general color
    };

    enum class PointsShape {
        SQUARE, // square splats
        CIRCLE  // circular splats
    };

private:
    uint mPointsCount = 0;

    float       mWidth        = 1.0f;
    PointsColor mColorToUse   = PointsColor::GENERAL;
    PointsShape mShape        = PointsShape::SQUARE;
    Color       mGeneralColor = Color::Black;

    OwnedOrRefBuffer<VertexBuffer> mPoints;
    OwnedOrRefBuffer<VertexBuffer> mPointColors;

    VertexBuffer mPointSplats;
    IndexBuffer  mPointSplatIndices;

public:
    ScreenSpacePoints() = default;

    ScreenSpacePoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  vertColors = std::vector<uint>())
    {
        setPoints(vertCoords, vertColors);
    }

    ScreenSpacePoints(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const VertexBuffer& vertexColors = NULL_VERTEX_BUFFER)
    {
        setPoints(pointsSize, vertexCoords, vertexColors);
    }

    void setPoints(
        const uint   pointsSize,
        const float* vertCoords,
        const uint*  vertColors = nullptr)
    {
        assert(vertCoords.size() % 2 == 0);

        mPointsCount = pointsSize;

        VertexBuffer points;
        {
            const uint numVec4 = (mPointsCount + 1u) / 2u;

            auto [buffer, releaseFn] =
                Context::getAllocatedBufferAndReleaseFn<float>(numVec4 * 4);

            for (uint i = 0; i < mPointsCount; ++i) {
                const uint vecIdx = i / 2u;
                if ((i & 1u) == 0u) {
                    buffer[vecIdx * 4 + 0] = vertCoords[i * 2 + 0];
                    buffer[vecIdx * 4 + 1] = vertCoords[i * 2 + 1];
                }
                else {
                    buffer[vecIdx * 4 + 2] = vertCoords[i * 2 + 0];
                    buffer[vecIdx * 4 + 3] = vertCoords[i * 2 + 1];
                }
            }

            points.createForCompute(
                buffer,
                numVec4,
                bgfx::Attrib::Position,
                4,
                PrimitiveType::FLOAT,
                false,
                bgfx::Access::Read,
                releaseFn);
        }
        mPoints.setOwned(std::move(points));

        if (vertColors) {
            assert(vertColors.size() == mPointsSize);

            VertexBuffer pointColors;
            {
                auto [buffer, releaseFn] =
                    Context::getAllocatedBufferAndReleaseFn<uint>(mPointsCount);

                std::copy(vertColors, vertColors + mPointsCount, buffer);

                pointColors.createForCompute(
                    buffer,
                    mPointsCount,
                    bgfx::Attrib::Color0,
                    4,
                    PrimitiveType::UCHAR,
                    true,
                    bgfx::Access::Read,
                    releaseFn);
            }
            mPointColors.setOwned(std::move(pointColors));
        }
        else {
            mPointColors.setOwned();
        }

        setPointSplatsBuffer(mPointSplats, mPointsCount);
        setPointSplatIndicesBuffer(mPointSplatIndices, mPointsCount);
    }

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  vertColors = std::vector<uint>())
    {
        setPoints(
            vertCoords.size() / 2,
            vertCoords.data(),
            vertColors.empty() ? nullptr : vertColors.data());
    }

    void setPoints(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const VertexBuffer& vertexColors = NULL_VERTEX_BUFFER)
    {
        mPointsCount = pointsSize;

        mPoints.setReferenced(&vertexCoords);
        mPointColors.setReferenced(&vertexColors);

        setPointSplatsBuffer(mPointSplats, mPointsCount);
        setPointSplatIndicesBuffer(mPointSplatIndices, mPointsCount);
    }

    void setWidthSetting(float width) { mWidth = width; }

    void setColorSetting(PointsColor colorToUse) { mColorToUse = colorToUse; }

    void setShapeSetting(PointsShape shape) { mShape = shape; }

    void setGeneralColor(const Color& generalColor)
    {
        mGeneralColor = generalColor;
    }

    void draw(bgfx::ViewId viewId) const
    {
        if (mPointsCount == 0 || !mPoints.isValid() ||
            !mPointSplats.isValid() || !mPointSplatIndices.isValid()) {
            return;
        }

        Context& ctx = Context::instance();
        if (!ctx.supportsCompute()) {
            return;
        }

        ProgramManager& pm = ctx.programManager();

        PointsColor colorToUse = mColorToUse;
        if (colorToUse == PointsColor::PER_POINT && !mPointColors.isValid()) {
            colorToUse = PointsColor::GENERAL;
        }

        ScreenSpacePointsUniforms::setPointsColor(
            static_cast<uint>(colorToUse));
        ScreenSpacePointsUniforms::setPointsShape(static_cast<uint>(mShape));
        ScreenSpacePointsUniforms::setPointsWidth(mWidth);
        ScreenSpacePointsUniforms::setPointsGeneralColor(mGeneralColor);

        mPoints.get().bindCompute(POINTS_POSITIONS_STAGE, bgfx::Access::Read);
        if (mPointColors.isValid()) {
            mPointColors.get().bindCompute(
                POINTS_COLORS_STAGE, bgfx::Access::Read);
        }
        mPointSplats.bindCompute(POINTS_OUTPUT_STAGE, bgfx::Access::Write);

        ScreenSpacePointsUniforms::bind();
        bgfx::dispatch(
            viewId,
            pm.getComputeProgram<ComputeProgram::SCREENSPACE_POINTS>(),
            mPointsCount,
            1,
            1);

        mPointSplats.bindVertex(0);
        mPointSplatIndices.bind();

        bgfx::setState(
            0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
            BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_BLEND_ALPHA);

        ScreenSpacePointsUniforms::bind();
        bgfx::submit(
            viewId, pm.getProgram<VertFragProgram::SCREENSPACE_POINTS>());
    }

private:
    static constexpr uint POINTS_POSITIONS_STAGE = 0;
    static constexpr uint POINTS_COLORS_STAGE    = 1;
    static constexpr uint POINTS_OUTPUT_STAGE    = 2;

    static constexpr uint POINTS_SPLAT_INDEX_COUNT_PER_POINT = 6;
    static constexpr uint POINTS_SPLAT_VERTEX_COUNT_PER_POINT = 4;

    void setPointSplatsBuffer(vcl::VertexBuffer& splats, uint pointsSize)
    {
        if (pointsSize == 0) {
            splats.destroy();
            return;
        }

        const uint splatVertCount = pointsSize * POINTS_SPLAT_VERTEX_COUNT_PER_POINT;

        auto [buffer, releaseFn] =
            vcl::Context::getAllocatedBufferAndReleaseFn<float>(splatVertCount * 4);

        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 1, bgfx::AttribType::Float)
            .end();

        splats.create(
            bgfx::makeRef(buffer, splatVertCount * 4 * sizeof(float), releaseFn),
            layout,
            BGFX_BUFFER_COMPUTE_WRITE);
    }

    void setPointSplatIndicesBuffer(vcl::IndexBuffer& indices, uint pointsSize)
    {
        const uint indexCount = pointsSize * POINTS_SPLAT_INDEX_COUNT_PER_POINT;

        auto [buffer, releaseFn] =
            vcl::Context::getAllocatedBufferAndReleaseFn<uint>(indexCount);

        for (uint i = 0; i < pointsSize; ++i) {
            const uint v = i * POINTS_SPLAT_VERTEX_COUNT_PER_POINT;
            const uint k = i * POINTS_SPLAT_INDEX_COUNT_PER_POINT;

            buffer[k + 0] = v + 0;
            buffer[k + 1] = v + 1;
            buffer[k + 2] = v + 2;
            buffer[k + 3] = v + 2;
            buffer[k + 4] = v + 1;
            buffer[k + 5] = v + 3;
        }

        indices.create(buffer, indexCount, true, releaseFn);
    }
};

} // namespace vcl

#endif // VCL_BGFX_SCREENSPACE_PRIMITIVES_SCREENSPACE_POINTS_H
