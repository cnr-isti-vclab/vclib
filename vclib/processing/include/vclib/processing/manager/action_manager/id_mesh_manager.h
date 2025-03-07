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

#ifndef VCL_PROCESSING_MANAGER_ACTION_MANAGER_ID_MESH_MANAGER_H
#define VCL_PROCESSING_MANAGER_ACTION_MANAGER_ID_MESH_MANAGER_H

#include "id_action_container.h"

#include <vclib/processing/engine/action_interfaces.h>

namespace vcl::proc::detail {

class IDMeshManager
{
    static const uint MESH_TYPE_NUMBER = toUnderlying(MeshTypeId::COUNT);

    std::array<IDActionContainer, MESH_TYPE_NUMBER> mFilterActions;

protected:
    void add(const std::shared_ptr<Action>& action)
    {
        uint mt = toUnderlying(action->meshType());
        mFilterActions[mt].add(action);
    }

public:
    // filter

    std::shared_ptr<Action> filterAction(const std::string& name, MeshTypeId mt)
        const
    {
        return mFilterActions[toUnderlying(mt)].action(name);
    }

    template<typename MeshType>
    std::shared_ptr<FilterActionT<MeshType>> filterAction(
        const std::string& name)
    {
        auto act =
            mFilterActions[toUnderlying(meshTypeId<MeshType>())].action(name);

        return std::dynamic_pointer_cast<FilterActionT<MeshType>>(act);
    }
};

} // namespace vcl::proc::detail

#endif // VCL_PROCESSING_MANAGER_ACTION_MANAGER_ID_MESH_MANAGER_H
