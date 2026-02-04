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
#include <iostream>
#include <optional>
#include <sstream>
#include <utility>

namespace vcl {
class SelectionBox
{
    using ArrayType = std::array<std::optional<Point2d>, 2>;
    std::array<std::optional<Point2d>, 2> mPoints = {
        std::nullopt,
        std::nullopt};


    /**
     * @param[in] box: Another box
     * 
     * @return True if this box and the other box have intersection on the x coordinate, false otherwise
     */
    bool xIntersectionExists(const SelectionBox& box) const&
    {
        return
            // xMin of first box is before xMax of second box and xMax of first
            // box is after xMin of second box
            (get1().value().x() <= box.get2().value().x() &&
             get2().value().x() >= box.get1().value().x())
            // OR (xMin of second box is before xMax of first box and xMax of
            // second box is after xMin of first box)
            || (box.get1().value().x() <= get2().value().x() &&
                box.get2().value().x() >= get1().value().x());
    }

    /**
     * @param[in] box: Another box
     * 
     * @return True if this box and the other box have intersection on the y coordinate, false otherwise
     */
    bool yIntersectionExists(const SelectionBox& box) const&
    {
        return
            // yMin of first box is before yMax of second box and yMax of first
            // box is after yMin of second box
            (get1().value().y() <= box.get2().value().y() &&
             get2().value().y() >= box.get1().value().y())
            // OR (yMin of second box is before yMax of first box and yMax of
            // second box is after yMin of first box)
            || (box.get1().value().y() <= get2().value().y() &&
                box.get2().value().y() >= get1().value().y());
    }

    /**
     * @param[in] box: Another box
     * 
     * @return True if this box and the other box have intersection, false otherwise
     */
    bool intersectionExists(const SelectionBox& box) const&
    {
        if (anyNull() || box.anyNull()) {
            return false;
        }
        // Boxes intersect if their coordinates interect both in the X and Y
        // axis
        return xIntersectionExists(box) && yIntersectionExists(box);
    }

public:
    SelectionBox() {}

    SelectionBox(ArrayType arr) { mPoints = arr; }

    /**
     * @brief Attempts to calculate a new SelectionBox that has the minimum coordinates on the first point
     * and the maximum coordinates on the second point
     * 
     * @return The calculated SelectionBox if neither point was null, copy of self otherwise
     */
    SelectionBox toMinAndMax() const
    {
        if (anyNull()) {
            return *this;
        }
        Point2d p1    = mPoints[0].value();
        Point2d p2    = mPoints[1].value();
        Point2d newp1 = {min(p1.x(), p2.x()), min(p1.y(), p2.y())};
        Point2d newp2 = {max(p1.x(), p2.x()), max(p1.y(), p2.y())};
        return SelectionBox(
            ArrayType {std::make_optional(newp1), std::make_optional(newp2)});
    }

    /**
     * @brief Performs the intersection between two SelectionBox(es)
     * 
     * @return The intersection between this box and another box
     */
    SelectionBox intersect(const SelectionBox& other) const&
    {
        SelectionBox b1 = toMinAndMax();
        SelectionBox b2 = other.toMinAndMax();
        SelectionBox ret;
        if (!b1.intersectionExists(b2)) {
            return ret;
        }
        // We exclude the case of NO INTERSECTION, therefore:
        //     - minimum point of the intersection is comprised of the biggest
        //     coordinates of the two boxes' minimum points
        //     - maximum point of the intersection is comprised of the smallest
        //     coordinates of the two boxes' maximum points
        ret.set1(
            Point2d {
                std::max(b1.get1().value().x(), b2.get1().value().x()),
                std::max(b1.get1().value().y(), b2.get1().value().y())});
        ret.set2(
            Point2d {
                std::min(b1.get2().value().x(), b2.get2().value().x()),
                std::min(b1.get2().value().y(), b2.get2().value().y())});
        return ret;
    }

    std::optional<Point2d> get1() const { return mPoints[0]; }

    std::optional<Point2d> get2() const { return mPoints[1]; }

    /**
     * @brief Sets the first point of the box
     * 
     * @param[in] p: The point in question 
     */
    void set1(Point2d p) { mPoints[0] = std::make_optional(p); }

    /**
     * @brief Sets the second point of the box
     * 
     * @param[in] p: The point in question
     */
    void set2(Point2d p) { mPoints[1] = std::make_optional(p); }

    /**
     * @brief Sets the first point of the box to null
     */
    void null1() { mPoints[0] = std::nullopt; }

    /**
     * @brief Sets the second point of the box to null
     */
    void null2() { mPoints[1] = std::nullopt; }

    /**
     * @brief Sets both of the box's points to null
     */
    void nullAll()
    {
        null1();
        null2();
    }

    /**
     * @return True if any of the box's points are null, false otherwise
     */
    bool anyNull() const
    {
        return !(mPoints[0].has_value() && mPoints[1].has_value());
    }

    /**
     * @return True if all of the box's points are null, false otherwise
     */
    bool allNull() const
    {
        return !(mPoints[0].has_value() || mPoints[1].has_value());
    }

    /**
     * @return True if any of the box's points have value, false otherwise
     */
    bool anyValue() const { return !allNull(); }

    /**
     * @return True if all of the box's points have value, false otherwise
     */
    bool allValue() const { return !anyNull(); }

    std::array<float, 4> asFloatArray() const
    {
        std::array<float, 4> arr;
        arr[0] = float(get1().value().x());
        arr[1] = float(get1().value().y());
        arr[2] = float(get2().value().x());
        arr[3] = float(get2().value().y());
        return arr;
    }

    // 0----------2
    // |          |
    // |          |
    // |          |
    // |          |
    // 1----------3
    /**
     * @return The positions of the vertices of the selection box in Screen Space
     */
    std::array<float, 8> vertexPositions() const
    {
        SelectionBox         pts = this->toMinAndMax();
        std::array<float, 8> arr;
        arr[0] = pts.mPoints[0].value().x(); // xmin
        arr[1] = pts.mPoints[0].value().y(); // ymin

        arr[2] = pts.mPoints[0].value().x(); // xmin
        arr[3] = pts.mPoints[1].value().y(); // ymax

        arr[4] = pts.mPoints[1].value().x(); // xmax
        arr[5] = pts.mPoints[0].value().y(); // ymin

        arr[6] = pts.mPoints[1].value().x(); // xmax
        arr[7] = pts.mPoints[1].value().y(); // ymax
        return arr;
    }

    constexpr static std::array<uint, 6> triangleIndices()
    {
        return std::array<uint, 6> {2, 3, 0, 3, 1, 0};
    }
};

} // namespace vcl

#endif
