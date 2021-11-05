/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_VERTEX_PROPERTIES_OPTIONAL_H
#define MGP_VERTEX_PROPERTIES_OPTIONAL_H

#include "properties_optional/custom_properties.h"
#include "properties_optional/optional_color.h"
#include "properties_optional/optional_face_references.h"
#include "properties_optional/optional_mutable_bit_flags.h"
#include "properties_optional/optional_normal.h"
#include "properties_optional/optional_scalar.h"

namespace mgp::vert {

/** Port OptionalInfo class into vert namespace **/
template <typename T>
using OptionalInfo = prop::OptionalInfo<T>;

template <typename T>
bool constexpr hasOptionalInfo() {return prop::hasOptionalInfo<T>();};

/** Port OptionalMutableBitFlags class into vert namespace **/
template <typename T>
using OptionalMutableBitFlags = prop::OptionalMutableBitFlags<T>;

template <typename T>
bool constexpr hasOptionalMutableBitFlags() {return prop::hasOptionalMutableBitFlags<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalMutableBitFlags = prop::ReturnIfHasOptionalMutableBitFlags<U, T>;

/** Port OptionalColor class into vert namespace **/
template <typename T>
using OptionalColor = prop::OptionalColor<T>;

template <typename T>
bool constexpr hasOptionalColor() {return prop::hasOptionalColor<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalColor = prop::ReturnIfHasOptionalColor<U, T>;

/** Port OptionalNormal class into vert namespace **/
template <typename Scalar, int N, typename T>
using OptionalNormal = prop::OptionalNormal<Scalar, N, T>;

template <typename Scalar, typename T>
using OptionalNormal3 = prop::OptionalNormal3<Scalar, T>;

template <typename T>
using OptionalNormal3f = prop::OptionalNormal3f<T>;

template <typename T>
using OptionalNormal3d = prop::OptionalNormal3d<T>;

template <typename T>
bool constexpr hasOptionalNormal() {return prop::hasOptionalNormal<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalNormal = prop::ReturnIfHasOptionalNormal<U, T>;

/** Port OptionalScalar class into vert namespace **/
template <typename S, typename T>
using OptionalScalar = prop::OptionalScalar<S, T>;

template <typename T>
using OptionalScalarf = prop::OptionalScalar<float, T>;

template <typename T>
using OptionalScalard = prop::OptionalScalar<double, T>;

template <typename T>
bool constexpr hasOptionalScalar() {return prop::hasOptionalScalar<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalScalar = prop::ReturnIfHasOptionalScalar<U, T>;

/** Port OptionalAdjacentFacesRef class into vert namespace **/
template <typename Face, typename T>
using OptionalAdjacentFaces = prop::OptionalFaceReferences<Face, -1, T>;

template <typename T>
bool constexpr hasOptionalAdjacentFaces() {return prop::hasOptionalFaceReferences<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalAdjacentFaces = prop::ReturnIfHasOptionalFaceReferences<U, T>;

/** Port CustomProperties class into vert namespace **/
template <typename T>
using CustomProperties = prop::CustomProperties<T>;

template <typename T>
bool constexpr hasCustomProperties() {return prop::hasCustomProperties<T>();};

template<typename U, typename T>
using ReturnIfHasCustomProperties = prop::ReturnIfHasCustomProperties<U, T>;

}

#endif // MGP_VERTEX_PROPERTIES_OPTIONAL_H
