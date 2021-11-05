/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_PROPERTIES_H
#define MGP_MESH_PROPERTIES_H

#include "properties/bounding_box.h"

namespace mgp::mesh {

/** Port BoundingBox class into mesh namespace **/
template <typename P>
using BoundingBox = prop::BoundingBox<P>;

using BoundingBox3f = prop::BoundingBox3f;
using BoundingBox3d = prop::BoundingBox3d;

template <typename T>
bool constexpr hasBoundingBox() {return prop::hasBoundingBox<T>();};

template<typename U, typename T>
using ReturnIfHasBoundingBox = prop::ReturnIfHasBoundingBox<U, T>;

}

#endif // MGP_MESH_PROPERTIES_H
