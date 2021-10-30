/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_COMPONENTS_H
#define MGP_MESH_VERTEX_COMPONENTS_H

#include "components/bit_flags.h"
#include "components/color.h"
#include "components/coordinate.h"
#include "components/face_references.h"
#include "components/mutable_bit_flags.h"
#include "components/normal.h"
#include "components/scalar.h"


namespace mgp::vert {

/** Port BitFlags class into vert namespace **/
using BitFlags = components::BitFlags;

template <typename T>
bool constexpr hasBitFlags() {return components::hasBitFlags<T>();};

/** Port Color class into vert namespace **/
using Color = components::Color;

template <typename T>
bool constexpr hasColor() {return components::hasColor<T>();};



/** Port Coordinate class into vert namespace **/
template<class Scalar, int N>
using Coordinate = components::Coordinate<Scalar, N>;

template<class Scalar>
using Coordinate3 = components::Coordinate3<Scalar>;

using Coordinate3f = components::Coordinate3f;
using Coordinate3d = components::Coordinate3d;

template <typename T>
bool constexpr hasCoordinate() {return components::hasCoordinate<T>();};

/** Port Normal classes into vert namespace **/
template<class Scalar, int N>
using Normal = components::Normal<Scalar, N>;

template<class Scalar>
using Normal3 = components::Normal3<Scalar>;

using Normal3f = components::Normal3f;
using Normal3d = components::Normal3d;

template <typename T>
bool constexpr hasNormal() {return components::hasNormal<T>();};

/** Port Scalar class into vert namespace **/
template<typename T>
using Scalar = components::Scalar<T>;

using Scalarf = components::Scalarf;
using Scalard = components::Scalard;

template <typename T>
bool constexpr hasScalar() {return components::hasScalar<T>();};

/** Port AdjacentFacesRef class into vert namespace **/
template <typename Face>
using AdjacentFaces = components::FaceReferences<Face, -1>;

template <typename T>
bool constexpr hasAdjacentFaces() {return components::hasFaceReferences<T>();};

} // namespace mgp::vert

#endif // MGP_MESH_VERTEX_COMPONENTS_H
