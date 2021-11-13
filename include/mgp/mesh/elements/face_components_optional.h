/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ELEMENTS_FACE_COMPONENTS_OPTIONAL_H
#define MGP_MESH_ELEMENTS_FACE_COMPONENTS_OPTIONAL_H

#include "../components_optional/custom_components.h"
#include "../components_optional/optional_adjacent_faces.h"
#include "../components_optional/optional_color.h"
#include "../components_optional/optional_mutable_bit_flags.h"
#include "../components_optional/optional_normal.h"
#include "../components_optional/optional_scalar.h"

namespace mgp::face {

/** Port OptionalInfo class into face namespace **/
template <typename T>
using OptionalInfo = comp::OptionalInfo<T>;

template <typename T>
bool constexpr hasOptionalInfo() {return comp::hasOptionalInfo<T>();};

/** Port OptionalMutableBitFlags class into face namespace **/
template <typename T>
using OptionalMutableBitFlags = comp::OptionalMutableBitFlags<T>;

template <typename T>
bool constexpr hasOptionalMutableBitFlags() {return comp::hasOptionalMutableBitFlags<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalMutableBitFlags = comp::ReturnIfHasOptionalMutableBitFlags<U, T>;

/** Port OptionalColor class into face namespace **/
template <typename T>
using OptionalColor = comp::OptionalColor<T>;

template <typename T>
bool constexpr hasOptionalColor() {return comp::hasOptionalColor<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalColor = comp::ReturnIfHasOptionalColor<U, T>;

/** Port OptionalNormal class into face namespace **/
template <typename Scalar, int N, typename T>
using OptionalNormal = comp::OptionalNormal<Scalar, N, T>;

template <typename Scalar, typename T>
using OptionalNormal3 = comp::OptionalNormal3<Scalar, T>;

template <typename T>
using OptionalNormal3f = comp::OptionalNormal3f<T>;

template <typename T>
using OptionalNormal3d = comp::OptionalNormal3d<T>;

template <typename T>
bool constexpr hasOptionalNormal() {return comp::hasOptionalNormal<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalNormal = comp::ReturnIfHasOptionalNormal<U, T>;

/** Port OptionalScalar class into face namespace **/
template <typename S, typename T>
using OptionalScalar = comp::OptionalScalar<S, T>;

template <typename T>
using OptionalScalarf = comp::OptionalScalar<float, T>;

template <typename T>
using OptionalScalard = comp::OptionalScalar<double, T>;

template <typename T>
bool constexpr hasOptionalScalar() {return comp::hasOptionalScalar<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalScalar = comp::ReturnIfHasOptionalScalar<U, T>;

/** Port OptionalAdjacentFaces class into face namespace **/
template <typename T>
using OptionalAdjacentFaces = comp::OptionalAdjacentFaces<T, -1, T>;

template <typename T>
using OptionalAdjacentTriangles = comp::OptionalAdjacentFaces<T, 3, T>;

template <typename T>
bool constexpr hasOptionalAdjacentFaces() {return comp::hasOptionalAdjacentFaces<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalAdjacentFaces = comp::ReturnIfHasOptionalAdjacentFaces<U, T>;

/** Port CustomComponents class into face namespace **/
template <typename T>
using CustomComponents = comp::CustomComponents<T>;

template <typename T>
bool constexpr hasCustomComponents() {return comp::hasCustomComponents<T>();};

template<typename U, typename T>
using ReturnIfHasCustomComponents = comp::ReturnIfHasCustomComponents<U, T>;

}

#endif // MGP_MESH_ELEMENTS_FACE_COMPONENTS_OPTIONAL_H
