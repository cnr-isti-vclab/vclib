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

#ifndef SPACE_H
#define SPACE_H

#include "space/array.h"
#include "space/box.h"
#include "space/color.h"
#include "space/image.h"
#include "space/matrix.h"
#include "space/plane.h"
#include "space/point.h"
#include "space/polygon.h"
#include "space/principal_curvature.h"
#include "space/sampler.h"
#include "space/segment.h"
#include "space/sphere.h"
#include "space/tex_coord.h"
#include "space/texture.h"

void spaceStaticAsserts()
{
    arrayStaticAsserts();
    boxStaticAsserts();
    colorStaticAsserts();
    imageStaticAsserts();
    matrixStaticAsserts();
    planeStaticAsserts();
    pointStaticAsserts();
    polygonStaticAsserts();
    principalCurvatureStaticAsserts();
    samplerStaticAsserts();
    segmentStaticAsserts();
    sphereStaticAsserts();
    texCoordStaticAsserts();
    textureStaticAsserts();

    using namespace vcl;

    // bitset
    static_assert(
        Serializable<BitSet<char>>, "Bitset<char> is not serializable");
}

#endif // SPACE_H
