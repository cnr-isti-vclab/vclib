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

#ifndef VCL_PROCESSING_ACTIONS_FILTER_MESH_GENERATE_H
#define VCL_PROCESSING_ACTIONS_FILTER_MESH_GENERATE_H

#include "generate/convex_hull_filter.h"

#include <vclib/meshes.h>

#include <memory>
#include <vector>

namespace vcl::proc {

std::vector<std::shared_ptr<Action>> generateFilterMeshActions()
{
    std::vector<std::shared_ptr<Action>> vec;

    vec.push_back(ConvexHullFilter<vcl::TriMesh>().clone());

    return vec;
}

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_FILTER_MESH_GENERATE_H
