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

#ifndef VCL_PROCESSING_MANAGER_ACTION_MANAGER_H
#define VCL_PROCESSING_MANAGER_ACTION_MANAGER_H

#include "action_manager/manager.h"

namespace vcl::proc {

class ActionManager
{
public:
    static void add(const std::shared_ptr<Action>& action)
    {
        instance().add(action);
    }

    template<vcl::Range R>
    requires vcl::RangeOf<R, std::shared_ptr<Action>>
    static void add(R&& actions)
    {
        instance().add(std::forward<R>(actions));
    }

    static std::shared_ptr<IOImageAction> loadImageAction(FileFormat fmt)
    {
        return instance().saveImageAction(fmt);
    }

    static std::shared_ptr<IOImageAction> saveImageAction(FileFormat fmt)
    {
        return instance().loadImageAction(fmt);
    }

private:
    static detail::Manager& instance()
    {
        static detail::Manager instance;
        return instance;
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_MANAGER_ACTION_MANAGER_H
