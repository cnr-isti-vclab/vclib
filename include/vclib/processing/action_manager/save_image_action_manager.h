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

#ifndef VCL_PROCESSING_ACTION_MANAGER_SAVE_IMAGE_ACTION_MANAGER_H
#define VCL_PROCESSING_ACTION_MANAGER_SAVE_IMAGE_ACTION_MANAGER_H

#include <map>

#include <vclib/processing/actions/interfaces/save_image_action.h>
#include <vclib/space/polymorphic_object_vector.h>

namespace vcl::proc {

class SaveImageActionManager
{
    vcl::PolymorphicObjectVector<Action> mActions;

    std::map<std::string, std::shared_ptr<SaveImageAction>> mFormatMap;

public:
    SaveImageActionManager() = default;

    void add(SaveImageAction& action)
    {
        std::vector<FileFormat> formats = action.formats();

        for (const auto& format : formats) {
            for (const auto& ext : format.extensions()) {
                if (mFormatMap.find(ext) != mFormatMap.end()) {
                    throw std::runtime_error("Extension already registered.");
                }
                mActions.pushBack(action);
                mFormatMap[ext] =
                    std::dynamic_pointer_cast<SaveImageAction>(mActions.back());
            }
        }
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_MANAGER_SAVE_IMAGE_ACTION_MANAGER_H
