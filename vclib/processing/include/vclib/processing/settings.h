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

#ifndef VCL_PROCESSING_SETTINGS_H
#define VCL_PROCESSING_SETTINGS_H

#include <vclib/meshes.h>

namespace vcl::proc {

/**
 * @brief Scalar type used in the processing module.
 */
using ScalarType = double;

/**
 * @brief Flag that indicates if the meshes are indexed.
 */
constexpr bool INDEXED_MESHES = false;

/**
 * @brief List of supported mesh types supported by the processing module.
 */
using MeshTypes = TypeWrapper<
    vcl::TriEdgeMeshT<ScalarType, INDEXED_MESHES>,
    vcl::PolyEdgeMeshT<ScalarType, INDEXED_MESHES>>;

} // namespace vcl::proc

#endif // VCL_PROCESSING_SETTINGS_H
