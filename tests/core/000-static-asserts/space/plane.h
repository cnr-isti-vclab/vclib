// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PLANE_H
#define PLANE_H

#include <vclib/space.h>

void planeStaticAsserts()
{
    using namespace vcl;

    using PlaneType = Plane<float>;

    // plane
    static_assert(
        PlaneConcept<PlaneType>, "Plane does not satisfy the PlaneConcept");
    static_assert(
        PlaneConcept<const PlaneType>,
        "const Plane does not satisfy the PlaneConcept");
    static_assert(
        PlaneConcept<const PlaneType>,
        "const Plane does not satisfy the PlaneConcept");
    static_assert(
        PlaneConcept<PlaneType&>, "Plane& does not satisfy the PlaneConcept");
    static_assert(
        PlaneConcept<const PlaneType&>,
        "const Plane& does not satisfy the PlaneConcept");
    static_assert(
        PlaneConcept<PlaneType&&>, "Plane&& does not satisfy the PlaneConcept");
    static_assert(Serializable<PlaneType>, "Plane is not serializable");
}

#endif // PLANE_H
