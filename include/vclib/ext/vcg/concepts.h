/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_EXT_VCG_CONCEPTS_H
#define VCL_EXT_VCG_CONCEPTS_H

#include <vclib/types.h>

#include <vcg/space/point2.h>
#include <vcg/space/point3.h>
#include <vcg/space/point4.h>

namespace vcl::vc {

template<typename T>
concept Point2Concept = T::Dimension == 2 && requires (T a) {
    // clang-format off
    { a.X() } -> std::convertible_to<float>;
    { a.Y() } -> std::convertible_to<float>;
    // clang-format on
};

template<typename T>
concept Point3Concept = T::Dimension == 3 && requires (T a) {
    // clang-format off
    { a.X() } -> std::convertible_to<float>;
    { a.Y() } -> std::convertible_to<float>;
    { a.Z() } -> std::convertible_to<float>;
    // clang-format on
};

template<typename T>
concept Point4Concept = T::Dimension == 4 && requires (T a) {
    // clang-format off
    { a.X() } -> std::convertible_to<float>;
    { a.Y() } -> std::convertible_to<float>;
    { a.Z() } -> std::convertible_to<float>;
    { a.W() } -> std::convertible_to<float>;
    // clang-format on
};

} // namespace vcl::vc

#endif // VCL_EXT_VCG_CONCEPTS_H
