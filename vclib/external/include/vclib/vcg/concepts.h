// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_VCG_CONCEPTS_H
#define VCL_VCG_CONCEPTS_H

#include <vclib/base.h>

#include <vcg/space/point2.h>
#include <vcg/space/point3.h>
#include <vcg/space/point4.h>

namespace vcl::vc {

template<typename T>
concept Point2Concept = T::Dimension == 2 && requires (T a) {
    { a.X() } -> std::convertible_to<float>;
    { a.Y() } -> std::convertible_to<float>;
};

template<typename T>
concept Point3Concept = T::Dimension == 3 && requires (T a) {
    { a.X() } -> std::convertible_to<float>;
    { a.Y() } -> std::convertible_to<float>;
    { a.Z() } -> std::convertible_to<float>;
};

template<typename T>
concept Point4Concept = T::Dimension == 4 && requires (T a) {
    { a.X() } -> std::convertible_to<float>;
    { a.Y() } -> std::convertible_to<float>;
    { a.Z() } -> std::convertible_to<float>;
    { a.W() } -> std::convertible_to<float>;
};

} // namespace vcl::vc

#endif // VCL_VCG_CONCEPTS_H
