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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_H
#define VCL_CONCEPTS_MESH_COMPONENTS_H

#include "components/adjacent_edges.h"
#include "components/adjacent_faces.h"
#include "components/adjacent_vertices.h"
#include "components/bit_flags.h"
#include "components/bounding_box.h"
#include "components/color.h"
#include "components/component.h"
#include "components/coordinate.h"
#include "components/custom_components.h"
#include "components/mark.h"
#include "components/name.h"
#include "components/normal.h"
#include "components/principal_curvature.h"
#include "components/quality.h"
#include "components/tex_coord.h"
#include "components/texture_paths.h"
#include "components/transform_matrix.h"
#include "components/vertex_references.h"
#include "components/wedge_colors.h"
#include "components/wedge_tex_coords.h"

/**
 * @defgroup components_concepts Components Concepts
 * @ingroup mesh_concepts
 * @ingroup components
 *
 * @brief List of concepts for types related to the Component classes the
 * library.
 */

#endif // VCL_CONCEPTS_MESH_COMPONENTS_H
