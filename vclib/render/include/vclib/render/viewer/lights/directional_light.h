// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_VIEWER_LIGHTS_DIRECTIONAL_LIGHT_H
#define VCL_RENDER_VIEWER_LIGHTS_DIRECTIONAL_LIGHT_H

#include <vclib/space/core/color.h>
#include <vclib/space/core/point.h>

namespace vcl {

template<typename Scalar>
class DirectionalLight
{
    vcl::Point3<Scalar> mDir   = vcl::Point3<Scalar>(0.0f, 0.0f, 1.0f);
    vcl::Color          mColor = vcl::Color::White;

public:
    using PointType = vcl::Point3<Scalar>;

    DirectionalLight() = default;

    DirectionalLight(
        const PointType&  d,
        const vcl::Color& c = vcl::Color::White) : mDir(d), mColor(c)
    {
    }

    PointType& direction() { return mDir; }

    const PointType& direction() const { return mDir; }

    vcl::Color& color() { return mColor; }

    const vcl::Color& color() const { return mColor; }

    void reset() { *this = {}; }
};

} // namespace vcl

#endif // VCL_RENDER_VIEWER_LIGHTS_DIRECTIONAL_LIGHT_H
