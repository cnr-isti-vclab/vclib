// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_CORE_MATRIX_AFFINE_H
#define VCL_SPACE_CORE_MATRIX_AFFINE_H

#include <Eigen/Core>

namespace vcl {

template<typename Scalar>
using Affine3 = Eigen::Transform<Scalar, 3, Eigen::Affine>;

using Affine3f = Affine3<float>;
using Affine3d = Affine3<double>;

} // namespace vcl

#endif // VCL_SPACE_CORE_MATRIX_AFFINE_H
