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

#include <vclib/bgfx/primitives/lines/primitive_lines.h>

#include <vclib/bgfx/primitives/lines/lines_utils.h>

namespace vcl::detail {

PrimitiveLines::PrimitiveLines(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    setPoints(vertCoords, vertNormals, vertColors, lineColors);
}

PrimitiveLines::PrimitiveLines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    setPoints(vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

PrimitiveLines::PrimitiveLines(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const IndexBuffer&  lineColors)
{
    setPoints(
        pointsSize, vertexCoords, vertexNormals, vertexColors, lineColors);
}

PrimitiveLines::PrimitiveLines(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const IndexBuffer&  lineColors)
{
    setPoints(
        pointsSize,
        vertexCoords,
        lineIndices,
        vertexNormals,
        vertexColors,
        lineColors);
}

void PrimitiveLines::swap(PrimitiveLines& other)
{
    using std::swap;

    swap(mVertexCoords, other.mVertexCoords);
    swap(mVertexNormals, other.mVertexNormals);
    swap(mVertexColors, other.mVertexColors);
    swap(mLineColors, other.mLineColors);
    swap(mIndices, other.mIndices);
}

void PrimitiveLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    const std::vector<uint> lineIndices;
    setPoints(
        false, vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

void PrimitiveLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    setPoints(
        true, vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

void PrimitiveLines::setPoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const IndexBuffer&  lineColors)
{
    IndexBuffer indices;
    setPoints(
        pointsSize,
        vertexCoords,
        indices,
        vertexNormals,
        vertexColors,
        lineColors);
}

void PrimitiveLines::setPoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const IndexBuffer&  lineColors)
{
    if (vertexCoords.isValid())
        mVertexCoords.setReferenced(&vertexCoords);
    if (vertexNormals.isValid())
        mVertexNormals.setReferenced(&vertexNormals);
    if (vertexColors.isValid())
        mVertexColors.setReferenced(&vertexColors);
    if (lineColors.isValid())
        mLineColors.setReferenced(&lineColors);
    if (lineIndices.isValid())
        mIndices.setReferenced(&lineIndices);
}

void PrimitiveLines::draw(uint viewId) const
{
    if (mVertexCoords)
        mVertexCoords.get().bind(0);
    if (mVertexNormals)
        mVertexNormals.get().bind(1);
    if (mVertexColors)
        mVertexColors.get().bind(2);
    if (mLineColors)
        mLineColors.get().bind(0);
    if (mIndices)
        mIndices.get().bind();

    bgfx::setState(linesDrawState() | BGFX_STATE_PT_LINES);
    bgfx::submit(viewId, mLinesPH);
}

void PrimitiveLines::setPoints(
    bool                      setLineIndices,
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    assert(vertCoords.size() % 3 == 0);
    assert(lineIndices.size() % 2 == 0);

    const bool setColors     = vertColors.size() != 0;
    const bool setNormals    = vertNormals.size() != 0;
    const bool setLineColors = lineColors.size() != 0;

    const uint numVertices = vertCoords.size() / 3;
    const uint numElements = setLineIndices ? lineIndices.size() : numVertices;

    assert(!setColors || vertCoords.size() == vertColors.size() * 3);
    assert(!setNormals || vertCoords.size() == vertNormals.size());
    assert(
        !setLineColors ||
        (setLineIndices && lineIndices.size() == lineColors.size() * 2) ||
        (!setLineIndices && vertCoords.size() / 3 == lineColors.size() * 2));

    if (numElements > 1) {
        // coordinates
        auto [vCoords, vCoordsReleaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(vertCoords.size());

        std::copy(vertCoords.begin(), vertCoords.end(), vCoords);

        bgfx::VertexLayout vCoordslayout;
        vCoordslayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();

        VertexBuffer tmpVP;
        tmpVP.create(
            bgfx::makeRef(
                vCoords, sizeof(float) * vertCoords.size(), vCoordsReleaseFn),
            vCoordslayout);
        mVertexCoords.setOwned(std::move(tmpVP));

        // normals
        auto [vNormals, vNormalsReleaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(vertCoords.size());

        if (!setNormals) {
            for (uint i = 0; i < vertCoords.size(); i++)
                vNormals[i] = 0.0f;
        }
        else {
            std::copy(vertNormals.begin(), vertNormals.end(), vNormals);
        }

        bgfx::VertexLayout vNormalsLayout;
        vNormalsLayout.begin()
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float, true)
            .end();

        VertexBuffer tmpVN;
        tmpVN.create(
            bgfx::makeRef(
                vNormals, sizeof(float) * vertCoords.size(), vNormalsReleaseFn),
            vNormalsLayout);
        mVertexNormals.setOwned(std::move(tmpVN));

        // colors
        auto [vColors, vColorsReleaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(numVertices);

        if (!setColors) {
            for (uint i = 0; i < numVertices; ++i)
                vColors[i] = 0xffffffff;
        }
        else {
            std::copy(vertColors.begin(), vertColors.end(), vColors);
        }

        bgfx::VertexLayout vColorsLayout;
        vColorsLayout.begin()
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

        VertexBuffer tmpVC;
        tmpVC.create(
            bgfx::makeRef(
                vColors, sizeof(uint32_t) * numVertices, vColorsReleaseFn),
            vColorsLayout);
        mVertexColors.setOwned(std::move(tmpVC));

        // line indices
        if (setLineIndices) {
            auto [lineInds, lineIndsReleaseFn] =
                Context::getAllocatedBufferAndReleaseFn<uint>(
                    lineIndices.size());

            std::copy(lineIndices.begin(), lineIndices.end(), lineInds);

            IndexBuffer tmpIB;
            tmpIB.create(
                bgfx::makeRef(
                    lineInds,
                    sizeof(uint32_t) * lineIndices.size(),
                    lineIndsReleaseFn),
                BGFX_BUFFER_INDEX32);
            mIndices.setOwned(std::move(tmpIB));
        }

        // line colors
        if (setLineColors) {
            auto [lColors, lColorsReleaseFn] =
                Context::getAllocatedBufferAndReleaseFn<uint>(
                    lineColors.size());

            std::copy(lineColors.begin(), lineColors.end(), lColors);

            IndexBuffer tmpLC;
            tmpLC.createForCompute(
                lColors,
                lineColors.size(),
                PrimitiveType::UINT,
                bgfx::Access::Read,
                lColorsReleaseFn);
            mLineColors.setOwned(std::move(tmpLC));
        }
    }
    else {
        mVertexCoords.setOwned(VertexBuffer());
        mVertexNormals.setOwned(VertexBuffer());
        mVertexColors.setOwned(VertexBuffer());
        mIndices.setOwned(IndexBuffer());
        mLineColors.setOwned(IndexBuffer());
    }
}

} // namespace vcl::detail
