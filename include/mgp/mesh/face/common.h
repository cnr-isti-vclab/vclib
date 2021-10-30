/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_COMMON_H
#define MGP_MESH_FACE_COMMON_H

#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components_optional/custom_components.h"
#include "../components/mutable_bit_flags.h"
#include "../components/normal.h"
#include "../components/scalar.h"
#include "../components_optional/optional_color.h"
#include "../components_optional/optional_mutable_bit_flags.h"
#include "../components_optional/optional_normal.h"
#include "../components_optional/optional_scalar.h"

namespace mgp::face {

/** Port OptionalInfo class into face namespace **/
template <typename T>
using OptionalInfo = mgp::components::OptionalInfo<T>;

template <typename T>
bool constexpr hasOptionalInfo() {return mgp::components::hasOptionalInfo<T>();};

/** Port BitFlags class into face namespace **/
using BitFlags = mgp::components::BitFlags;

template <typename T>
bool constexpr hasBitFlags() {return mgp::components::hasBitFlags<T>();};

/** Port OptionalMutableBitFlags class into face namespace **/
template <typename T>
using OptionalMutableBitFlags = mgp::components::OptionalMutableBitFlags<T>;

template <typename T>
bool constexpr hasOptionalMutableBitFlags() {return mgp::components::hasOptionalMutableBitFlags<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalMutableBitFlags = components::ReturnIfHasOptionalMutableBitFlags<U, T>;

/** Port Color class into face namespace **/
using Color = mgp::components::Color;

template <typename T>
bool constexpr hasColor() {return mgp::components::hasColor<T>();};

/** Port OptionalColor class into face namespace **/
template <typename T>
using OptionalColor = mgp::components::OptionalColor<T>;

template <typename T>
bool constexpr hasOptionalColor() {return mgp::components::hasOptionalColor<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalColor = components::ReturnIfHasOptionalColor<U, T>;

/** Port Normal classes into face namespace **/
template<typename Scalar, int N>
using Normal = mgp::components::Normal<Scalar, N>;

template<typename Scalar>
using Normal3 = mgp::components::Normal3<Scalar>;

using Normal3f = mgp::components::Normal3f;
using Normal3d = mgp::components::Normal3d;

template <typename T>
bool constexpr hasNormal() {return mgp::components::hasNormal<T>();};

/** Port OptionalNormal class into face namespace **/
template <typename Scalar, int N, typename T>
using OptionalNormal = mgp::components::OptionalNormal<Scalar, N, T>;

template <typename Scalar, typename T>
using OptionalNormal3 = mgp::components::OptionalNormal3<Scalar, T>;

template <typename T>
using OptionalNormal3f = mgp::components::OptionalNormal3f<T>;

template <typename T>
using OptionalNormal3d = mgp::components::OptionalNormal3d<T>;

template <typename T>
bool constexpr hasOptionalNormal() {return mgp::components::hasOptionalNormal<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalNormal = components::ReturnIfHasOptionalNormal<U, T>;

/** Port Scalar class into face namespace **/
template<typename T>
using Scalar = mgp::components::Scalar<T>;

using Scalarf = mgp::components::Scalarf;
using Scalard = mgp::components::Scalard;

template <typename T>
bool constexpr hasScalar() {return mgp::components::hasScalar<T>();};

/** Port OptionalScalar class into face namespace **/
template <typename S, typename T>
using OptionalScalar = mgp::components::OptionalScalar<S, T>;

template <typename T>
using OptionalScalarf = mgp::components::OptionalScalar<float, T>;

template <typename T>
using OptionalScalard = mgp::components::OptionalScalar<double, T>;

template <typename T>
bool constexpr hasOptionalScalar() {return mgp::components::hasOptionalScalar<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalScalar = components::ReturnIfHasOptionalScalar<U, T>;

/** Port CustomComponents class into face namespace **/
template <typename T>
using CustomComponents = mgp::components::CustomComponents<T>;

template <typename T>
bool constexpr hasCustomComponents() {return mgp::components::hasCustomComponents<T>();};

template<typename U, typename T>
using ReturnIfHasCustomComponents = components::ReturnIfHasCustomComponents<U, T>;

} // namespace mgp::face

#endif // MGP_MESH_FACE_COMMON_H
