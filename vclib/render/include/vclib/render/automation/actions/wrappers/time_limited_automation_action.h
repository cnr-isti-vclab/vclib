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

#ifndef VCL_TIME_LIMITED_AUTOMATION_ACTION_H
#define VCL_TIME_LIMITED_AUTOMATION_ACTION_H

#include <chrono>
#include <vclib/misc/timer.h>
#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>

namespace vcl {

/*
    Automation that allows you to add a maximum duration (in terms of time) to
   an automation, so that after the chosen duration has elapsed the automation
   is guaranteed to be over
*/
class TimeLimitedAutomationAction : public WrapperAutomationAction
{
    using Parent = WrapperAutomationAction;
    float durationSeconds;
    Timer timer;

public:
    TimeLimitedAutomationAction(
        const AbstractAutomationAction& innerAction,
        float                           durationSeconds = 5.5f) :
            Parent(innerAction), durationSeconds {durationSeconds} {};

    void start() override
    {
        timer.start();
        Parent::start();
    }

    void doAction() override
    {
        AbstractAutomationAction::doAction();
        if (timer.delay() >= durationSeconds) {
            end();
            return;
        }
        Parent::doAction();
    }

    void end() override
    {
        Parent::end();
        timer.stop();
    }

    std::shared_ptr<AbstractAutomationAction> clone() const& override
    {
        return std::make_shared<TimeLimitedAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<TimeLimitedAutomationAction>(std::move(*this));
    }
};

} // namespace vcl

#endif
