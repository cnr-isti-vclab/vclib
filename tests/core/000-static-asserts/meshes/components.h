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

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "components/bounding_box.h"
#include "components/color.h"
#include "components/coordinate.h"
#include "components/mark.h"
#include "components/name.h"
#include "components/normal.h"
#include "components/principal_curvature.h"
#include "components/quality.h"

void componentsStaticAsserts()
{
    boundingBoxComponentStaticAsserts();
    colorComponentStaticAsserts();
    coordinateComponentStaticAsserts();
    markComponentStaticAsserts();
    nameComponentStaticAsserts();
    normalComponentStaticAsserts();
    principalCurvatureComponentStaticAsserts();
    qualityComponentStaticAsserts();
}

#endif // COMPONENTS_H
