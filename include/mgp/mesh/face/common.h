/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_COMMON_H
#define MGP_MESH_FACE_COMMON_H

#include "../common/color.h"
#include "../common/info.h"
#include "../common/normal.h"
#include "../common/optional_info.h"
#include "../common/optional_color.h"

namespace mgp {
namespace face {

/** Port Info class into face namespace **/
using Info = mgp::common::Info;

/** Port OptionalInfo class into face namespace **/
template <typename T>
using OptionalInfo = mgp::common::OptionalInfo<T>;

/** Port Color class into face namespace **/
using Color = mgp::common::Color;

/** Port OptionalColor class into face namespace **/
template <typename T>
using OptionalColor = mgp::common::OptionalColor<T>;

/** Port Normal classes into face namespace **/
template<class Scalar, int N>
using Normal = mgp::common::Normal<Scalar, N>;

template<class Scalar>
using Normal3 = mgp::common::Normal3<Scalar>;

using Normal3f = mgp::common::Normal3f;
using Normal3d = mgp::common::Normal3d;

} // namespace face
} // namespace mgp

#endif // MGP_MESH_FACE_COMMON_H
