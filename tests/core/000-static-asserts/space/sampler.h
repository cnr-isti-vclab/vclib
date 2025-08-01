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

#ifndef SAMPLER_H
#define SAMPLER_H

#include <vclib/mesh.h>
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

    static_assert(
        std::ranges::range<VertexSampler<detail::TMPSimplePolyMesh::Vertex>>,
        "A VertexSampler is not a range");
}

#endif // SAMPLER_H
