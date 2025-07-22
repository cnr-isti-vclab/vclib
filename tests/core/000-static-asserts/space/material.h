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

#ifndef MATERIAL_H
#define MATERIAL_H

#include <vclib/space.h>

void materialStaticAsserts()
{
    using namespace vcl;

    // material
    static_assert(
        MaterialConcept<Material>,
        "Material does not satisfy the MaterialConcept");
    static_assert(
        MaterialConcept<const Material>,
        "const Material does not satisfy the MaterialConcept");
    static_assert(
        MaterialConcept<Material&>,
        "Material& does not satisfy the MaterialConcept");
    static_assert(
        MaterialConcept<const Material&>,
        "const Material& does not satisfy the MaterialConcept");
    static_assert(
        MaterialConcept<Material&&>,
        "Material&& does not satisfy the MaterialConcept");

    static_assert(Serializable<Material>, "Material is not serializable");
}

#endif // MATERIAL_H
