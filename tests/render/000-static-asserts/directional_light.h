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

#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <vclib/render/concepts/directional_light.h>
#include <vclib/render/viewer/lights/directional_light.h>

void directionalLightStaticAsserts()
{
    using namespace vcl;

    using DirectionalLightd = DirectionalLight<double>;

    static_assert(
        DirectionalLightConcept<DirectionalLightd>,
        "DirectionalLight does not satisfy the DirectionalLightConcept");
    static_assert(
        DirectionalLightConcept<const DirectionalLightd>,
        "const DirectionalLight does not satisfy the DirectionalLightConcept");
    static_assert(
        DirectionalLightConcept<DirectionalLightd&>,
        "DirectionalLight& does not satisfy the DirectionalLightConcept");
    static_assert(
        DirectionalLightConcept<const DirectionalLightd&>,
        "const DirectionalLight& does not satisfy the DirectionalLightConcept");
    static_assert(
        DirectionalLightConcept<DirectionalLightd&&>,
        "DirectionalLight&& does not satisfy the DirectionalLightConcept");
}

#endif // DIRECTIONAL_LIGHT_H
