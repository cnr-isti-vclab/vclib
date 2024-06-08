/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include "action_manager/io_action_manager.h"
#include "actions/interfaces.h"

namespace vcl::proc {

class ActionManager {
    IOActionManager<SaveImageAction> mSaveImageActionManager;
    IOActionManager<SaveMeshAction>  mSaveMeshActionManager;

public:
    void add(const std::shared_ptr<Action>& action) {
        std::shared_ptr<SaveImageAction> saveImgAction;
        std::shared_ptr<SaveMeshAction>  saveMeshAction;

        switch(action->type()) {
        case ActionType::SAVE_IMAGE_ACTION:
            saveImgAction = std::dynamic_pointer_cast<SaveImageAction>(action);
            mSaveImageActionManager.add(saveImgAction);
            break;
        case ActionType::SAVE_MESH_ACTION:
            saveMeshAction = std::dynamic_pointer_cast<SaveMeshAction>(action);
            mSaveMeshActionManager.add(saveMeshAction);
            break;
        default: throw std::runtime_error("Action type not supported");
        }
        action->setManager(this);
    }

    template<vcl::Range R> requires vcl::RangeOf<R, std::shared_ptr<Action>>
    void add(R&& actions) {
        for (const auto& action : actions) {
            add(action);
        }
    }

    std::shared_ptr<SaveImageAction> saveImageAction(
        const FileFormat& format)
    {
        return mSaveImageActionManager.get(format);
    }

    std::shared_ptr<SaveMeshAction> saveMeshAction(
        const FileFormat& format)
    {
        return mSaveMeshActionManager.get(format);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_MANAGER_H
