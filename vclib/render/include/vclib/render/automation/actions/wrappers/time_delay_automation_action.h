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
#include <vclib/misc/timer.h>

namespace vcl{

/*
    Automation that allows you to add a delay (in terms of time) to an action,
    so that it can be started only after the delay has elapsed
*/
class TimeDelayAutomationAction : public WrapperAutomationAction
{
    Timer timer;
    float delaySeconds;
    bool innerStarted = false;
    using Parent = WrapperAutomationAction;

    public:

    TimeDelayAutomationAction(const AbstractAutomationAction &action, float delaySeconds)
    : Parent(action),
    delaySeconds{delaySeconds}
    {};

    void start() override
    {
        AbstractAutomationAction::start();
        timer.start();
    }

    void doAction() override
    {
        AbstractAutomationAction::doAction();
        if(timer.delay() < delaySeconds){
            return;
        }
        if(!innerStarted){
            innerAction->start();
            innerStarted = true;
        }
        if(innerAction->isActive()){
            innerAction->doAction();
            return;
        }
        end();
    }

    void end() override
    {
        Parent::end();
        timer.stop();
        innerStarted = false;
    }

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<TimeDelayAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<TimeDelayAutomationAction>(std::move(*this));
    }
};

}

#endif