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

#ifndef VCL_PER_FRAME_SCALE_AUTOMATION_ACTION_H
#define VCL_PER_FRAME_SCALE_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>

#include <iomanip>
#include <sstream>

namespace vcl {

/**
 * The PerFrameChangeScaleAbsoluteAutomationAction is an automation that
 * represents the scaling of a DesktopTrackball, with the strength of the
 * scaling measured per-frame
 */
template<typename BmarkDrawer>
class PerFrameChangeScaleAbsoluteAutomationAction :
        public AbstractAutomationAction<BmarkDrawer>
{
    using Parent = AbstractAutomationAction<BmarkDrawer>;
    using Parent::benchmarkDrawer;
    float mPixelDeltaPerFrame;

public:
    PerFrameChangeScaleAbsoluteAutomationAction(float pixelDeltaPerFrame) :
            mPixelDeltaPerFrame {pixelDeltaPerFrame}
    {
    }

    std::string getDescription() override
    {
        std::ostringstream temp;
        temp << "Absolute scale " << std::fixed << std::setprecision(3)
             << std::showpos << mPixelDeltaPerFrame << " per frame";
        return temp.str();
    }

    void doAction() override
    {
        Parent::doAction();
        benchmarkDrawer->scale(mPixelDeltaPerFrame);
    };

    void end() override { Parent::end(); };

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<
            PerFrameChangeScaleAbsoluteAutomationAction<BmarkDrawer>>(*this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<
            PerFrameChangeScaleAbsoluteAutomationAction<BmarkDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
