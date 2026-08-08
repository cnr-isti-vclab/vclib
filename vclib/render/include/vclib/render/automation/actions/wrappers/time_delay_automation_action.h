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

#ifndef VCL_TIME_DELAY_AUTOMATION_ACTION_H
#define VCL_TIME_DELAY_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>

#include <vclib/base.h>

#include <iomanip>
#include <sstream>

namespace vcl {

/**
 * The TimeDelayAutomationAction is an automation that allows you to add a delay
 * (in terms of time) to an action, so that it can be started only after the
 * delay has elapsed
 */
template<typename BmarkEditor>
class TimeDelayAutomationAction : public WrapperAutomationAction<BmarkEditor>
{
    Timer mTimer;
    float mDelaySeconds;
    bool  mInnerStarted = false;
    using Parent        = WrapperAutomationAction<BmarkEditor>;
    using Parent::benchmarkEditor;
    using Parent::innerAction;

public:
    TimeDelayAutomationAction(
        const AbstractAutomationAction<BmarkEditor>& action,
        float delaySeconds) : Parent(action), mDelaySeconds {delaySeconds} {};

    void start() override
    {
        AbstractAutomationAction<BmarkEditor>::start();
        mTimer.start();
    }

    std::string getDescription() override
    {
        std::ostringstream temp;
        temp << "After " << std::fixed << std::setprecision(3) << mDelaySeconds
             << " seconds: " << innerAction->getDescription();
        return temp.str();
    }

    void doAction() override
    {
        AbstractAutomationAction<BmarkEditor>::doAction();
        if (mTimer.delay() < mDelaySeconds) {
            return;
        }
        if (!mInnerStarted) {
            innerAction->start();
            mInnerStarted = true;
        }
        if (innerAction->isActive()) {
            innerAction->doAction();
            return;
        }
        end();
    }

    void end() override
    {
        Parent::end();
        mTimer.stop();
        mInnerStarted = false;
    }

    std::shared_ptr<AbstractAutomationAction<BmarkEditor>> clone()
        const& override
    {
        return std::make_shared<TimeDelayAutomationAction<BmarkEditor>>(*this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkEditor>> clone() && override
    {
        return std::make_shared<TimeDelayAutomationAction<BmarkEditor>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
