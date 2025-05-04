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

#include <vclib/bgfx/drawable/polylines/gpu_generated_polylines.h>

namespace vcl {

GPUGeneratedPolylines::GPUGeneratedPolylines()
{
    checkCaps();
}

GPUGeneratedPolylines::GPUGeneratedPolylines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals) : Lines()
{
    setPoints(vertCoords, vertColors, vertNormals);
}

void GPUGeneratedPolylines::swap(GPUGeneratedPolylines& other)
{
    using std::swap;

    Lines::swap(other);

    swap(mVertCoords, other.mVertCoords);
    swap(mVertColors, other.mVertColors);
    swap(mVertNormals, other.mVertNormals);

    swap(mVertices, other.mVertices);

    swap(mSegmentIndices, other.mSegmentIndices);
    swap(mJointIndices, other.mJointIndices);

    swap(mComputeData, other.mComputeData);
}

void GPUGeneratedPolylines::draw(uint viewId) const
{
    bindSettingsUniform();

    mVertices.bind(0);
    mSegmentIndices.bind();
    bgfx::setState(drawState());
    bgfx::submit(viewId, mLinesPH);

    // mJointIndices is valid only if there are more than 2 points
    if (mJointIndices.isValid() && settings().getJoint() != PolyLineJoint::ROUND_JOINT) {
        mVertices.bind(0);
        mJointIndices.bind();
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);
    }
}

void GPUGeneratedPolylines::setPoints(        
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  vertColors,
    const std::vector<float>& vertNormals)
{
    const uint nPoints = vertCoords.size() / 3;

    if (nPoints > 1) {
        setCoordsBuffers(vertCoords);
        setColorsBuffers(vertColors);
        setNormalsBuffers(vertNormals);

        allocateVertexBuffer(nPoints);
        allocateIndexBuffer(nPoints);
        // buffers are set for the compute stage
        generateVerticesAndIndicesBuffers(nPoints);
        // here buffers are computed and ready for rendering
    }
    else {
        mVertCoords.destroy();
        mVertColors.destroy();
        mVertNormals.destroy();
        mVertices.destroy();
        mSegmentIndices.destroy();
        mJointIndices.destroy();
    }
}

void GPUGeneratedPolylines::setCoordsBuffers(const std::vector<float>& vertCoords)
{
    auto [buffer, releaseFn] =
        getAllocatedBufferAndReleaseFn<float>(vertCoords.size());

    std::copy(vertCoords.begin(), vertCoords.end(), buffer);

    mVertCoords.createForCompute(
        buffer,
        vertCoords.size() / 3,
        bgfx::Attrib::Position,
        3,
        PrimitiveType::FLOAT,
        false,
        bgfx::Access::Read,
        releaseFn
    );
}

void GPUGeneratedPolylines::setColorsBuffers(const std::vector<uint>& vertColors)
{
    auto [buffer, releaseFn] =
        getAllocatedBufferAndReleaseFn<uint>(vertColors.size());

    std::copy(vertColors.begin(), vertColors.end(), buffer);

    mVertColors.createForCompute(
        buffer,
        vertColors.size(),
        bgfx::Attrib::Color0,
        4,
        PrimitiveType::UCHAR,
        true,
        bgfx::Access::Read,
        releaseFn
    );
}

void GPUGeneratedPolylines::setNormalsBuffers(const std::vector<float>& vertNormals)
{
    auto [buffer, releaseFn] =
        getAllocatedBufferAndReleaseFn<float>(vertNormals.size());

    std::copy(vertNormals.begin(), vertNormals.end(), buffer);

    mVertNormals.createForCompute(
        buffer,
        vertNormals.size() / 3,
        bgfx::Attrib::Normal,
        3,
        PrimitiveType::FLOAT,
        false,
        bgfx::Access::Read,
        releaseFn
    );
}

void GPUGeneratedPolylines::allocateVertexBuffer(uint pointSize)
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord2, 3, bgfx::AttribType::Float)
        .end();

    const uint sz = (pointSize - 1) * 4 * 16;

    mVertices.create(
        bgfx::makeRef(nullptr, sizeof(float) * sz),
        layout,
        BGFX_BUFFER_COMPUTE_WRITE,
        true);
}

void GPUGeneratedPolylines::allocateIndexBuffer(uint pointSize)
{
    const uint szSeg   = (pointSize - 1) * 6;
    const uint szJoint = (pointSize - 2) * 6;

    mSegmentIndices.create(
        bgfx::makeRef(nullptr, sizeof(uint) * szSeg),
        BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_INDEX32,
        true);

    if (pointSize > 2) {
        mJointIndices.create(
            bgfx::makeRef(nullptr, sizeof(uint) * szJoint),
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_INDEX32,
            true);
    }
    else {
        mJointIndices.destroy();
    }
}

void GPUGeneratedPolylines::generateVerticesAndIndicesBuffers(uint pointSize)
{
    float data[] = {static_cast<float>(pointSize - 1), 0, 0, 0};
    mComputeData.bind(data);

    mVertCoords.bind(0);
    mVertColors.bind(1);
    mVertNormals.bind(2);

    mVertices.bind(3, bgfx::Access::Write);
    mSegmentIndices.bind(4, bgfx::Access::Write);
    mJointIndices.bind(5, bgfx::Access::Write);

    bgfx::dispatch(0, mComputeVertexPH, pointSize - 1, 1, 1);
    // after the dispatch, the vert and indices buffers are ready to be used in
    // the rendering pipeline
    mVertices.setCompute(false);
    mSegmentIndices.setCompute(false);
    mJointIndices.setCompute(false);
}

} // namespace vcl
