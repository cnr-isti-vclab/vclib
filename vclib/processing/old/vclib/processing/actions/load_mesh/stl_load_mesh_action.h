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

#ifndef VCL_PROCESSING_ACTIONS_LOAD_MESH_STL_LOAD_MESH_ACTION_H
#define VCL_PROCESSING_ACTIONS_LOAD_MESH_STL_LOAD_MESH_ACTION_H

#include <vclib/processing/action_interfaces/load_mesh_action.h>
#include <vclib/processing/functions.h>
#include <vclib/processing/meshes.h>
#include <vclib/processing/parameters.h>

#include <vclib/algorithms/mesh/clean.h>
#include <vclib/load_save/stl/load.h>

namespace vcl::proc {

class StlLoadMeshAction : public LoadMeshAction
{
public:
    using LoadMeshAction::load;

    std::string name() const override { return "Load Stl Mesh"; }

    std::shared_ptr<Action> clone() const override
    {
        return std::make_shared<StlLoadMeshAction>(*this);
    }

    ParameterVector parameters() const override
    {
        ParameterVector params;

        params.pushBack(BoolParameter(
            "unify_duplicate_vertices",
            true,
            "Unify Duplicate Vertices",
            "STL files may contain duplicated vertices. "
            "If this option is enabled, the loader will remove them. "
            "This may lead to a reduction in the number of vertices. "
            "The original number of vertices and the number of removed "
            "vertices will be logged."));

        return params;
    }

    std::vector<FileFormat> formats() const override
    {
        return {FileFormat("stl", "STL STereo Lithography interface format")};
    }

    std::shared_ptr<MeshI> load(
        const std::string&     filename,
        const ParameterVector& parameters,
        MeshInfo&              loadedInfo,
        AbstractLogger&        log = logger()) const override
    {
        auto tm = vcl::loadStl<TriMesh>(filename, loadedInfo, log);
        if (parameters.get("unify_duplicate_vertices")->boolValue()) {
            vcl::removeDuplicatedVertices(tm);
            tm.compactVertices();
            // todo: log the number of removed vertices
        }

        postLoad(tm, loadedInfo);
        return std::make_shared<TriMesh>(tm);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_LOAD_MESH_STL_LOAD_MESH_ACTION_H
