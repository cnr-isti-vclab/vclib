/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/types.h>

#ifndef VCLIB_PROCESSING_SCALAR_TYPE
#define VCLIB_PROCESSING_SCALAR_TYPE double
#endif // VCLIB_PROCESSING_SCALAR_TYPE

#ifndef VCLIB_PROCESSING_INDEXED_MESHES
#define VCLIB_PROCESSING_INDEXED_MESHES false
#endif // VCLIB_PROCESSING_INDEXED_MESHES

namespace vcl::proc {

/**
 * @brief Scalar type used in the meshes of the processing module.
 *
 * @ingroup processing
 */
using ProcScalarType = VCLIB_PROCESSING_SCALAR_TYPE;

/**
 * @brief Flag that enables the use of indexed meshes in the processing module.
 *
 * @ingroup processing
 */
inline constexpr bool INDEXED_MESHES = VCLIB_PROCESSING_INDEXED_MESHES;

// Action Types
inline constexpr uint SAVE_MESH_ACTION = 0;

} // namespace vcl::proc

#endif // VCL_PROCESSING_SETTINGS_H
