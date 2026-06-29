// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_PRIMITIVES_LINES_PRIMITIVE_LINES_H
#define VCL_BGFX_PRIMITIVES_LINES_PRIMITIVE_LINES_H

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>

namespace vcl::detail {

// Note: copy constructor and assignment are not allowed (because of bgfx
// handles). Move constructor and assignment are allowed.
class PrimitiveLines
{
    bgfx::ProgramHandle mLinesPH =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::PRIMITIVE_LINES>();

    vcl::OwnedOrRefBuffer<VertexBuffer> mVertexCoords;
    vcl::OwnedOrRefBuffer<VertexBuffer> mVertexNormals;
    vcl::OwnedOrRefBuffer<VertexBuffer> mVertexColors;
    vcl::OwnedOrRefBuffer<IndexBuffer>  mLineColors;

    vcl::OwnedOrRefBuffer<IndexBuffer> mIndices;

public:
    PrimitiveLines() = default;

    PrimitiveLines(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors  = std::vector<uint>(),
        const std::vector<uint>&  lineColors  = std::vector<uint>());

    PrimitiveLines(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors  = std::vector<uint>(),
        const std::vector<uint>&  lineColors  = std::vector<uint>());

    PrimitiveLines(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const VertexBuffer& vertexNormals = VertexBuffer(),
        const VertexBuffer& vertexColors  = VertexBuffer(),
        const IndexBuffer&  lineColors    = IndexBuffer());

    PrimitiveLines(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const IndexBuffer&  lineIndices,
        const VertexBuffer& vertexNormals = VertexBuffer(),
        const VertexBuffer& vertexColors  = VertexBuffer(),
        const IndexBuffer&  lineColors    = IndexBuffer());

    void swap(PrimitiveLines& other);

    friend void swap(PrimitiveLines& a, PrimitiveLines& b) { a.swap(b); }

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors);

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors);

    void setPoints(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const VertexBuffer& vertexNormals = VertexBuffer(),
        const VertexBuffer& vertexColors  = VertexBuffer(),
        const IndexBuffer&  lineColors    = IndexBuffer());

    void setPoints(
        const uint          pointsSize,
        const VertexBuffer& vertexCoords,
        const IndexBuffer&  lineIndices,
        const VertexBuffer& vertexNormals = VertexBuffer(),
        const VertexBuffer& vertexColors  = VertexBuffer(),
        const IndexBuffer&  lineColors    = IndexBuffer());

    void draw(uint viewId) const;

private:
    void setPoints(
        bool                      setLineIndices,
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals,
        const std::vector<uint>&  vertColors,
        const std::vector<uint>&  lineColors);
};

} // namespace vcl::detail

#endif // VCL_BGFX_PRIMITIVES_LINES_PRIMITIVE_LINES_H
