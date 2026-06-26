// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_PRIMITIVES_LINES_CPU_GENERATED_LINES_H
#define VCL_BGFX_PRIMITIVES_LINES_CPU_GENERATED_LINES_H

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>

namespace vcl::detail {

// Note: copy constructor and assignment are not allowed (because of bgfx
// handles). Move constructor and assignment are allowed.
class CPUGeneratedLines
{
    bgfx::ProgramHandle mLinesPH =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::CUSTOM_CPU_LINES>();

    VertexBuffer mVertexCoords;
    VertexBuffer mVertexNormals;
    VertexBuffer mVertexColors;
    IndexBuffer  mLineColors;

    IndexBuffer mIndices;

public:
    CPUGeneratedLines() = default;

    CPUGeneratedLines(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors  = std::vector<uint>(),
        const std::vector<uint>&  lineColors  = std::vector<uint>());

    CPUGeneratedLines(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors  = std::vector<uint>(),
        const std::vector<uint>&  lineColors  = std::vector<uint>());

    void swap(CPUGeneratedLines& other);

    friend void swap(CPUGeneratedLines& a, CPUGeneratedLines& b) { a.swap(b); }

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors  = std::vector<uint>(),
        const std::vector<uint>&  lineColors  = std::vector<uint>());

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors  = std::vector<uint>(),
        const std::vector<uint>&  lineColors  = std::vector<uint>());

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

#endif // VCL_BGFX_PRIMITIVES_LINES_CPU_GENERATED_LINES_H
