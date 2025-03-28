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

#include <numbers>
#include <vclib/misc/timer.h>
#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/viewer/desktop_trackball.h>
#include <vclib/space/core/quaternion.h>

namespace vcl {

/*
    An automation that represent the rotation of a DesktopTrackball, with the
   intensity of the rotation calculated using the time between frames
*/
class RotationAutomationAction : public AbstractAutomationAction
{
    using Parent = AbstractAutomationAction;
    DesktopTrackBall<float>* trackball;
    float                    radiansPerSecond;
    Point3f                  around;
    Timer                    timer;

public:
    static RotationAutomationAction fromSecondsPerRotation(
        DesktopTrackBall<float>* trackball,
        float                    secondsPerRotation,
        Point3f                  axis)
    {
        return RotationAutomationAction(
            trackball,
            (2 * std::numbers::pi_v<float>) / secondsPerRotation,
            axis);
    }

    static RotationAutomationAction* ptrFromSecondsPerRotation(
        DesktopTrackBall<float>* trackball,
        float                    secondsPerRotation,
        Point3f                  axis)
    {
        return new RotationAutomationAction(
            trackball,
            (2 * std::numbers::pi_v<float>) / secondsPerRotation,
            axis);
    }

    RotationAutomationAction(
        DesktopTrackBall<float>* trackball,
        float                    radiansPerSecond,
        Point3f                  axis) :
            trackball {trackball}, radiansPerSecond {radiansPerSecond},
            around {axis} {};

    void start() override
    {
        Parent::start();
        timer.start();
    };

    void doAction() override
    {
        Parent::doAction();
        auto rotation =
            Quaternion<float>(radiansPerSecond * timer.delay(), around);

        trackball->rotate(rotation);

        timer.start();
    };

    void end() override
    {
        Parent::end();
        timer.stop();
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
