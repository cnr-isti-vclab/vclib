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

#ifndef VCL_BGFX_PRIMITIVES_LINES_PRIMITIVE_LINES_H
#define VCL_BGFX_PRIMITIVES_LINES_PRIMITIVE_LINES_H

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>

#include <variant>

namespace vcl::detail {

// Note: copy constructor and assignment are not allowed (because of bgfx
// handles). Move constructor and assignment are allowed.
class PrimitiveLines
{
    bgfx::ProgramHandle mLinesPH =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::PRIMITIVE_LINES>();

    enum Ownership { OWNED = 0, NOT_OWNED = 1 };

    // true if the buffers are created and managed internally
    // by default, all variants are owned (first element of the variant)
    bool mOwnsBuffers = true;

    std::variant<VertexBuffer, const VertexBuffer*> mVertexCoords;
    std::variant<VertexBuffer, const VertexBuffer*> mVertexNormals;
    std::variant<VertexBuffer, const VertexBuffer*> mVertexColors;
    std::variant<IndexBuffer, const IndexBuffer*>   mLineColors;

    std::variant<IndexBuffer, const IndexBuffer*> mIndices;

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
    void reinitBuffers(Ownership owned);

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
