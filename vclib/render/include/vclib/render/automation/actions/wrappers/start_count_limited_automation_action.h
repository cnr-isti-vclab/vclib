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

#ifndef VCL_START_COUNT_LIMITED_AUTOMATION_ACTION_H
#define VCL_START_COUNT_LIMITED_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>

namespace vcl {

/**
 * The StartCountLimitedAutomationAction is an automation allows you to add a
 * maximum amount of times start() is called (in terms of start() calls) to an
 * automation, so that after start() has been called for those many times the
 * automation is guaranteed to never be started again
 */
class StartCountLimitedAutomationAction : public WrapperAutomationAction
{
    using Parent = WrapperAutomationAction;

    uint32_t maximumStarts;
    uint32_t currentStarts = 0;

public:
    StartCountLimitedAutomationAction(
        const AbstractAutomationAction& innerAction,
        uint32_t                        maximumStarts) :
            Parent(innerAction), maximumStarts {maximumStarts} {};

    void start() override
    {
        if (currentStarts >= maximumStarts) {
            AbstractAutomationAction::start();
            return;
        }
        Parent::start();
        currentStarts++;
    };

    void doAction() override
    {
        Parent::doAction();
        if (!innerAction->isActive()) {
            end();
        }
    };

    std::shared_ptr<AbstractAutomationAction> clone() const& override
    {
        return std::make_shared<StartCountLimitedAutomationAction>(*this);
    };

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<StartCountLimitedAutomationAction>(
            std::move(*this));
    };
};

} // namespace vcl

#endif
