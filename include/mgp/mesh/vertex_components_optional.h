/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_VERTEX_COMPONENTS_OPTIONAL_H
#define MGP_VERTEX_COMPONENTS_OPTIONAL_H

#include "components_optional/custom_components.h"
#include "components_optional/optional_color.h"
#include "components_optional/optional_face_references.h"
#include "components_optional/optional_mutable_bit_flags.h"
#include "components_optional/optional_normal.h"
#include "components_optional/optional_scalar.h"

namespace mgp::vert {

/** Port OptionalInfo class into vert namespace **/
template <typename T>
using OptionalInfo = components::OptionalInfo<T>;

template <typename T>
bool constexpr hasOptionalInfo() {return components::hasOptionalInfo<T>();};

/** Port OptionalMutableBitFlags class into vert namespace **/
template <typename T>
using OptionalMutableBitFlags = components::OptionalMutableBitFlags<T>;

template <typename T>
bool constexpr hasOptionalMutableBitFlags() {return components::hasOptionalMutableBitFlags<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalMutableBitFlags = components::ReturnIfHasOptionalMutableBitFlags<U, T>;

/** Port OptionalColor class into vert namespace **/
template <typename T>
using OptionalColor = components::OptionalColor<T>;

template <typename T>
bool constexpr hasOptionalColor() {return components::hasOptionalColor<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalColor = components::ReturnIfHasOptionalColor<U, T>;

/** Port OptionalNormal class into vert namespace **/
template <typename Scalar, int N, typename T>
using OptionalNormal = components::OptionalNormal<Scalar, N, T>;

template <typename Scalar, typename T>
using OptionalNormal3 = components::OptionalNormal3<Scalar, T>;

template <typename T>
using OptionalNormal3f = components::OptionalNormal3f<T>;

template <typename T>
using OptionalNormal3d = components::OptionalNormal3d<T>;

template <typename T>
bool constexpr hasOptionalNormal() {return components::hasOptionalNormal<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalNormal = components::ReturnIfHasOptionalNormal<U, T>;

/** Port OptionalScalar class into vert namespace **/
template <typename S, typename T>
using OptionalScalar = components::OptionalScalar<S, T>;

template <typename T>
using OptionalScalarf = components::OptionalScalar<float, T>;

template <typename T>
using OptionalScalard = components::OptionalScalar<double, T>;

template <typename T>
bool constexpr hasOptionalScalar() {return components::hasOptionalScalar<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalScalar = components::ReturnIfHasOptionalScalar<U, T>;

/** Port OptionalAdjacentFacesRef class into vert namespace **/
template <typename Face, typename T>
using OptionalAdjacentFaces = components::OptionalFaceReferences<Face, T>;

template <typename T>
bool constexpr hasOptionalAdjacentFaces() {return components::hasOptionalFaceReferences<T>();};

/** Port CustomComponents class into vert namespace **/
template <typename T>
using CustomComponents = components::CustomComponents<T>;

template <typename T>
bool constexpr hasCustomComponents() {return components::hasCustomComponents<T>();};

template<typename U, typename T>
using ReturnIfHasCustomComponents = components::ReturnIfHasCustomComponents<U, T>;

}

#endif // MGP_VERTEX_COMPONENTS_OPTIONAL_H
