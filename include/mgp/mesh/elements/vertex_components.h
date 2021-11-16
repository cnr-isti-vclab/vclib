/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ELEMENTS_VERTEX_COMPONENTS_H
#define MGP_MESH_ELEMENTS_VERTEX_COMPONENTS_H

#include "../components/adjacent_faces.h"
#include "../components/adjacent_vertices.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/coordinate.h"
#include "../components/mutable_bit_flags.h"
#include "../components/normal.h"
#include "../components/scalar.h"
#include "../components/tex_coord.h"
#include "../components/vertex_references.h"


namespace mgp::vert {

/** Port BitFlags class into vert namespace **/
using BitFlags = comp::BitFlags;

template <typename T>
bool constexpr hasBitFlags() {return comp::hasBitFlags<T>();};

/** Port Color class into vert namespace **/
using Color = comp::Color;

template <typename T>
bool constexpr hasColor() {return comp::hasColor<T>();};

/** Port Coordinate class into vert namespace **/
template<typename Scalar, int N>
using Coordinate = comp::Coordinate<Scalar, N>;

template<typename Scalar>
using Coordinate3 = comp::Coordinate3<Scalar>;

using Coordinate3f = comp::Coordinate3f;
using Coordinate3d = comp::Coordinate3d;

template <typename T>
bool constexpr hasCoordinate() {return comp::hasCoordinate<T>();};

/** Port Normal classes into vert namespace **/
template<typename Scalar, int N>
using Normal = comp::Normal<Scalar, N>;

template<typename Scalar>
using Normal3 = comp::Normal3<Scalar>;

using Normal3f = comp::Normal3f;
using Normal3d = comp::Normal3d;

template <typename T>
bool constexpr hasNormal() {return comp::hasNormal<T>();};

/** Port Scalar class into vert namespace **/
template<typename T>
using Scalar = comp::Scalar<T>;

using Scalarf = comp::Scalarf;
using Scalard = comp::Scalard;

template <typename T>
bool constexpr hasScalar() {return comp::hasScalar<T>();};

/** Port AdjacentFacesRef class into vert namespace **/
template <typename Face>
using AdjacentFaces = comp::AdjacentFaces<Face, -1>;

template <typename T>
bool constexpr hasAdjacentFaces() {return comp::hasAdjacentFaces<T>();};

/** Port MutableBitFlags class into face namespace **/
using MutableBitFlags = comp::MutableBitFlags;

template <typename  T>
bool constexpr hasMutableBitFlags() {return comp::hasMutableBitFlags<T>();}

/** Port TexCoord class into vert namespace **/
template <typename T>
using TexCoord = comp::TexCoord<T>;

template <typename  T>
bool constexpr hasTexCoord() {return comp::hasTexCoord<T>();}

using TexCoordf = comp::TexCoordf;
using TexCoordd = comp::TexCoordd;

/** Port AdjacentVertices class into vert namespace **/
template <typename Vertex>
using AdjacentVertices = comp::AdjacentVertices<Vertex, -1>;

template <typename T>
bool constexpr hasAdjacentVertices() {return comp::hasAdjacentVertices<T>();};

} // namespace mgp::vert

#endif // MGP_MESH_ELEMENTS_VERTEX_COMPONENTS_H
