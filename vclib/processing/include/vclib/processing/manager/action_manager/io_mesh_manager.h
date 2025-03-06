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

#ifndef VCL_PROCESSING_MANAGER_ACTION_MANAGER_IO_MESH_MANAGER_H
#define VCL_PROCESSING_MANAGER_ACTION_MANAGER_IO_MESH_MANAGER_H

#include "io_action_container.h"

#include <vclib/processing/engine/action_interfaces.h>

namespace vcl::proc::detail {

class IOMeshManager
{
    static const uint MESH_TYPE_NUMBER = toUnderlying(MeshTypeId::COUNT);

    std::array<IOActionContainer, MESH_TYPE_NUMBER> mMeshIOActions;

protected:
    void add(const std::shared_ptr<IOAction>& action)
    {
        uint mt = toUnderlying(action->meshType());
        mMeshIOActions[mt].add(action);
    }

public:
    // load mesh

    std::vector<FileFormat> loadMeshFormats(MeshTypeId mt) const
    {
        return mMeshIOActions[toUnderlying(mt)].loadFormats();
    }

    template<typename MeshType>
    std::vector<FileFormat> loadMeshFormats() const
    {
        return mMeshIOActions[toUnderlying(meshTypeId<MeshType>())]
            .loadFormats();
    }

    std::vector<FileFormat> loadMeshFormats() const
    {
        std::set<FileFormat> formats;
        for (uint i = 0; i < toUnderlying(MeshTypeId::COUNT); ++i) {
            auto meshFormats = mMeshIOActions[i].loadFormats();

            formats.insert(meshFormats.begin(), meshFormats.end());
        }
        return std::vector<FileFormat>(formats.begin(), formats.end());
    }

    std::shared_ptr<IOAction> loadMeshAction(FileFormat fmt, MeshTypeId mt)
        const
    {
        return mMeshIOActions[toUnderlying(mt)].loadAction(fmt);
    }

    template<typename MeshType>
    std::shared_ptr<MeshIOActionT<MeshType>> loadMeshAction(FileFormat fmt)
    {
        auto act =
            mMeshIOActions[toUnderlying(meshTypeId<MeshType>())].loadAction(
                fmt);

        return std::dynamic_pointer_cast<MeshIOActionT<MeshType>>(act);
    }

    ParameterVector loadMeshParameters(FileFormat fmt) const
    {
        std::shared_ptr<IOAction> act;
        for (uint i = 0; i < toUnderlying(MeshTypeId::COUNT); ++i) {
            if (mMeshIOActions[i].loadFormatExists(fmt)) {
                act = mMeshIOActions[i].loadAction(fmt);
            }
        }
        if (act) {
            switch (act->meshType()) {
            case MeshTypeId::TRIANGLE_MESH:
                return std::dynamic_pointer_cast<MeshIOActionT<TriEdgeMesh>>(
                           act)
                    ->parametersLoad(fmt);
            case MeshTypeId::POLYGON_MESH:
                return std::dynamic_pointer_cast<MeshIOActionT<PolyEdgeMesh>>(
                           act)
                    ->parametersLoad(fmt);
            default: return ParameterVector();
            }
        }
        else {
            throw std::runtime_error("The FileFormat is not supported.");
        }
    }

    // save mesh

    std::vector<FileFormat> saveMeshFormats(MeshTypeId mt) const
    {
        return mMeshIOActions[toUnderlying(mt)].saveFormats();
    }

    template<typename MeshType>
    std::vector<FileFormat> saveMeshFormats() const
    {
        return mMeshIOActions[toUnderlying(meshTypeId<MeshType>())]
            .saveFormats();
    }

    std::vector<FileFormat> saveMeshFormats() const
    {
        std::set<FileFormat> formats;
        for (uint i = 0; i < toUnderlying(MeshTypeId::COUNT); ++i) {
            auto meshFormats = mMeshIOActions[i].saveFormats();

            formats.insert(meshFormats.begin(), meshFormats.end());
        }
        return std::vector<FileFormat>(formats.begin(), formats.end());
    }

    std::shared_ptr<IOAction> saveMeshAction(FileFormat fmt, MeshTypeId mt)
        const
    {
        return mMeshIOActions[toUnderlying(mt)].saveAction(fmt);
    }

    template<typename MeshType>
    std::shared_ptr<MeshIOActionT<MeshType>> saveMeshAction(
        FileFormat fmt) const
    {
        auto act =
            mMeshIOActions[toUnderlying(meshTypeId<MeshType>())].saveAction(
                fmt);

        return std::dynamic_pointer_cast<MeshIOActionT<MeshType>>(act);
    }
};

} // namespace vcl::proc::detail

#endif // VCL_PROCESSING_MANAGER_ACTION_MANAGER_IO_MESH_MANAGER_H
