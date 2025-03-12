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

#ifndef VCL_PROCESSING_MANAGER_ACTION_MANAGER_MESH_IO_MANAGER_H
#define VCL_PROCESSING_MANAGER_ACTION_MANAGER_MESH_IO_MANAGER_H

#include "io_action_container.h"

#include <vclib/processing/engine/action_aggregators.h>

namespace vcl::proc::detail {

class MeshIOManager
{
    IOActionContainer<MeshIOActions> mMeshIOActions;

protected:
    void add(const std::shared_ptr<MeshIOActions>& action)
    {
        mMeshIOActions.add(action);
    }

public:
    // load mesh

    std::vector<FileFormat> loadMeshFormats() const
    {
        return mMeshIOActions.loadFormats();
    }

    ParameterVector loadMeshParameters(FileFormat fmt) const
    {
        return loadMeshActions(fmt)->parametersLoad(fmt);
    }

    std::shared_ptr<MeshIOActions> loadMeshActions(FileFormat fmt) const
    {
        return mMeshIOActions.loadAction(fmt);
    }

    template<MeshConcept MeshType>
    std::shared_ptr<MeshIOActionT<MeshType>> loadMeshAction(
        FileFormat fmt) const
    {
        std::shared_ptr<MeshIOActions> actions = loadMeshActions(fmt);
        return actions->action<MeshType>();
    }

    // save mesh

    std::vector<FileFormat> saveMeshFormats() const
    {
        return mMeshIOActions.saveFormats();
    }

    std::vector<FileFormat> saveMeshFormats(MeshTypeId m) const
    {
        return mMeshIOActions.saveFormats(m);
    }

    ParameterVector saveMeshParameters(FileFormat fmt) const
    {
        return saveMeshActions(fmt)->parametersSave(fmt);
    }

    std::shared_ptr<MeshIOActions> saveMeshActions(FileFormat fmt) const
    {
        return mMeshIOActions.saveAction(fmt);
    }

    template<MeshConcept MeshType>
    std::shared_ptr<MeshIOActionT<MeshType>> saveMeshAction(
        FileFormat fmt) const
    {
        std::shared_ptr<MeshIOActions> actions = saveMeshActions(fmt);
        return actions->action<MeshType>();
    }
};

} // namespace vcl::proc::detail

#endif // VCL_PROCESSING_MANAGER_ACTION_MANAGER_MESH_IO_MANAGER_H
