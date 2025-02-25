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

#include "io_action_container.h"

#include <vclib/processing/engine/action_interfaces.h>

namespace vcl::proc {

std::vector<std::shared_ptr<Action>> ioImageActions();
std::vector<std::shared_ptr<Action>> ioMeshActions();

namespace detail {

class Manager {
    static const uint MESH_TYPE_NUMBER = toUnderlying(MeshTypeId::COUNT);

    IOActionContainer mImageActions;
    std::array<IOActionContainer, MESH_TYPE_NUMBER> mMeshActions;

public:
    Manager() { addDefaultActions(); }

    void add(const std::shared_ptr<Action>& action)
    {
        using enum Action::Type;

        uint mt;

        std::shared_ptr<IOAction> ioImageAction;
        std::shared_ptr<IOAction> ioMeshAction;

        switch (action->type()) {
        case IO_IMAGE_ACTION:
            ioImageAction = std::dynamic_pointer_cast<IOAction>(action);
            mImageActions.add(ioImageAction);
            break;
        case IO_MESH_ACTION:
            checkMeshAction(action);
            ioMeshAction = std::dynamic_pointer_cast<IOAction>(action);
            mt = toUnderlying(action->meshType());
            mMeshActions[mt].add(ioMeshAction);
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

    void addDefaultActions()
    {
        add(ioImageActions());
        add(ioMeshActions());
    }

    std::shared_ptr<IOImageAction> loadImageAction(FileFormat fmt) const
    {
        return std::dynamic_pointer_cast<IOImageAction>(
            mImageActions.getLoad(fmt));
    }

    std::shared_ptr<IOImageAction> saveImageAction(FileFormat fmt) const
    {
        return std::dynamic_pointer_cast<IOImageAction>(
            mImageActions.getSave(fmt));
    }

private:
    void checkMeshAction(const std::shared_ptr<Action>& action)
    {
        if (action->meshType() >= MeshTypeId::COUNT) {
            throw std::runtime_error(
                "The Action MeshType is not supported by the ActionManager.");
        }
    }

};

} // namespace detail

} // namespace vcl::proc

#endif // VCL_PROCESSING_MANAGER_ACTION_MANAGER_MANAGER_H
