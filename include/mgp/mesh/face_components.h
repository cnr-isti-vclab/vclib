/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_COMPONENTS_H
#define MGP_MESH_FACE_COMPONENTS_H

#include "components/bit_flags.h"
#include "components/color.h"
#include "components/mutable_bit_flags.h"
#include "components/normal.h"
#include "components/scalar.h"
#include "components/triangle_bit_flags.h"
#include "components/vertex_references.h"

namespace mgp::face {

/** Port BitFlags class into face namespace **/
using BitFlags = components::BitFlags;

template <typename T>
bool constexpr hasBitFlags() {return components::hasBitFlags<T>();};

using TriangleBitFlags = components::TriangleBitFlags;

template <typename T>
bool constexpr hasTriangleBitFlags() {return components::hasTriangleBitFlags<T>();};

/** Port Color class into face namespace **/
using Color = components::Color;

template <typename T>
bool constexpr hasColor() {return components::hasColor<T>();};

/** Port Normal classes into face namespace **/
template<typename Scalar, int N>
using Normal = components::Normal<Scalar, N>;

template<typename Scalar>
using Normal3 = components::Normal3<Scalar>;

using Normal3f = components::Normal3f;
using Normal3d = components::Normal3d;

template <typename T>
bool constexpr hasNormal() {return components::hasNormal<T>();};

/** Port Scalar class into face namespace **/
template<typename T>
using Scalar = components::Scalar<T>;

using Scalarf = components::Scalarf;
using Scalard = components::Scalard;

template <typename T>
bool constexpr hasScalar() {return components::hasScalar<T>();};

/** Port VertexRefsArray class into face namespace **/
template<typename Vertex, int N>
using VertexReferences = components::VertexReferences<Vertex, N>;

template<typename Vertex>
using TriangleVertexRefs = components::TriVertexReferences<Vertex>;

template <typename  T>
bool constexpr hasVertexReferences() {return components::hasVertexReferences<T>();}

} // namespace mgp::face

#endif // MGP_MESH_FACE_COMPONENTS_H
