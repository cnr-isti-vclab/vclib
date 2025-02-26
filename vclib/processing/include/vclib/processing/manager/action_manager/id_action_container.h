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

#ifndef VCL_PROCESSING_MANAGER_ACTION_MANAGER_ID_ACTION_CONTAINER_H
#define VCL_PROCESSING_MANAGER_ACTION_MANAGER_ID_ACTION_CONTAINER_H

#include <vclib/processing/engine/action_interfaces/action.h>

#include <map>

namespace vcl::proc {

class IDActionContainer
{
    using ActionMap = std::map<std::string, std::shared_ptr<Action>>;

    ActionMap mActionMap;

public:
    IDActionContainer() = default;

    void add(std::shared_ptr<Action> action)
    {
        if (!action) {
            throw std::runtime_error("Action is nullptr.");
        }
        checkActionDoesNotExist(action->name());
        mActionMap[action->name()] = action;
    }

    std::shared_ptr<Action> action(const std::string& name) const
    {
        auto it = findActionExists(name);
        return it->second;
    }

private:
    void checkActionDoesNotExist(const std::string& name) const
    {
        if (mActionMap.find(name) != mActionMap.end()) {
            throw std::runtime_error("Action " + name + " already registered.");
        }
    }

    ActionMap::const_iterator findActionExists(const std::string& name) const
    {
        auto it = mActionMap.find(name);
        if (it == mActionMap.end()) {
            throw std::runtime_error("Action " + name + " not registered.");
        }
        return it;
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_MANAGER_ACTION_MANAGER_ID_ACTION_CONTAINER_H
