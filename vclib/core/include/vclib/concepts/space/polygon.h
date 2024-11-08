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

#include "point.h"

namespace vcl {

template<typename T>
concept PolygonConcept = requires (
    T&&                                            obj,
    typename RemoveRef<T>::PointType               p,
    typename RemoveRef<T>::PointType&              pR,
    typename RemoveRef<T>::ScalarType              s,
    std::vector<typename RemoveRef<T>::PointType>  vecP,
    std::vector<typename RemoveRef<T>::ScalarType> vecS) {
    // type requirements
    typename RemoveRef<T>::ScalarType;
    typename RemoveRef<T>::PointType;
    typename RemoveRef<T>::Iterator;
    typename RemoveRef<T>::ConstIterator;

    obj.DIM;

    // constructors
    RemoveRef<T>();
    RemoveRef<T>(vecP.begin(), vecP.end());
    RemoveRef<T>(vecP);

    { obj.size() } -> std::same_as<uint>;
    { obj.point(uint()) } -> PointConcept;
    { obj.sideLength(uint()) } -> std::same_as<decltype(s)>;
    { obj.barycenter() } -> PointConcept;
    { obj.weightedBarycenter(vecS) } -> PointConcept;
    { obj.perimeter() } -> std::same_as<decltype(s)>;
    { obj.area() } -> std::same_as<decltype(s)>;
    { obj.begin() } -> std::input_iterator;
    { obj.end() } -> std::input_iterator;

    // non const requirements
    requires vcl::IsConst<T> || requires {
        { obj.resize(uint()) } -> std::same_as<void>;
        { obj.reserve(uint()) } -> std::same_as<void>;
        { obj.clear() } -> std::same_as<void>;
        { obj.pushBack(p) } -> std::same_as<void>;
        { obj.point(uint()) } -> std::same_as<decltype(pR)>;
        { obj.begin() } -> std::output_iterator<decltype(p)>;
        { obj.end() } -> std::output_iterator<decltype(p)>;
    };
};

template<typename T>
concept Polygon2Concept = PolygonConcept<T> && RemoveRef<T>::DIM == 2;

template<typename T>
concept Polygon3Concept =
    PolygonConcept<T> && RemoveRef<T>::DIM == 3 && requires (T&& obj) {
        { obj.normal() } -> Point3Concept;
    };

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_POLYGON_H
