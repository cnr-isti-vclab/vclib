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

#include <iomanip>
#include <sstream>

namespace vcl {

/**
 * The FrameDelayAutomationAction is an automation that allows you to add a
 * delay (in terms of frames) to an action, so that it can be started only after
 * the delay has elapsed
 */
template<typename BmarkDrawer>
class FrameDelayAutomationAction : public WrapperAutomationAction<BmarkDrawer>
{
    uint32_t mCurrentFrames = 0;
    uint32_t mDelayFrames;
    bool     mInnerStarted = false;
    using Parent           = WrapperAutomationAction<BmarkDrawer>;
    using Parent::benchmarkDrawer;
    using Parent::innerAction;

public:
    FrameDelayAutomationAction(
        const AbstractAutomationAction<BmarkDrawer>& action,
        uint32_t delayFrames) : Parent(action), mDelayFrames {delayFrames} {};

    void start() override { AbstractAutomationAction<BmarkDrawer>::start(); }

    std::string getDescription()
    {
        std::ostringstream temp;
        temp << "After " << std::fixed << std::setprecision(3) << mDelayFrames
             << " frames: " << innerAction->getDescription();
        return temp.str();
    }

    void doAction() override
    {
        AbstractAutomationAction<BmarkDrawer>::doAction();
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

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<FrameDelayAutomationAction<BmarkDrawer>>(*this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<FrameDelayAutomationAction<BmarkDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
