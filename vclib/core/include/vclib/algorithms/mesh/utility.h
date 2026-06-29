// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_UTILITY_H
#define VCL_ALGORITHMS_MESH_UTILITY_H

#include <vclib/base.h>
#include <vclib/mesh.h>

namespace vcl::detail {

// returns a non-empty vector if the ELEM_ID container is not compact and the
// user wants compact indices
template<uint ELEM_ID>
std::vector<uint> elemCompactIndices(const auto& mesh, bool wantCompact)
{
    std::vector<uint> elemCompIndices;

    bool isCompact = mesh.template count<ELEM_ID>() ==
                     mesh.template containerSize<ELEM_ID>();

    if (wantCompact && !isCompact)
        elemCompIndices = mesh.template compactIndices<ELEM_ID>();
    return elemCompIndices;
}

// returns a non-empty vector if the vertex container is not compact and the
// user wants compact indices
std::vector<uint> vertCompactIndices(const auto& mesh, bool wantCompact)
{
    return elemCompactIndices<ElemId::VERTEX>(mesh, wantCompact);
}

// lambda to get the vertex index of a face (considering compact vertex indices)
auto vIndexLambda(const auto& mesh, const std::vector<uint>& vertCompIndices)
{
    // lambda to get the vertex index of a face (considering compact indices)
    auto vIndex = [&vertCompIndices](const auto& f, uint i) {
        if (vertCompIndices.size() > 0)
            return vertCompIndices[f.vertexIndex(i)];
        else
            return f.vertexIndex(i);
    };

    return vIndex;
}

// returns a non-empty vector if the face container is not compact and the
// user wants compact indices
std::vector<uint> faceCompactIndices(const auto& mesh, bool wantCompact)
{
    return elemCompactIndices<ElemId::FACE>(mesh, wantCompact);
}

} // namespace vcl::detail

#endif // VCL_ALGORITHMS_MESH_UTILITY_H
