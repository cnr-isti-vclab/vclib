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
}

#endif // PRINCIPAL_CURVATURE_H
