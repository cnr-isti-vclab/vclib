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

#ifndef VCL_PER_FRAME_ROTATION_AUTOMATION_ACTION_H
#define VCL_PER_FRAME_ROTATION_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>

#include <vclib/space/core/quaternion.h>

#include <numbers>

namespace vcl {

/**
 * The PerFrameRotationAutomationAction is an automation that represents the
 * rotation of a DesktopTrackball, with the strength of the rotation measured
 * per-frame
 */
template<typename BmarkDrawer>
class PerFrameRotationAutomationAction :
        public AbstractAutomationAction<BmarkDrawer>
{
    using Parent = AbstractAutomationAction<BmarkDrawer>;
    using Parent::benchmarkDrawer;
    float   mRadiansPerFrame;
    Point3f mAround;

public:
    /**
     * Method to construct an object from this class by specifing how many
     * frames you want an entire rotation (2*pi) to take
     */
    static PerFrameRotationAutomationAction fromFramesPerRotation(
        float   framesPerRotation,
        Point3f axis)
    {
        return PerFrameRotationAutomationAction(
            (2 * std::numbers::pi_v<float>) / framesPerRotation, axis);
    }

    /**
     * Method to construct an object from this class by specifing how many
     * frames you want an entire rotation (2*pi) to take. It returns a pointer
     * to a heap allocated object
     */
    static PerFrameRotationAutomationAction* ptrFromFramesPerRotation(
        float   framesPerRotation,
        Point3f axis)
    {
        return new PerFrameRotationAutomationAction(
            (2 * std::numbers::pi_v<float>) / framesPerRotation, axis);
    }

    PerFrameRotationAutomationAction(float radiansPerFrame, Point3f axis) :
            mRadiansPerFrame {radiansPerFrame}, mAround {axis} {};

    void doAction() override
    {
        Parent::doAction();
        auto rotation = Quaternion<float>(mRadiansPerFrame, mAround);
        benchmarkDrawer->rotate(rotation);
    };

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<PerFrameRotationAutomationAction<BmarkDrawer>>(
            *this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<PerFrameRotationAutomationAction<BmarkDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
