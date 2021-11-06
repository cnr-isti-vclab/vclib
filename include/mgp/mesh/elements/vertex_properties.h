/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ELEMENTS_VERTEX_PROPERTIES_H
#define MGP_MESH_ELEMENTS_VERTEX_PROPERTIES_H

#include "../properties/bit_flags.h"
#include "../properties/color.h"
#include "../properties/coordinate.h"
#include "../properties/face_references.h"
#include "../properties/mutable_bit_flags.h"
#include "../properties/normal.h"
#include "../properties/scalar.h"
#include "../properties/tex_coord.h"


namespace mgp::vert {

/** Port BitFlags class into vert namespace **/
using BitFlags = prop::BitFlags;

template <typename T>
bool constexpr hasBitFlags() {return prop::hasBitFlags<T>();};

/** Port Color class into vert namespace **/
using Color = prop::Color;

template <typename T>
bool constexpr hasColor() {return prop::hasColor<T>();};

/** Port Coordinate class into vert namespace **/
template<class Scalar, int N>
using Coordinate = prop::Coordinate<Scalar, N>;

template<class Scalar>
using Coordinate3 = prop::Coordinate3<Scalar>;

using Coordinate3f = prop::Coordinate3f;
using Coordinate3d = prop::Coordinate3d;

template <typename T>
bool constexpr hasCoordinate() {return prop::hasCoordinate<T>();};

/** Port Normal classes into vert namespace **/
template<class Scalar, int N>
using Normal = prop::Normal<Scalar, N>;

template<class Scalar>
using Normal3 = prop::Normal3<Scalar>;

using Normal3f = prop::Normal3f;
using Normal3d = prop::Normal3d;

template <typename T>
bool constexpr hasNormal() {return prop::hasNormal<T>();};

/** Port Scalar class into vert namespace **/
template<typename T>
using Scalar = prop::Scalar<T>;

using Scalarf = prop::Scalarf;
using Scalard = prop::Scalard;

template <typename T>
bool constexpr hasScalar() {return prop::hasScalar<T>();};

/** Port AdjacentFacesRef class into vert namespace **/
template <typename Face>
using AdjacentFaces = prop::FaceReferences<Face, -1>;

template <typename T>
bool constexpr hasAdjacentFaces() {return prop::hasFaceReferences<T>();};

/** Port MutableBitFlags class into face namespace **/
using MutableBitFlags = prop::MutableBitFlags;

template <typename  T>
bool constexpr hasMutableBitFlags() {return prop::hasMutableBitFlags<T>();}

/** Port TexCoord class into vert namespace **/
template <typename T>
using TexCoord = prop::TexCoord<T>;

template <typename  T>
bool constexpr hasTexCoord() {return prop::hasTexCoord<T>();}

using TexCoordf = prop::TexCoordf;
using TexCoordd = prop::TexCoordd;

} // namespace mgp::vert

#endif // MGP_MESH_ELEMENTS_VERTEX_PROPERTIES_H
