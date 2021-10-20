/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_COMMON_H
#define MGP_MESH_VERTEX_COMMON_H

#include "../common/info.h"
#include "../common/color.h"
#include "../common/normal.h"

namespace mgp {
namespace vert {

/** Port Info class into vert namespace **/
using Info = mgp::common::Info;

/** Port Color class into vert namespace **/
using Color = mgp::common::Color;

/** Port Color classes into vert namespace **/
template<class Scalar, int N>
using Normal = mgp::common::Normal<Scalar, N>;

template<class Scalar>
using Normal3 = mgp::common::Normal3<Scalar>;

using Normal3f = mgp::common::Normal3f;
using Normal3d = mgp::common::Normal3d;

} // namespace vert
} // namespace mgp

#endif // MGP_MESH_VERTEX_COMMON_H
