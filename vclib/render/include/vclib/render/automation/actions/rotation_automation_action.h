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

#include <vclib/space/core/quaternion.h>

#include <vclib/misc/timer.h>

#include <numbers>

namespace vcl {

/**
 * The RotationAutomationAction is an automation that represents the
 * rotation of a DesktopTrackball, with the strength of the rotation measured
 * per-second
 */
template<typename BmarkDrawer>
class RotationAutomationAction : public AbstractAutomationAction<BmarkDrawer>
{
    using Parent = AbstractAutomationAction<BmarkDrawer>;
    using Parent::benchmarkDrawer;
    float   mRadiansPerSecond;
    Point3f mAround;
    Timer   mTimer;

public:
    /**
     * Method to construct an object from this class by specifing how many
     * frames you want an entire rotation (2*pi) to take
     */
    static RotationAutomationAction fromSecondsPerRotation(
        float   secondsPerRotation,
        Point3f axis)
    {
        return RotationAutomationAction(
            (2 * std::numbers::pi_v<float>) / secondsPerRotation, axis);
    }

    /**
     * Method to construct an object from this class by specifing how many
     * frames you want an entire rotation (2*pi) to take. It returns a pointer
     * to a heap allocated object.
     */
    static RotationAutomationAction* ptrFromSecondsPerRotation(
        float   secondsPerRotation,
        Point3f axis)
    {
        return new RotationAutomationAction(
            (2 * std::numbers::pi_v<float>) / secondsPerRotation, axis);
    }

    RotationAutomationAction(float radiansPerSecond, Point3f axis) :
            mRadiansPerSecond {radiansPerSecond}, mAround {axis} {};

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

        benchmarkDrawer->rotate(rotation);

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
        return std::make_shared<RotationAutomationAction<BmarkDrawer>>(*this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<RotationAutomationAction<BmarkDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
