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

#ifndef VCL_NULL_AUTOMATION_ACTION_H
#define VCL_NULL_AUTOMATION_ACTION_H

#include "abstract_automation_action.h"
#include <chrono>
#include <exception>
#include <memory>

namespace vcl {

/**
 * The NullAutomationAction is an automation that does nothing
 */
template<typename BmarkDrawer>
class NullAutomationAction : public AbstractAutomationAction<BmarkDrawer>
{
public:
    std::string getDescription() override { return std::string("Do nothing"); }

    bool isNull() const override 
    {
        return true;
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<NullAutomationAction<BmarkDrawer>>(*this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<NullAutomationAction<BmarkDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
