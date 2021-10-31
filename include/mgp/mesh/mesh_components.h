/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_H
#define MGP_MESH_COMPONENTS_H

#include "components/bounding_box.h"

namespace mgp::mesh {

/** Port BoundingBox class into mesh namespace **/
template <typename P>
using BoundingBox = components::BoundingBox<P>;

using BoundingBox3f = components::BoundingBox3f;
using BoundingBox3d = components::BoundingBox3d;

template <typename T>
bool constexpr hasBoundingBox() {return components::hasBoundingBox<T>();};

template<typename U, typename T>
using ReturnIfHasBoundingBox = components::ReturnIfHasBoundingBox<U, T>;

}

#endif // MGP_MESH_COMPONENTS_H
