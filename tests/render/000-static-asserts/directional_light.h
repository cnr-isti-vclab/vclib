// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
