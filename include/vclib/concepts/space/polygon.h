/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_CONCEPTS_SPACE_POLYGON_H
#define VCL_CONCEPTS_SPACE_POLYGON_H

#include <vclib/types.h>

namespace vcl {

template<typename T>
concept PolygonConcept = requires(
    T o,
    const T& co,
    const typename T::PointType& p)
{
    typename T::ScalarType;
    typename T::PointType;

    co.DIM;

    { co.size() } -> std::same_as<uint>;
    { o.point(uint()) } -> std::same_as<typename T::PointType&>;
    { co.point(uint()) } -> std::same_as<const typename T::PointType&>;
    { co.sideLength(uint()) } -> std::same_as<typename T::ScalarType>;
    { co.barycenter() } -> std::same_as<typename T::PointType>;
    { co.perimeter() } -> std::same_as<typename T::ScalarType>;
    { co.area() } -> std::same_as<typename T::ScalarType>;
};

template<typename T>
concept Polygon2Concept = PolygonConcept<T> && T::DIM == 2;

template<typename T>
concept Polygon3Concept = PolygonConcept<T> && T::DIM == 3 && requires(
    const T& co)
{
    { co.normal() } -> std::same_as<typename T::PointType>;
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_POLYGON_H
