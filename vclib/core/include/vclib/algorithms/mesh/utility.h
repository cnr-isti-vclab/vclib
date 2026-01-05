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

    bool isCompact = mesh.template number<ELEM_ID>() ==
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
