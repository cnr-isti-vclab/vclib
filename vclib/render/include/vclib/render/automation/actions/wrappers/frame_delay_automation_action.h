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

#ifndef VCL_FRAME_DELAY_AUTOMATION_ACTION_H
#define VCL_FRAME_DELAY_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>

namespace vcl {

/**
 * The TimeDelayAutomationAction is an automation that allows you to add a delay
 * (in terms of frames) to an action, so that it can be started only after the
 * delay has elapsed
 */
class FrameDelayAutomationAction : public WrapperAutomationAction
{
    uint32_t mCurrentFrames = 0;
    uint32_t mDelayFrames;
    bool     mInnerStarted = false;
    using Parent           = WrapperAutomationAction;

public:
    FrameDelayAutomationAction(
        const AbstractAutomationAction& action,
        uint32_t delayFrames) : Parent(action), mDelayFrames {delayFrames} {};

    void start() override { AbstractAutomationAction::start(); }

    void doAction() override
    {
        AbstractAutomationAction::doAction();
        if (mCurrentFrames < mDelayFrames) {
            mCurrentFrames++;
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
        mCurrentFrames = 0;
        mInnerStarted  = false;
    }

    std::shared_ptr<AbstractAutomationAction> clone() const& override
    {
        return std::make_shared<FrameDelayAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<FrameDelayAutomationAction>(std::move(*this));
    }
};

} // namespace vcl

#endif
