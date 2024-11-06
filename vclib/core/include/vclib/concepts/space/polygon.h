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

#ifndef VCL_CONCEPTS_SPACE_POLYGON_H
#define VCL_CONCEPTS_SPACE_POLYGON_H

#include <vclib/types.h>

namespace vcl {

template<typename T>
concept ConstPolygonConcept = requires (T obj, const T& cObj) {
    typename T::ScalarType;
    typename T::PointType;

    cObj.DIM;

    { cObj.size() } -> std::same_as<uint>;
    { cObj.point(uint()) } -> std::same_as<const typename T::PointType&>;
    { cObj.sideLength(uint()) } -> std::same_as<typename T::ScalarType>;
    { cObj.barycenter() } -> std::same_as<typename T::PointType>;
    { cObj.perimeter() } -> std::same_as<typename T::ScalarType>;
    { cObj.area() } -> std::same_as<typename T::ScalarType>;
};

template<typename T>
concept PolygonConcept =
    ConstPolygonConcept<T> && requires (T obj, const typename T::PointType& p) {
        { obj.resize(uint()) } -> std::same_as<void>;
        { obj.reserve(uint()) } -> std::same_as<void>;
        { obj.clear() } -> std::same_as<void>;
        { obj.pushBack(p) } -> std::same_as<void>;
        { obj.point(uint()) } -> std::same_as<typename T::PointType&>;
    };

template<typename T>
concept ConstPolygon2Concept = ConstPolygonConcept<T> && T::DIM == 2;

template<typename T>
concept Polygon2Concept = ConstPolygon2Concept<T> && PolygonConcept<T>;

template<typename T>
concept ConstPolygon3Concept =
    ConstPolygonConcept<T> && T::DIM == 3 && requires (const T& cObj) {
        { cObj.normal() } -> std::same_as<typename T::PointType>;
    };

template<typename T>
concept Polygon3Concept = ConstPolygon3Concept<T> && PolygonConcept<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_POLYGON_H
