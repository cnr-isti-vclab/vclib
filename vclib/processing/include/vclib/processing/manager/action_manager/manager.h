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

#ifndef VCL_PROCESSING_MANAGER_ACTION_MANAGER_MANAGER_H
#define VCL_PROCESSING_MANAGER_ACTION_MANAGER_MANAGER_H

#include "id_mesh_manager.h"
#include "io_image_manager.h"
#include "io_mesh_manager.h"

#include <vclib/processing/engine/action_interfaces.h>

namespace vcl::proc {

std::vector<std::shared_ptr<Action>> actionInstances();

namespace detail {

class Manager :
        public IDMeshManager,
        public IOMeshManager,
        public IOImageManager
{
public:
    Manager() { addDefaultActions(); }

    void add(const std::shared_ptr<Action>& action)
    {
        using enum Action::Type;

        uint mt;

        std::shared_ptr<ImageIOAction> ioImageAction;
        std::shared_ptr<MeshIOActions> ioMeshActions;
        std::shared_ptr<FilterActions> filterActions;

        switch (action->type()) {
        case IMAGE_IO_ACTION:
            ioImageAction = std::dynamic_pointer_cast<ImageIOAction>(action);
            IOImageManager::add(ioImageAction);
            break;
        case MESH_IO_ACTION:
            ioMeshActions = std::dynamic_pointer_cast<MeshIOActions>(action);
            IOMeshManager::add(ioMeshActions);
            break;
        case FILTER_ACTION:
            filterActions = std::dynamic_pointer_cast<FilterActions>(action);
            IDMeshManager::add(filterActions);
            break;
        default: throw std::runtime_error("Action type not supported");
        }
    }

    template<vcl::Range R>
    requires vcl::RangeOf<R, std::shared_ptr<Action>>
    void add(R&& actions)
    {
        for (const auto& action : actions) {
            add(action);
        }
    }

    void addDefaultActions() { add(actionInstances()); }
};

} // namespace detail

} // namespace vcl::proc

#endif // VCL_PROCESSING_MANAGER_ACTION_MANAGER_MANAGER_H
