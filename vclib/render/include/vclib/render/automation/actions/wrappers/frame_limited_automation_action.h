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

#ifndef VCL_FRAME_LIMITED_AUTOMATION_ACTION_H
#define VCL_FRAME_LIMITED_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>

#include <iomanip>
#include <sstream>

namespace vcl {

/**
 * The FrameLimitedAutomationAction is an automation that allows you
 * to add a maximum duration (in terms of frames) to an automation, so that
 * after the chosen duration has elapsed the automation is guaranteed to be over
 */
template<typename BmarkDrawer>
class FrameLimitedAutomationAction : public WrapperAutomationAction<BmarkDrawer>
{
    using Parent = WrapperAutomationAction<BmarkDrawer>;
    using Parent::benchmarkDrawer;
    using Parent::innerAction;

    uint32_t mCurrentFrames = 0;
    uint32_t mDurationFrames;

public:
    FrameLimitedAutomationAction(
        const AbstractAutomationAction<BmarkDrawer>& innerAction,
        uint32_t                                     durationFrames = 400.f) :
            Parent(innerAction), mDurationFrames {durationFrames},
            mCurrentFrames {0} {};

    void start() override { Parent::start(); }

    std::string getDescription() override
    {
        std::ostringstream temp;
        temp << "For " << std::fixed << std::setprecision(3) << mDurationFrames
             << " frames: " << innerAction->getDescription();
        return temp.str();
    }

    void doAction() override
    {
        mCurrentFrames++;
        if (mCurrentFrames >= mDurationFrames) {
            end();
            return;
        }
        Parent::doAction();
    }

    void end() override
    {
        Parent::end();
        mCurrentFrames = 0;
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<FrameLimitedAutomationAction<BmarkDrawer>>(
            *this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<FrameLimitedAutomationAction<BmarkDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
