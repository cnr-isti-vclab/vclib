/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ELEMENTS_FACE_COMPONENTS_H
#define MGP_MESH_ELEMENTS_FACE_COMPONENTS_H

#include "../components/bit_flags.h"
#include "../components/face_references.h"
#include "../components/color.h"
#include "../components/mutable_bit_flags.h"
#include "../components/normal.h"
#include "../components/scalar.h"
#include "../components/polygon_bit_flags.h"
#include "../components/triangle_bit_flags.h"
#include "../components/vertex_references.h"
#include "../components/wedge_tex_coords.h"

namespace mgp::face {

/** Port BitFlags class into face namespace **/
using BitFlags = comp::BitFlags;

template <typename T>
bool constexpr hasBitFlags() {return comp::hasBitFlags<T>();};

using TriangleBitFlags = comp::TriangleBitFlags;

template <typename T>
bool constexpr hasTriangleBitFlags() {return comp::hasTriangleBitFlags<T>();};

/** Port Color class into face namespace **/
using Color = comp::Color;

template <typename T>
bool constexpr hasColor() {return comp::hasColor<T>();};

/** Port Normal classes into face namespace **/
template<typename Scalar, int N>
using Normal = comp::Normal<Scalar, N>;

template<typename Scalar>
using Normal3 = comp::Normal3<Scalar>;

using Normal3f = comp::Normal3f;
using Normal3d = comp::Normal3d;

template <typename T>
bool constexpr hasNormal() {return comp::hasNormal<T>();};

/** Port Scalar class into face namespace **/
template<typename T>
using Scalar = comp::Scalar<T>;

using Scalarf = comp::Scalarf;
using Scalard = comp::Scalard;

template <typename T>
bool constexpr hasScalar() {return comp::hasScalar<T>();};

/** Port VertexReferences class into face namespace **/
template<typename Vertex, int N>
using VertexReferences = comp::VertexReferences<Vertex, N>;

template<typename Vertex>
using TriangleVertexRefs = comp::TriVertexReferences<Vertex>;

template <typename  T>
bool constexpr hasVertexReferences() {return comp::hasVertexReferences<T>();}

/** Port AdjacentFaces class into face namespace **/
template<typename Face>
using AdjacentFaces = comp::FaceReferences<Face, -1>;

template<typename Face>
using AdjacentTriangles = comp::FaceReferences<Face, 3>;

template <typename  T>
bool constexpr hasAdjacentFaces() {return comp::hasFaceReferences<T>();}

/** Port WedgeTexCoords class into face namespace **/
template<typename Scalar>
using WedgeTexCoords = comp::WedgeTexCoords<Scalar, -1>;

template<typename Scalar>
using TriangleWedgeTexCoords = comp::WedgeTexCoords<Scalar, 3>;

using WedgeTexCoordf = comp::WedgeTexCoords<float, -1>;
using WedgeTexCoordd = comp::WedgeTexCoords<double, -1>;
using TriangleWedgeTexCoordf = TriangleWedgeTexCoords<float>;
using TriangleWedgeTexCoordd = TriangleWedgeTexCoords<double>;

template <typename  T>
bool constexpr hasWedgeTexCoords() {return comp::hasWedgeTexCoords<T>();}

/** Port MutableBitFlags class into face namespace **/
using MutableBitFlags = comp::MutableBitFlags;

template <typename  T>
bool constexpr hasMutableBitFlags() {return comp::hasMutableBitFlags<T>();}

} // namespace mgp::face

#endif // MGP_MESH_ELEMENTS_FACE_COMPONENTS_H
