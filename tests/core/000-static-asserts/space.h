// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SPACE_H
#define SPACE_H

#include "space/array.h"
#include "space/bit_set.h"
#include "space/box.h"
#include "space/camera.h"
#include "space/color.h"
#include "space/image.h"
#include "space/material.h"
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
#include "space/texture_descriptor.h"

void spaceStaticAsserts()
{
    arrayStaticAsserts();
    bitSetStaticAsserts();
    boxStaticAsserts();
    cameraStaticAsserts();
    colorStaticAsserts();
    imageStaticAsserts();
    materialStaticAsserts();
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
    textureDescriptorStaticAsserts();

    using namespace vcl;

    // bitset
    static_assert(
        Serializable<BitSet<char>>, "Bitset<char> is not serializable");
}

#endif // SPACE_H
