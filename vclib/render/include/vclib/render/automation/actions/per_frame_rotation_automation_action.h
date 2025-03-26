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
#include <vclib/render/viewer/desktop_trackball.h>
#include <vclib/space/core/quaternion.h>
#include <numbers>

namespace vcl{

/*
    An automation that represent the rotation of a DesktopTrackball, with the intensity of the rotation
    calculated per frame
*/
class PerFrameRotationAutomationAction : public AbstractAutomationAction
{
    using Parent = AbstractAutomationAction;
    DesktopTrackBall<float> *trackball;
    float radiansPerFrame;
    Point3f around;

    public:

    static PerFrameRotationAutomationAction fromFramesPerRotation(DesktopTrackBall<float> *trackball, float framesPerRotation, Point3f axis)
    { 
        return PerFrameRotationAutomationAction(trackball, (2*std::numbers::pi_v<float>) / framesPerRotation, axis);
    }

    static PerFrameRotationAutomationAction* ptrFromFramesPerRotation(DesktopTrackBall<float> *trackball, float framesPerRotation, Point3f axis)
    {
        return new PerFrameRotationAutomationAction(trackball, (2*std::numbers::pi_v<float>) / framesPerRotation, axis);
    }

    PerFrameRotationAutomationAction(DesktopTrackBall<float> *trackball, float radiansPerFrame, Point3f axis)
    : trackball{trackball},
    radiansPerFrame{radiansPerFrame},
    around{axis}
    {};

    void start() override
    {
        Parent::start();
        trackball->startIgnoringTrackBallEvents();
    };

    void doAction() override
    {
        Parent::doAction();
        auto rotation = Quaternion<float>(radiansPerFrame, around);

        trackball->rotate(rotation);
    };

    void end() override
    {
        Parent::end();
    };

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<PerFrameRotationAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<PerFrameRotationAutomationAction>(std::move(*this));
    }
};

}

#endif