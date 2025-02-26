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

#include "id_action_container.h"
#include "io_action_container.h"

#include <vclib/processing/engine/action_interfaces.h>

namespace vcl::proc {

std::vector<std::shared_ptr<Action>> actionInstances();

namespace detail {

class Manager {
    static const uint MESH_TYPE_NUMBER = toUnderlying(MeshTypeId::COUNT);

    IOActionContainer mImageIOActions;
    std::array<IOActionContainer, MESH_TYPE_NUMBER> mMeshIOActions;
    std::array<IDActionContainer, MESH_TYPE_NUMBER> mFilterActions;

public:
    Manager() { addDefaultActions(); }

    void add(const std::shared_ptr<Action>& action)
    {
        using enum Action::Type;

        uint mt;

        std::shared_ptr<IOAction> ioImageAction;
        std::shared_ptr<IOAction> ioMeshAction;

        switch (action->type()) {
        case IMAGE_IO_ACTION:
            ioImageAction = std::dynamic_pointer_cast<IOAction>(action);
            mImageIOActions.add(ioImageAction);
            break;
        case MESH_IO_ACTION:
            checkMeshAction(action);
            ioMeshAction = std::dynamic_pointer_cast<IOAction>(action);
            mt = toUnderlying(action->meshType());
            mMeshIOActions[mt].add(ioMeshAction);
            break;
        case FILTER_ACTION:
            checkMeshAction(action);
            mt = toUnderlying(action->meshType());
            mFilterActions[mt].add(action);
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
        add(actionInstances());
    }

    // load image

    std::vector<FileFormat> loadImageFormats() const
    {
        return mImageIOActions.loadFormats();
    }

    std::shared_ptr<ImageIOAction> loadImageAction(FileFormat fmt) const
    {
        return std::dynamic_pointer_cast<ImageIOAction>(
            mImageIOActions.loadAction(fmt));
    }

    // load mesh

    std::vector<FileFormat> loadMeshFormats(MeshTypeId mt) const
    {
        return mMeshIOActions[toUnderlying(mt)].loadFormats();
    }

    template<typename MeshType>
    std::vector<FileFormat> loadMeshFormats() const
    {
        return mMeshIOActions[toUnderlying(meshTypeId<MeshType>())].loadFormats();
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
    std::shared_ptr<MeshIOAction<MeshType>> loadMeshAction(FileFormat fmt)
    {
        auto act =
            mMeshIOActions[toUnderlying(meshTypeId<MeshType>())].loadAction(fmt);

        return std::dynamic_pointer_cast<MeshIOAction<MeshType>>(act);
    }

    // save image

    std::vector<FileFormat> saveImageFormats() const
    {
        return mImageIOActions.saveFormats();
    }

    std::shared_ptr<ImageIOAction> saveImageAction(FileFormat fmt) const
    {
        return std::dynamic_pointer_cast<ImageIOAction>(
            mImageIOActions.saveAction(fmt));
    }

    // save mesh

    std::vector<FileFormat> saveMeshFormats(MeshTypeId mt) const
    {
        return mMeshIOActions[toUnderlying(mt)].saveFormats();
    }

    template<typename MeshType>
    std::vector<FileFormat> saveMeshFormats() const
    {
        return mMeshIOActions[toUnderlying(meshTypeId<MeshType>())].saveFormats();
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
    std::shared_ptr<MeshIOAction<MeshType>> saveMeshAction(
        FileFormat fmt) const
    {
        auto act =
            mMeshIOActions[toUnderlying(meshTypeId<MeshType>())].saveAction(fmt);

        return std::dynamic_pointer_cast<MeshIOAction<MeshType>>(act);
    }

    // filter

    std::shared_ptr<Action> filterAction(const std::string& name, MeshTypeId mt)
        const
    {
        return mFilterActions[toUnderlying(mt)].action(name);
    }

    template<typename MeshType>
    std::shared_ptr<FilterAction<MeshType>> filterAction(
        const std::string& name)
    {
        auto act =
            mFilterActions[toUnderlying(meshTypeId<MeshType>())].action(name);

        return std::dynamic_pointer_cast<FilterAction<MeshType>>(act);
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
