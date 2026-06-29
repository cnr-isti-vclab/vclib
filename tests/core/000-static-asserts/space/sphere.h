// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SPHERE_H
#define SPHERE_H

#include <vclib/space.h>

void sphereStaticAsserts()
{
    using namespace vcl;

    using SphereType = Sphered;

    // sphere
    static_assert(
        SphereConcept<SphereType>, "Sphere does not satisfy the SphereConcept");
    static_assert(
        SphereConcept<const SphereType>,
        "const Sphere does not satisfy the SphereConcept");
    static_assert(
        SphereConcept<SphereType&>,
        "Sphere& does not satisfy the SphereConcept");
    static_assert(
        SphereConcept<const SphereType&>,
        "const Sphere& does not satisfy the SphereConcept");
    static_assert(
        SphereConcept<SphereType&&>,
        "Sphere&& does not satisfy the SphereConcept");
}

#endif // SPHERE_H
