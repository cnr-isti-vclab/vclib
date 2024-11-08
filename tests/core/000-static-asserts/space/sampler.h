/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef SAMPLER_H
#define SAMPLER_H

#include <vclib/mesh/tmp_meshes.h>
#include <vclib/space.h>

void samplerStaticAsserts()
{
    using namespace vcl;

    using PointSamplerType  = PointSampler<>;
    using MeshSamplerType   = MeshSampler<detail::TMPSimplePolyMesh>;
    using VertexSamplerType = VertexSampler<detail::TMPSimplePolyMesh::Vertex>;
    using ConstVertexSamplerType =
        ConstVertexSampler<detail::TMPSimplePolyMesh::Vertex>;

    // point sampler
    static_assert(
        SamplerConcept<PointSamplerType>,
        "PointSampler does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<const PointSamplerType>,
        "const PointSampler does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<PointSamplerType&>,
        "PointSampler& does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<const PointSamplerType&>,
        "const PointSampler& does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<PointSamplerType&&>,
        "PointSampler&& does not satisfy the SamplerConcept");

    // mesh sampler
    static_assert(
        SamplerConcept<MeshSamplerType>,
        "MeshSampler does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<const MeshSamplerType>,
        "const MeshSampler does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<MeshSamplerType&>,
        "MeshSampler& does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<const MeshSamplerType&>,
        "const MeshSampler& does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<MeshSamplerType&&>,
        "MeshSampler&& does not satisfy the SamplerConcept");

    // vertex sampler
    static_assert(
        SamplerConcept<VertexSamplerType>,
        "VertexSampler does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<const VertexSamplerType>,
        "const VertexSampler does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<VertexSamplerType&>,
        "VertexSampler& does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<const VertexSamplerType&>,
        "const VertexSampler& does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<VertexSamplerType&&>,
        "VertexSampler&& does not satisfy the SamplerConcept");

    // const vertex sampler
    static_assert(
        SamplerConcept<ConstVertexSamplerType>,
        "ConstVertexSampler does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<const ConstVertexSamplerType>,
        "const ConstVertexSampler does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<ConstVertexSamplerType&>,
        "ConstVertexSampler& does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<const ConstVertexSamplerType&>,
        "const ConstVertexSampler& does not satisfy the SamplerConcept");
    static_assert(
        SamplerConcept<ConstVertexSamplerType&&>,
        "ConstVertexSampler&& does not satisfy the SamplerConcept");

    // TODO: fix these asserts
    // ranges
    //     static_assert(
    //        std::ranges::range<VertexSampler<detail::TMPSimplePolyMesh::Vertex>>,
    //        "");

    //    VertexSampler<detail::TMPSimplePolyMesh::Vertex> v;
    //    auto it = std::ranges::begin(v);
}

#endif // SAMPLER_H
