// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_MESH_COMPONENTS_H
#define VCL_MESH_MESH_COMPONENTS_H

#include "components/bounding_box.h"
#include "components/color.h"
#include "components/custom_components.h"
#include "components/mark.h"
#include "components/materials.h"
#include "components/name.h"
#include "components/transform_matrix.h"

/**
 * @defgroup mesh_components Mesh Components
 * @ingroup components
 *
 * @brief This group lists all the Components that may be used by a Mesh class.
 *
 * All these components are listed inside the vcl::mesh namespace.
 */

namespace vcl::mesh {

/* Port concepts into the mesh namespace */
template<typename T>
concept HasBoundingBox = comp::HasBoundingBox<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasCustomComponents = comp::HasCustomComponents<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasMaterials = comp::HasMaterials<T>;
template<typename T>
concept HasName = comp::HasName<T>;
template<typename T>
concept HasTransformMatrix = comp::HasTransformMatrix<T>;

/**
 * @addtogroup mesh_components
 * @{
 */

/** Port BoundingBox class into mesh namespace **/
template<typename P>
using BoundingBox = comp::BoundingBox<P>;

template<typename S>
using BoundingBox3 = comp::BoundingBox3<S>;

using BoundingBox3f = comp::BoundingBox3f<>;
using BoundingBox3d = comp::BoundingBox3d<>;

/** Port Color class into mesh namespace **/
using Color = comp::Color<>;

/** Port CustomComponents class into mesh namespace **/
using CustomComponents = comp::CustomComponents<>;

/** Port Mark class into mesh namespace **/
using Mark = comp::Mark<>;

/** Port Materials class into mesh namespace **/
using Materials = comp::Materials;

/** Port Name class into mesh namespace **/
using Name = comp::Name<>;

/** Port TransformMatrix class into mesh namespace **/
template<typename Scalar>
using TransformMatrix = comp::TransformMatrix<Scalar>;

using TransformMatrixf = comp::TransformMatrixf<>;
using TransformMatrixd = comp::TransformMatrixd<>;

/** @} */ // end of mesh_components group

} // namespace vcl::mesh

#endif // VCL_MESH_MESH_COMPONENTS_H
