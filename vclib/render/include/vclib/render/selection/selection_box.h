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

#ifndef VCL_SELECTION_BOX_H
#define VCL_SELECTION_BOX_H

#include <vclib/space/core/point.h>

#include <array>
#include <optional>
#include <utility>

namespace vcl {
class SelectionBox
{
    using ArrayType = std::array<std::optional<Point2d>, 2>;
    std::array<std::optional<Point2d>, 2> mPoints = {std::nullopt, std::nullopt};

    void toMinAndMax() {
        if (!mPoints[0].has_value() || !mPoints[1].has_value()) {
            return;
        }
        Point2d p1 = mPoints[0].value();
        Point2d p2 = mPoints[1].value();
        Point2d newp1 = {min(p1.x(), p2.x()), min(p1.y(), p2.y())};
        Point2d newp2 = {max(p1.x(), p2.x()), max(p1.y(), p2.y())};
        mPoints[0] = std::make_optional(newp1);
        mPoints[1] = std::make_optional(newp2);
    }

public:
    SelectionBox() {}

    SelectionBox(ArrayType arr) { mPoints = arr; }

    std::optional<Point2d> get1() {
        return mPoints[0];
    }

    std::optional<Point2d> get2() {
        return mPoints[1];
    }

    void set1(Point2d p) {
        mPoints[0] = std::make_optional(p);
        toMinAndMax();
    }

    void set2(Point2d p) {
        mPoints[1] = std::make_optional(p);
        toMinAndMax();
    }

    void null1() {
        mPoints[0] = std::nullopt;
    }

    void null2() {
        mPoints[1] = std::nullopt;
    }

    void nullAll() {
        null1();
        null2();
    }

    bool anyNull() {
        return !(mPoints[0].has_value() && mPoints[1].has_value());
    }

    bool allNull() {
        return !(mPoints[0].has_value() || mPoints[1].has_value());
    }

    bool anyValue() {
        return !allNull();
    }

    bool allValue() {
        return !anyNull();
    }
};
} // namespace vcl

#endif
