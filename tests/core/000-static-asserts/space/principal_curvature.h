// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PRINCIPAL_CURVATURE_H
#define PRINCIPAL_CURVATURE_H

#include <vclib/space.h>

void principalCurvatureStaticAsserts()
{
    using namespace vcl;

    using PrincipalCurvatureType = PrincipalCurvature<double>;

    // PrincipalCurvature
    static_assert(
        PrincipalCurvatureConcept<PrincipalCurvatureType>,
        "PrincipalCurvature does not satisfy the PrincipalCurvatureConcept");
    static_assert(
        PrincipalCurvatureConcept<const PrincipalCurvatureType>,
        "const PrincipalCurvature does not satisfy the "
        "PrincipalCurvatureConcept");
    static_assert(
        PrincipalCurvatureConcept<PrincipalCurvatureType&>,
        "PrincipalCurvature& does not satisfy the PrincipalCurvatureConcept");
    static_assert(
        PrincipalCurvatureConcept<const PrincipalCurvatureType&>,
        "const PrincipalCurvature& does not satisfy the "
        "PrincipalCurvatureConcept");
    static_assert(
        PrincipalCurvatureConcept<PrincipalCurvatureType&&>,
        "PrincipalCurvature&& does not satisfy the PrincipalCurvatureConcept");
    static_assert(
        Serializable<PrincipalCurvatureType>,
        "PrincipalCurvature is not serializable");
}

#endif // PRINCIPAL_CURVATURE_H
