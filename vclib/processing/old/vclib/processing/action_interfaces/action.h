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

#ifndef VCL_PROCESSING_ACTION_INTERFACES_ACTION_H
#define VCL_PROCESSING_ACTION_INTERFACES_ACTION_H

#include <vclib/misc/logger.h>
#include <vclib/misc/string.h>
#include <vclib/processing/meshes/mesh_i.h>
#include <vclib/types.h>

#include <algorithm>
#include <memory>

namespace vcl::proc {

class ActionManager;

struct ActionType
{
    enum Enum {
        LOAD_IMAGE_ACTION = 0,
        SAVE_IMAGE_ACTION,
        LOAD_MESH_ACTION,
        SAVE_MESH_ACTION,
        FILTER_MESH_ACTION,
    };
};

class Action
{
    friend ActionManager;

    /**
     * @brief A pointer to the manager that contains the action.
     *
     * It could be used by the action to access and run other actions.
     */
    ActionManager* mManage = nullptr;

    inline static ConsoleLogger log;

public:
    Action()          = default;
    virtual ~Action() = default;

    virtual std::shared_ptr<Action> clone() const = 0;

    virtual std::string name() const = 0;

    virtual uint type() const = 0;

    std::string identifier() const { return identifierFromName(name()); }

    static std::string identifierFromName(const std::string& name)
    {
        std::string n = name;

        std::replace(n.begin(), n.end(), ' ', '_');
        n = vcl::toLower(n);

        return n;
    }

protected:
    void setManager(ActionManager* manager) { mManage = manager; }

    ActionManager* manager() const { return mManage; }

    static ConsoleLogger& logger() { return log; }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_INTERFACES_ACTION_H
