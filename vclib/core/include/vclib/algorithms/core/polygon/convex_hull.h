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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_ALGORITHMS_CORE_POLYGON_CONVEX_HULL_H
#define VCL_ALGORITHMS_CORE_POLYGON_CONVEX_HULL_H

#include "core2.h"

namespace vcl {

namespace detail {

/**
 * @brief Graham scan on a collection of points (upper or lower)
 * @param[in] first: First iterator of the input container
 * @param[in] end: End iterator of the input container
 * @param[out] outContainer: Output container for the convex hull
 */
template<Point2IteratorConcept InputIterator, typename OutputContainer>
void grahamScanOnContainer(
    const InputIterator first,
    const InputIterator end,
    OutputContainer&    outContainer)
{
    // Iterators
    InputIterator it1;
    InputIterator it2;
    InputIterator it3;

    InputIterator last;

    // Stack of iterator to the vector (it is going to be the output)
    std::vector<InputIterator> stack;

    // Last points to the last element of the collection
    last = end;
    last--;

    assert(first != end);
    assert(std::next(first) != end);
    assert(*first != *last);

    // Add first and last to the results
    stack.push_back(last);
    stack.push_back(first);

    // Initializing t1
    it1 = first;

    // Skip points not at the left of first-last segment
    do {
        it1++;
    } while (it1 != last && !areCounterClockwise(*first, *last, *it1));

    if (it1 != last) {
        // Initialize stack with the first element which could be in the convex
        // hull
        stack.push_back(it1);

        it2 = it1;

        // Iterator to stack, pointing to the previous element of the back
        // element
        typename std::vector<InputIterator>::reverse_iterator stackItR =
            stack.rbegin();
        stackItR++;

        it3 = *stackItR;

        for (it1++; it1 != last; it1++) {
            // Skip point for which last element is on the right of
            // it1 - it2 segment
            if (areCounterClockwise(*it1, *it2, *last)) {
                while (!areCounterClockwise(*it2, *it3, *it1)) {
                    // Pop from stack
                    stack.pop_back();

                    // Setting other iterators
                    it2 = it3;

                    stackItR = stack.rbegin();
                    stackItR++;

                    it3 = *stackItR;

                    assert(stack.size() >= 2);
                }

                // Add to stack it1 and setting other iterators
                stack.push_back(it1);
                it3 = it2;
                it2 = it1;
            }
        }
    }

    // Writing on the output iterator the results
    typename std::vector<InputIterator>::iterator sIter = stack.begin();
    for (++sIter; sIter != stack.end(); ++sIter) {
        outContainer.pushBack(**sIter);
    }
}

} // namespace detail

/**
 * @brief Get the 2D convex hull using Graham scan algorithm on a set of points.
 *
 * @tparam InputIterator Iterator type of the input container of points. It must
 * Iterate over a range of elements that satisfy the Point2Concept.
 * @param[in] first First iterator of the input container of points.
 * @param[in] end End iterator of the input container of points.
 * @return A polygon representing the convex hull of the input points.
 *
 * @ingroup core_polygon
 */
template<Point2IteratorConcept InputIterator>
auto convexHull(InputIterator first, InputIterator end)
{
    using PointType = std::decay_t<decltype(*first)>;

    Polygon<PointType> convexHull;

    // If the container is empty
    if (first == end)
        return convexHull;

    // Sort the points
    std::vector<PointType> sortedPoints(first, end);
    std::sort(sortedPoints.begin(), sortedPoints.end());

    // If the is composed by 1 points (or more than 1 of the same point)
    if (*(sortedPoints.begin()) == *(sortedPoints.rbegin())) {
        convexHull.pushBack(*(sortedPoints.begin()));

        return convexHull;
    }

    // Graham scan on upper and lower convex hull
    detail::grahamScanOnContainer(
        sortedPoints.begin(), sortedPoints.end(), convexHull);
    detail::grahamScanOnContainer(
        sortedPoints.rbegin(), sortedPoints.rend(), convexHull);

    convexHull.pushBack(convexHull.point(0));
    std::reverse(convexHull.begin(), convexHull.end());
    convexHull.resize(convexHull.size() - 1);

    return convexHull;
}

template<Range InputContainer>
auto convexHull(const InputContainer& container)
    requires Point2Concept<std::ranges::range_value_t<InputContainer>>
{
    return convexHull(container.begin(), container.end());
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_POLYGON_CONVEX_HULL_H
