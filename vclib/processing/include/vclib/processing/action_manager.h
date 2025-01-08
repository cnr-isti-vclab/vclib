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

#ifndef VCL_PROCESSING_ACTION_MANAGER_H
#define VCL_PROCESSING_ACTION_MANAGER_H

#include "action_interfaces.h"
#include "action_manager/identifier_action_manager.h"
#include "action_manager/io_action_manager.h"

namespace vcl::proc {

class ActionManager
{
    // load/save actions
    IOActionManager<LoadImageAction> mLoadImageActionManager;
    IOActionManager<LoadMeshAction>  mLoadMeshActionManager;
    IOActionManager<SaveImageAction> mSaveImageActionManager;
    IOActionManager<SaveMeshAction>  mSaveMeshActionManager;

    // filter/mesh
    IdentifierActionManager<FilterMeshAction> mFilterMeshActionManager;

public:
    void add(const std::shared_ptr<Action>& action)
    {
        std::shared_ptr<LoadImageAction> loadImageAction;
        std::shared_ptr<LoadMeshAction>  loadMeshAction;
        std::shared_ptr<SaveImageAction> saveImgAction;
        std::shared_ptr<SaveMeshAction>  saveMeshAction;

        std::shared_ptr<FilterMeshAction> filterMeshAction;

        switch (action->type()) {
        case ActionType::LOAD_IMAGE_ACTION:
            loadImageAction =
                std::dynamic_pointer_cast<LoadImageAction>(action);
            mLoadImageActionManager.add(loadImageAction);
            break;
        case ActionType::LOAD_MESH_ACTION:
            loadMeshAction = std::dynamic_pointer_cast<LoadMeshAction>(action);
            mLoadMeshActionManager.add(loadMeshAction);
            break;
        case ActionType::SAVE_IMAGE_ACTION:
            saveImgAction = std::dynamic_pointer_cast<SaveImageAction>(action);
            mSaveImageActionManager.add(saveImgAction);
            break;
        case ActionType::SAVE_MESH_ACTION:
            saveMeshAction = std::dynamic_pointer_cast<SaveMeshAction>(action);
            mSaveMeshActionManager.add(saveMeshAction);
            break;
        case ActionType::FILTER_MESH_ACTION:
            filterMeshAction =
                std::dynamic_pointer_cast<FilterMeshAction>(action);
            mFilterMeshActionManager.add(filterMeshAction);
            break;
        default: throw std::runtime_error("Action type not supported");
        }
        action->setManager(this);
    }

    template<vcl::Range R>
    requires vcl::RangeOf<R, std::shared_ptr<Action>>
    void add(R&& actions)
    {
        for (const auto& action : actions) {
            add(action);
        }
    }

    const IOActionManager<LoadImageAction>& loadImageActionManager() const
    {
        return mLoadImageActionManager;
    }

    const IOActionManager<LoadMeshAction>& loadMeshActionManager() const
    {
        return mLoadMeshActionManager;
    }

    const IOActionManager<SaveImageAction>& saveImageActionManager() const
    {
        return mSaveImageActionManager;
    }

    const IOActionManager<SaveMeshAction>& saveMeshActionManager() const
    {
        return mSaveMeshActionManager;
    }

    std::vector<FileFormat> loadImageFormats() const
    {
        return mLoadImageActionManager.formats();
    }

    std::vector<FileFormat> loadMeshFormats() const
    {
        return mLoadMeshActionManager.formats();
    }

    std::vector<FileFormat> saveImageFormats() const
    {
        return mSaveImageActionManager.formats();
    }

    std::vector<FileFormat> saveMeshFormats() const
    {
        return mSaveMeshActionManager.formats();
    }

    std::shared_ptr<LoadImageAction> loadImageAction(const FileFormat& format)
    {
        return mLoadImageActionManager.get(format);
    }

    std::shared_ptr<LoadMeshAction> loadMeshAction(const FileFormat& format)
    {
        return mLoadMeshActionManager.get(format);
    }

    std::shared_ptr<SaveImageAction> saveImageAction(const FileFormat& format)
    {
        return mSaveImageActionManager.get(format);
    }

    std::shared_ptr<SaveMeshAction> saveMeshAction(const FileFormat& format)
    {
        return mSaveMeshActionManager.get(format);
    }

    auto filterMeshActions() { return mFilterMeshActionManager.actions(); }

    std::shared_ptr<FilterMeshAction> filterMeshActionById(
        const std::string& id)
    {
        return mFilterMeshActionManager.get(id);
    }

    std::shared_ptr<FilterMeshAction> filterMeshActionByName(
        const std::string& name)
    {
        return mFilterMeshActionManager.getByName(name);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_MANAGER_H
