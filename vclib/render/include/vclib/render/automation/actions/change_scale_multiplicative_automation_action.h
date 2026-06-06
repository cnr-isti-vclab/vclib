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

#ifndef VCL_SCALE_MULTIPLICATIVE_AUTOMATION_ACTION_H
#define VCL_SCALE_MULTIPLICATIVE_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>

#include <vclib/base.h>

#include <iomanip>
#include <sstream>

namespace vcl {

/**
 * The ChangeScaleMultiplicativeAutomationAction is an automation that
 * represents a scaling in a multiplicative way, with the strength of the
 * scaling measured per-second
 *
 * @note Multiplicative scaling is represented by the following formula:
 * finalScale = initialScale * (1 + (deltaScale * duration))
 */
template<typename BmarkDrawer>
class ChangeScaleMultiplicativeAutomationAction :
        public AbstractAutomationAction<BmarkDrawer>
{
    using Parent = AbstractAutomationAction<BmarkDrawer>;
    using Parent::benchmarkDrawer;
    float mOriginalScale;
    float mPixelDeltaPerSecond;
    Timer mTimer;

public:
    ChangeScaleMultiplicativeAutomationAction(float pixelDeltaPerSecond) :
            mPixelDeltaPerSecond {pixelDeltaPerSecond}
    {
    }

    std::string getDescription() override
    {
        std::ostringstream temp;
        temp << "Multiplicative scale " << std::fixed << std::setprecision(3)
             << std::showpos << mPixelDeltaPerSecond << " per second";
        return temp.str();
    }

    void start() override
    {
        Parent::start();
        mOriginalScale = benchmarkDrawer->getScale();
        mTimer.start();
    }

    void doAction() override
    {
        Parent::doAction();
        benchmarkDrawer->changeScaleMultiplicative(
            mPixelDeltaPerSecond * mTimer.delay(), mOriginalScale);
        mTimer.start();
    };

    void end() override
    {
        Parent::end();
        mTimer.stop();
    };

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<
            ChangeScaleMultiplicativeAutomationAction<BmarkDrawer>>(*this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<
            ChangeScaleMultiplicativeAutomationAction<BmarkDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
