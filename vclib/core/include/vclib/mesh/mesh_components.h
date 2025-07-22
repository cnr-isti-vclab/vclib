/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_MESH_COMPONENTS_H
#define VCL_MESH_MESH_COMPONENTS_H

#include "components/bounding_box.h"
#include "components/color.h"
#include "components/custom_components.h"
#include "components/mark.h"
#include "components/materials.h"
#include "components/name.h"
#include "components/texture_images.h"
#include "components/texture_paths.h"
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

/** Port TextureImages class into mesh namespace **/
using TextureImages = comp::TextureImages;

/** Port TexturePaths class into mesh namespace **/
using TexturePaths = comp::TexturePaths;

/** Port Textures class into mesh namespace **/
template<bool STORE_TEXTURE_IMAGES>
using Textures =
    std::conditional_t<STORE_TEXTURE_IMAGES, TextureImages, TexturePaths>;

/** Port TransformMatrix class into mesh namespace **/
template<typename Scalar>
using TransformMatrix = comp::TransformMatrix<Scalar>;

using TransformMatrixf = comp::TransformMatrixf<>;
using TransformMatrixd = comp::TransformMatrixd<>;

/** @} */ // end of mesh_components group

} // namespace vcl::mesh

#endif // VCL_MESH_MESH_COMPONENTS_H
