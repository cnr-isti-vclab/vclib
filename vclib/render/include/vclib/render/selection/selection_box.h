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

#ifndef VCL_RENDER_SELECTION_SELECTION_BOX_H
#define VCL_RENDER_SELECTION_SELECTION_BOX_H

#include <vclib/algorithms/core.h>
#include <vclib/space/core.h>

#include <array>
#include <optional>

namespace vcl {
class SelectionBox
{
    using ArrayType = std::array<std::optional<Point2d>, 2>;

    ArrayType mPoints = {std::nullopt, std::nullopt};

public:
    SelectionBox() {}

    SelectionBox(ArrayType arr) { mPoints = arr; }

    Box2d box2d() const
    {
        Box2d b;
        if (mPoints[0].has_value() && mPoints[1].has_value()) {
            b.add(mPoints[0].value());
            b.add(mPoints[1].value());
        }
        return b;
    }

    /**
     * @brief Sets the first point of the box
     *
     * @param[in] p: The point in question
     */
    void setAnchor(Point2d p) { mPoints[0] = std::make_optional(p); }

    /**
     * @brief Sets the second point of the box
     *
     * @param[in] p: The point in question
     */
    void setExtent(Point2d p) { mPoints[1] = std::make_optional(p); }

    /**
     * @brief Sets both of the box's points to null
     */
    void nullAll()
    {
        mPoints[0] = std::nullopt;
        mPoints[1] = std::nullopt;
    }
};

} // namespace vcl

#endif // VCL_RENDER_SELECTION_SELECTION_BOX_H
