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

#ifndef VCL_ROTATION_AUTOMATION_ACTION_H
#define VCL_ROTATION_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/viewer/desktop_trackball.h>

#include <vclib/space/core/quaternion.h>

#include <vclib/misc/timer.h>

#include <numbers>

namespace vcl {

/**
 * The RotationAutomationAction is an automation that represents the
 * rotation of a DesktopTrackball, with the strength of the rotation measured
 * per-second
 */
class RotationAutomationAction : public AbstractAutomationAction
{
    using Parent = AbstractAutomationAction;
    DesktopTrackBall<float>* mTrackBall;
    float                    mRadiansPerSecond;
    Point3f                  mAround;
    Timer                    mTimer;

public:
    static RotationAutomationAction fromSecondsPerRotation(
        DesktopTrackBall<float>* trackBall,
        float                    secondsPerRotation,
        Point3f                  axis)
    {
        return RotationAutomationAction(
            trackBall,
            (2 * std::numbers::pi_v<float>) / secondsPerRotation,
            axis);
    }

    static RotationAutomationAction* ptrFromSecondsPerRotation(
        DesktopTrackBall<float>* trackBall,
        float                    secondsPerRotation,
        Point3f                  axis)
    {
        return new RotationAutomationAction(
            trackBall,
            (2 * std::numbers::pi_v<float>) / secondsPerRotation,
            axis);
    }

    RotationAutomationAction(
        DesktopTrackBall<float>* trackBall,
        float                    radiansPerSecond,
        Point3f                  axis) :
            mTrackBall {trackBall}, mRadiansPerSecond {radiansPerSecond},
            mAround {axis} {};

    void start() override
    {
        Parent::start();
        mTimer.start();
    };

    void doAction() override
    {
        Parent::doAction();
        auto rotation =
            Quaternion<float>(mRadiansPerSecond * mTimer.delay(), mAround);

        mTrackBall->rotate(rotation);

        mTimer.start();
    };

    void end() override
    {
        Parent::end();
        mTimer.stop();
    };

    std::shared_ptr<AbstractAutomationAction> clone() const& override
    {
        return std::make_shared<RotationAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<RotationAutomationAction>(std::move(*this));
    }
};

} // namespace vcl

#endif
