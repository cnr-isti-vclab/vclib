/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_COMMON_H
#define MGP_MESH_FACE_COMMON_H

#include "../common/color.h"
#include "../common/info.h"
#include "../common/normal.h"
#include "../common/scalar.h"
#include "../common/optional_info.h"
#include "../common/optional_color.h"
#include "../common/optional_normal.h"
#include "../common/optional_scalar.h"

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
template<typename Scalar, int N>
using Normal = mgp::common::Normal<Scalar, N>;

template<typename Scalar>
using Normal3 = mgp::common::Normal3<Scalar>;

using Normal3f = mgp::common::Normal3f;
using Normal3d = mgp::common::Normal3d;

/** Port OptionalNormal class into face namespace **/
template <typename Scalar, int N, typename T>
using OptionalNormal = mgp::common::OptionalNormal<Scalar, N, T>;

template <typename Scalar, typename T>
using OptionalNormal3 = mgp::common::OptionalNormal3<Scalar, T>;

template <typename T>
using OptionalNormal3f = mgp::common::OptionalNormal3f<T>;

template <typename T>
using OptionalNormal3d = mgp::common::OptionalNormal3d<T>;

/** Port Scalar class into face namespace **/
template<typename T>
using Scalar = mgp::common::Scalar<T>;

using Scalarf = mgp::common::Scalarf;
using Scalard = mgp::common::Scalard;

/** Port OptionalScalar class into face namespace **/
template <typename S, typename T>
using OptionalScalar = mgp::common::OptionalScalar<S, T>;

template <typename T>
using OptionalScalarf = mgp::common::OptionalScalar<float, T>;

template <typename T>
using OptionalScalard = mgp::common::OptionalScalar<double, T>;

} // namespace face
} // namespace mgp

#endif // MGP_MESH_FACE_COMMON_H
