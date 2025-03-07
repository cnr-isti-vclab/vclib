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

#ifndef VCL_PROCESSING_ACTION_INSTANCES_IO_MESH_H
#define VCL_PROCESSING_ACTION_INSTANCES_IO_MESH_H

#include "fill_actions.h"

#include <vclib/processing/actions/io_mesh.h>
#include <vclib/processing/engine/action_aggregators/mesh_io_actions.h>

#include <memory>
#include <vector>

namespace vcl::proc {

// template<MeshConcept MeshType>
// inline std::vector<std::shared_ptr<Action>> ioMeshActions()
// {
//     std::vector<std::shared_ptr<Action>> vec;

//     using Actions = TemplatedTypeWrapper<BaseIOMesh>;

//     fillActionsIfSupported<MeshType>(vec, Actions());

//     return vec;
// }

// inline std::vector<std::shared_ptr<Action>> ioMeshActions()
// {
//     std::vector<std::shared_ptr<Action>> vec;

//     // lambda called for each mesh type
//     auto fMesh = [&]<typename MeshType>() {
//         auto v = ioMeshActions<MeshType>();
//         vec.insert(vec.end(), v.begin(), v.end());
//     };

//     // call lambda for each mesh type
//     vcl::ForEachType<MeshTypes>::apply(fMesh);

//     return vec;
// }

inline std::vector<std::shared_ptr<Action>> ioMeshActions()
{
    std::vector<std::shared_ptr<Action>> vec;

    using Actions = TemplatedTypeWrapper<BaseIOMesh>;

    fillAggregatedActions<MeshIOActions>(vec, Actions());

    return vec;
}

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_INSTANCES_IO_MESH_H
