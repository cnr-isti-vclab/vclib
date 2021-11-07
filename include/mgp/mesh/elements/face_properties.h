/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ELEMENTS_FACE_PROPERTIES_H
#define MGP_MESH_ELEMENTS_FACE_PROPERTIES_H

#include "../properties/bit_flags.h"
#include "../properties/face_references.h"
#include "../properties/color.h"
#include "../properties/mutable_bit_flags.h"
#include "../properties/normal.h"
#include "../properties/scalar.h"
#include "../properties/polygon_bit_flags.h"
#include "../properties/triangle_bit_flags.h"
#include "../properties/vertex_references.h"

namespace mgp::face {

/** Port BitFlags class into face namespace **/
using BitFlags = prop::BitFlags;

template <typename T>
bool constexpr hasBitFlags() {return prop::hasBitFlags<T>();};

using TriangleBitFlags = prop::TriangleBitFlags;

template <typename T>
bool constexpr hasTriangleBitFlags() {return prop::hasTriangleBitFlags<T>();};

/** Port Color class into face namespace **/
using Color = prop::Color;

template <typename T>
bool constexpr hasColor() {return prop::hasColor<T>();};

/** Port Normal classes into face namespace **/
template<typename Scalar, int N>
using Normal = prop::Normal<Scalar, N>;

template<typename Scalar>
using Normal3 = prop::Normal3<Scalar>;

using Normal3f = prop::Normal3f;
using Normal3d = prop::Normal3d;

template <typename T>
bool constexpr hasNormal() {return prop::hasNormal<T>();};

/** Port Scalar class into face namespace **/
template<typename T>
using Scalar = prop::Scalar<T>;

using Scalarf = prop::Scalarf;
using Scalard = prop::Scalard;

template <typename T>
bool constexpr hasScalar() {return prop::hasScalar<T>();};

/** Port VertexReferences class into face namespace **/
template<typename Vertex, int N>
using VertexReferences = prop::VertexReferences<Vertex, N>;

template<typename Vertex>
using TriangleVertexRefs = prop::TriVertexReferences<Vertex>;

template <typename  T>
bool constexpr hasVertexReferences() {return prop::hasVertexReferences<T>();}

/** Port AdjacentFaces class into face namespace **/
template<typename Face, int N>
using AdjacentFaces = prop::FaceReferences<Face, N>;

template <typename  T>
bool constexpr hasAdjacentFaces() {return prop::hasFaceReferences<T>();}

/** Port MutableBitFlags class into face namespace **/
using MutableBitFlags = prop::MutableBitFlags;

template <typename  T>
bool constexpr hasMutableBitFlags() {return prop::hasMutableBitFlags<T>();}

} // namespace mgp::face

#endif // MGP_MESH_ELEMENTS_FACE_PROPERTIES_H
