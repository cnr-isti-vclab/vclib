/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_COMMON_H
#define MGP_MESH_VERTEX_COMMON_H

#include "../common/bit_flags.h"
#include "../common/color.h"
#include "../common/custom_attributes.h"
#include "../common/mutable_bit_flags.h"
#include "../common/normal.h"
#include "../common/scalar.h"
#include "../common/optional_color.h"
#include "../common/optional_mutable_bit_flags.h"
#include "../common/optional_normal.h"
#include "../common/optional_scalar.h"

namespace mgp::vert {

/** Port OptionalInfo class into vert namespace **/
template <typename T>
using OptionalInfo = mgp::common::OptionalInfo<T>;

template <typename T>
bool constexpr hasOptionalInfo() {return mgp::common::hasOptionalInfo<T>();};

/** Port BitFlags class into vert namespace **/
using BitFlags = mgp::common::BitFlags;

template <typename T>
bool constexpr hasBitFlags() {return mgp::common::hasBitFlags<T>();};

/** Port OptionalMutableBitFlags class into vert namespace **/
template <typename T>
using OptionalMutableBitFlags = mgp::common::OptionalMutableBitFlags<T>;

template <typename T>
bool constexpr hasOptionalMutableBitFlags() {return mgp::common::hasOptionalMutableBitFlags<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalMutableBitFlags = common::ReturnIfHasOptionalMutableBitFlags<U, T>;

/** Port Color class into vert namespace **/
using Color = mgp::common::Color;

template <typename T>
bool constexpr hasColor() {return mgp::common::hasColor<T>();};

/** Port OptionalColor class into vert namespace **/
template <typename T>
using OptionalColor = mgp::common::OptionalColor<T>;

template <typename T>
bool constexpr hasOptionalColor() {return mgp::common::hasOptionalColor<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalColor = common::ReturnIfHasOptionalColor<U, T>;

/** Port Normal classes into vert namespace **/
template<class Scalar, int N>
using Normal = mgp::common::Normal<Scalar, N>;

template<class Scalar>
using Normal3 = mgp::common::Normal3<Scalar>;

using Normal3f = mgp::common::Normal3f;
using Normal3d = mgp::common::Normal3d;

template <typename T>
bool constexpr hasNormal() {return mgp::common::hasNormal<T>();};

/** Port OptionalNormal class into vert namespace **/
template <typename Scalar, int N, typename T>
using OptionalNormal = mgp::common::OptionalNormal<Scalar, N, T>;

template <typename Scalar, typename T>
using OptionalNormal3 = mgp::common::OptionalNormal3<Scalar, T>;

template <typename T>
using OptionalNormal3f = mgp::common::OptionalNormal3f<T>;

template <typename T>
using OptionalNormal3d = mgp::common::OptionalNormal3d<T>;

template <typename T>
bool constexpr hasOptionalNormal() {return mgp::common::hasOptionalNormal<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalNormal = common::ReturnIfHasOptionalNormal<U, T>;

/** Port Scalar class into vert namespace **/
template<typename T>
using Scalar = mgp::common::Scalar<T>;

using Scalarf = mgp::common::Scalarf;
using Scalard = mgp::common::Scalard;

template <typename T>
bool constexpr hasScalar() {return mgp::common::hasScalar<T>();};

/** Port OptionalScalar class into vert namespace **/
template <typename S, typename T>
using OptionalScalar = mgp::common::OptionalScalar<S, T>;

template <typename T>
using OptionalScalarf = mgp::common::OptionalScalar<float, T>;

template <typename T>
using OptionalScalard = mgp::common::OptionalScalar<double, T>;

template <typename T>
bool constexpr hasOptionalScalar() {return mgp::common::hasOptionalScalar<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalScalar = common::ReturnIfHasOptionalScalar<U, T>;

/** Port CustomAttributes class into vert namespace **/
template <typename T>
using CustomAttributes = mgp::common::CustomAttributes<T>;

template <typename T>
bool constexpr hasCustomAttributes() {return mgp::common::hasCustomAttributes<T>();};

template<typename U, typename T>
using ReturnIfHasCustomAttributes = common::ReturnIfHasCustomAttributes<U, T>;

} // namespace mgp::vert

#endif // MGP_MESH_VERTEX_COMMON_H
