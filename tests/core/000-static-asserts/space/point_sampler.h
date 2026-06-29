// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef POINT_SAMPLER_H
#define POINT_SAMPLER_H

#include <vclib/mesh.h>
#include <vclib/space.h>

void pointSamplerStaticAsserts()
{
    using namespace vcl;

    using PointSamplerType = PointSampler<>;

    // point sampler
    static_assert(
        PointSamplerConcept<PointSamplerType>,
        "PointSampler does not satisfy the PointSamplerConcept");
    static_assert(
        PointSamplerConcept<const PointSamplerType>,
        "const PointSampler does not satisfy the PointSamplerConcept");
    static_assert(
        PointSamplerConcept<PointSamplerType&>,
        "PointSampler& does not satisfy the PointSamplerConcept");
    static_assert(
        PointSamplerConcept<const PointSamplerType&>,
        "const PointSampler& does not satisfy the PointSamplerConcept");
    static_assert(
        PointSamplerConcept<PointSamplerType&&>,
        "PointSampler&& does not satisfy the PointSamplerConcept");
}

#endif // POINT_SAMPLER_H
