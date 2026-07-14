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

#ifndef VCL_START_COUNT_DELAY_AUTOMATION_ACTION_H
#define VCL_START_COUNT_DELAY_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>

#include <iomanip>
#include <sstream>

namespace vcl {

/**
 * The StartCountDelayAutomationAction is an automation allows you to add a
 * delay (in terms of number of start() calls) to an action, so that the inner
 * automation can be started only after the delay has elapsed
 *
 * @note This is substantially the same as "executing only after a certain
 * number of loops have passed"
 */
template<typename BmarkEditor>
class StartCountDelayAutomationAction :
        public WrapperAutomationAction<BmarkEditor>
{
    using Parent = WrapperAutomationAction<BmarkEditor>;
    using Parent::benchmarkEditor;
    using Parent::innerAction;

    uint mWaitStarts;
    uint mCurrentStarts = 0;

public:
    StartCountDelayAutomationAction(
        const AbstractAutomationAction<BmarkEditor>& innerAction,
        uint waitStarts) : Parent(innerAction), mWaitStarts {waitStarts} {};

    void start() override
    {
        if (mCurrentStarts < mWaitStarts) {
            AbstractAutomationAction<BmarkEditor>::start();
            mCurrentStarts++;
            return;
        }
        Parent::start();
    };

    std::string getDescription() override
    {
        std::ostringstream temp;
        temp << "After " << std::fixed << std::setprecision(3)
             << mWaitStarts - mCurrentStarts
             << " loops: " << innerAction->getDescription();
        return temp.str();
    }

    void doAction() override
    {
        Parent::doAction();
        if (!innerAction->isActive()) {
            end();
        }
    };

    void end() override { Parent::end(); }

    std::shared_ptr<AbstractAutomationAction<BmarkEditor>> clone()
        const& override
    {
        return std::make_shared<StartCountDelayAutomationAction<BmarkEditor>>(
            *this);
    };

    std::shared_ptr<AbstractAutomationAction<BmarkEditor>> clone() && override
    {
        return std::make_shared<StartCountDelayAutomationAction<BmarkEditor>>(
            std::move(*this));
    };
};

} // namespace vcl

#endif
