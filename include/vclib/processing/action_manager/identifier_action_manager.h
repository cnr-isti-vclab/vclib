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

#ifndef VCL_PROCESSING_ACTION_MANAGER_IDENTIFIER_ACTION_MANAGER_H
#define VCL_PROCESSING_ACTION_MANAGER_IDENTIFIER_ACTION_MANAGER_H

#include <map>

#include <vclib/processing/actions/interfaces/action.h>
#include <vclib/space/vector/polymorphic_object_vector.h>

namespace vcl::proc {

template<typename IDAction>
class IdentifierActionManager
{
    vcl::PolymorphicObjectVector<Action> mActions;

    std::map<std::string, std::shared_ptr<IDAction>> mIdActionMap;

public:
    IdentifierActionManager() = default;

    void add(std::shared_ptr<IDAction> action)
    {
        if (!action) {
            throw std::runtime_error("Action is nullptr.");
        }
        std::string id = action->identifier();
        checkIdDoesNotExist(id);
        mActions.pushBack(action);
        mIdActionMap[id] = std::dynamic_pointer_cast<IDAction>(mActions.back());
    }

    std::shared_ptr<IDAction> get(const std::string& id)
    {
        checkIdExists(id);
        return mIdActionMap[id];
    }

    std::shared_ptr<IDAction> getByName(const std::string& id)
    {
        return get(Action::identifierFromName(id));
    }

private:
    void checkIdDoesNotExist(const std::string& id)
    {
        if (mIdActionMap.find(id) != mIdActionMap.end()) {
            throw std::runtime_error("Action with the same id already exists.");
        }
    }

    void checkIdExists(const std::string& id)
    {
        if (mIdActionMap.find(id) == mIdActionMap.end()) {
            throw std::runtime_error("Action not found.");
        }
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_MANAGER_IDENTIFIER_ACTION_MANAGER_H
