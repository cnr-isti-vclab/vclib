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

#ifndef VCL_RENDER_LIGHTS_DIRECTIONAL_LIGHT_H
#define VCL_RENDER_LIGHTS_DIRECTIONAL_LIGHT_H

#include <vclib/space/color.h>
#include <vclib/space/point.h>

namespace vcl {

template<typename ScalarType>
class DirectionalLight
{
    vcl::Point3<ScalarType> d = vcl::Point3<ScalarType>(0.0f, 0.0f, 1.0f);
    vcl::Color              c = vcl::Color::White;

public:
    DirectionalLight() = default;

    DirectionalLight(
        const vcl::Point3<ScalarType>& d,
        const vcl::Color&              c = vcl::Color::White) :
            d(d),
            c(c)
    {
    }

    vcl::Point3<ScalarType>& direction() { return d; }

    const vcl::Point3<ScalarType>& direction() const { return d; }

    vcl::Color& color() { return c; }

    const vcl::Color& color() const { return c; }

    void reset() { *this = {}; }
};

} // namespace vcl

#endif // VCL_RENDER_LIGHTS_DIRECTIONAL_LIGHT_H
