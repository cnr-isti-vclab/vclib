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

#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>

#include <vclib/misc/timer.h>

#include <chrono>

namespace vcl {

/**
 * The TimeLimitedAutomationAction is an automation that allows you
 * to add a maximum duration (in terms of time) to an automation, so that after
 * the chosen duration has elapsed the automation is guaranteed to be over
 */
template<typename BmarkDrawer>
class TimeLimitedAutomationAction : public WrapperAutomationAction<BmarkDrawer>
{
    using Parent = WrapperAutomationAction<BmarkDrawer>;
    using Parent::benchmarkDrawer;
    using Parent::innerAction;
    float mDurationSeconds;
    Timer mTimer;

public:
    TimeLimitedAutomationAction(
        const AbstractAutomationAction<BmarkDrawer>& innerAction,
        float                                        durationSeconds = 5.5f) :
            Parent(innerAction), mDurationSeconds {durationSeconds} {};

    void start() override
    {
        mTimer.start();
        Parent::start();
    }

    void doAction() override
    {
        AbstractAutomationAction<BmarkDrawer>::doAction();
        if (mTimer.delay() >= mDurationSeconds) {
            end();
            return;
        }
        Parent::doAction();
    }

    void end() override
    {
        Parent::end();
        mTimer.stop();
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<TimeLimitedAutomationAction<BmarkDrawer>>(
            *this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<TimeLimitedAutomationAction<BmarkDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
