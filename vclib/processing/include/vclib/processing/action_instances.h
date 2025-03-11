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

#ifndef VCL_PROCESSING_ACTION_INSTANCES_H
#define VCL_PROCESSING_ACTION_INSTANCES_H

#include "action_instances/convert.h"
#include "action_instances/filter_mesh.h"
#include "action_instances/io_image.h"
#include "action_instances/io_mesh.h"

namespace vcl::proc {

inline std::vector<std::shared_ptr<Action>> actionInstances()
{
    std::vector<std::shared_ptr<Action>> vec;

    // Convert actions
    auto convertVector = convertActions();
    vec.insert(vec.end(), convertVector.begin(), convertVector.end());

    // IO Image actions
    auto ioImgVector = ioImageActions();
    vec.insert(vec.end(), ioImgVector.begin(), ioImgVector.end());

    // IO Mesh actions
    auto ioMeshVector = ioMeshActions();
    vec.insert(vec.end(), ioMeshVector.begin(), ioMeshVector.end());

    // // Filter Mesh actions
    auto filterMeshVector = filterMeshActions();
    vec.insert(vec.end(), filterMeshVector.begin(), filterMeshVector.end());

    return vec;
}

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_INSTANCES_H
