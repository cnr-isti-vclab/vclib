// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
