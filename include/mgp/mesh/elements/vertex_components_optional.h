/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ELEMENTS_VERTEX_COMPONENTS_OPTIONAL_H
#define MGP_MESH_ELEMENTS_VERTEX_COMPONENTS_OPTIONAL_H

#include "../components_optional/custom_components.h"
#include "../components_optional/optional_color.h"
#include "../components_optional/optional_face_references.h"
#include "../components_optional/optional_mutable_bit_flags.h"
#include "../components_optional/optional_normal.h"
#include "../components_optional/optional_scalar.h"
#include "../components_optional/optional_tex_coord.h"
#include "../components_optional/optional_vertex_references.h"

namespace mgp::vert {

/** Port OptionalInfo class into vert namespace **/
template <typename T>
using OptionalInfo = comp::OptionalInfo<T>;

template <typename T>
bool constexpr hasOptionalInfo() {return comp::hasOptionalInfo<T>();};

/** Port OptionalMutableBitFlags class into vert namespace **/
template <typename T>
using OptionalMutableBitFlags = comp::OptionalMutableBitFlags<T>;

template <typename T>
bool constexpr hasOptionalMutableBitFlags() {return comp::hasOptionalMutableBitFlags<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalMutableBitFlags = comp::ReturnIfHasOptionalMutableBitFlags<U, T>;

/** Port OptionalColor class into vert namespace **/
template <typename T>
using OptionalColor = comp::OptionalColor<T>;

template <typename T>
bool constexpr hasOptionalColor() {return comp::hasOptionalColor<T>();};

template<typename U, typename T>
using ReturnIfHasOptionalColor = comp::ReturnIfHasOptionalColor<U, T>;

/** Port OptionalNormal class into vert namespace **/
template <typename Scalar, int N, typename T>
using OptionalNormal = comp::OptionalNormal<Scalar, N, T>;

template <typename Scalar, typename T>
using OptionalNormal3 = comp::OptionalNormal3<Scalar, T>;

template <typename T>
using OptionalNormal3f = comp::OptionalNormal3f<T>;

template <typename T>
using OptionalNormal3d = comp::OptionalNormal3d<T>;

template <typename T>
bool constexpr hasOptionalNormal() {return comp::hasOptionalNormal<T>();}

template<typename U, typename T>
using ReturnIfHasOptionalNormal = comp::ReturnIfHasOptionalNormal<U, T>;

/** Port OptionalScalar class into vert namespace **/
template <typename S, typename T>
using OptionalScalar = comp::OptionalScalar<S, T>;

template <typename T>
using OptionalScalarf = comp::OptionalScalar<float, T>;

template <typename T>
using OptionalScalard = comp::OptionalScalar<double, T>;

template <typename T>
bool constexpr hasOptionalScalar() {return comp::hasOptionalScalar<T>();}

template<typename U, typename T>
using ReturnIfHasOptionalScalar = comp::ReturnIfHasOptionalScalar<U, T>;

/** Port OptionalTexCoord class into vert namespace **/
template <typename S, typename T>
using OptionalTexCoord = comp::OptionalTexCoord<S, T>;

template <typename T>
using OptionalTexCoordf = comp::OptionalTexCoord<float, T>;

template <typename T>
using OptionalTexCoordd = comp::OptionalTexCoord<double, T>;

template <typename T>
bool constexpr hasOptionalTexCoord() {return comp::hasOptionalTexCoord<T>();}

template<typename U, typename T>
using ReturnIfHasOptionalTexCoord = comp::ReturnIfHasOptionalTexCoord<U, T>;

/** Port OptionalAdjacentFacesRef class into vert namespace **/
template <typename Face, typename T>
using OptionalAdjacentFaces = comp::OptionalFaceReferences<Face, -1, T>;

template <typename T>
bool constexpr hasOptionalAdjacentFaces() {return comp::hasOptionalFaceReferences<T>();}

template<typename U, typename T>
using ReturnIfHasOptionalAdjacentFaces = comp::ReturnIfHasOptionalFaceReferences<U, T>;

/** Port OptionalAdjacentVertexRef class into vert namespace **/
template <typename T>
using OptionalAdjacentVertices = comp::OptionalVertexReferences<T, -1, T>;

template <typename T>
bool constexpr hasOptionalAdjacentVertices() {return comp::hasOptionalVertexReferences<T>();}

template<typename U, typename T>
using ReturnIfHasOptionalAdjacentVertices = comp::ReturnIfHasOptionalVertexReferences<U, T>;

/** Port CustomComponents class into vert namespace **/
template <typename T>
using CustomComponents = comp::CustomComponents<T>;

template <typename T>
bool constexpr hasCustomComponents() {return comp::hasCustomComponents<T>();};

template<typename U, typename T>
using ReturnIfHasCustomComponents = comp::ReturnIfHasCustomComponents<U, T>;

}

#endif // MGP_MESH_ELEMENTS_VERTEX_COMPONENTS_OPTIONAL_H
