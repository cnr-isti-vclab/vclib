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

#ifndef SPACE_H
#define SPACE_H

#include "space/array.h"
#include "space/bit_set.h"
#include "space/box.h"
#include "space/color.h"
#include "space/image.h"
#include "space/matrix.h"
#include "space/plane.h"
#include "space/point.h"
#include "space/point_sampler.h"
#include "space/polygon.h"
#include "space/principal_curvature.h"
#include "space/segment.h"
#include "space/sphere.h"
#include "space/tex_coord.h"
#include "space/tex_coord_indexed.h"
#include "space/texture.h"

void spaceStaticAsserts()
{
    arrayStaticAsserts();
    bitSetStaticAsserts();
    boxStaticAsserts();
    colorStaticAsserts();
    imageStaticAsserts();
    matrixStaticAsserts();
    planeStaticAsserts();
    pointStaticAsserts();
    pointSamplerStaticAsserts();
    polygonStaticAsserts();
    principalCurvatureStaticAsserts();
    segmentStaticAsserts();
    sphereStaticAsserts();
    texCoordStaticAsserts();
    texCoordIndexedStaticAsserts();
    textureStaticAsserts();

    using namespace vcl;

    // bitset
    static_assert(
        Serializable<BitSet<char>>, "Bitset<char> is not serializable");
}

#endif // SPACE_H
