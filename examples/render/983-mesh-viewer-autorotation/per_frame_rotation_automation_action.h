#ifndef PER_FRAME_ROTATION_AUTOMATION_ACTION_H
#define PER_FRAME_ROTATION_AUTOMATION_ACTION_H

#include "automation_action.h"
#include <vclib/render/viewer/desktop_trackball.h>
#include <vclib/space/core/quaternion.h>
#include <bx/bx.h>

class PerFrameRotationAutomationAction : public AutomationAction
{
    static inline uint32_t activeCount = 0;

    static void notifyStarted()
    {
        activeCount++;
    }

    static void notifyEnded()
    {
        activeCount--;
    }

    static bool isAnyActive()
    {
        return activeCount != 0;
    }

    using Parent = AutomationAction;
    vcl::DesktopTrackBall<float> *trackball;
    float radiansPerFrame;
    vcl::Point3f around;

    public:

    static PerFrameRotationAutomationAction fromFramesPerRotation(vcl::DesktopTrackBall<float> *trackball, float framesPerRotation, vcl::Point3f axis)
    { 
        return PerFrameRotationAutomationAction(trackball, bx::kPi2 / framesPerRotation, axis);
    }

    static PerFrameRotationAutomationAction* ptrFromFramesPerRotation(vcl::DesktopTrackBall<float> *trackball, float framesPerRotation, vcl::Point3f axis)
    {
        return new PerFrameRotationAutomationAction(trackball, bx::kPi2 / framesPerRotation, axis);
    }

    PerFrameRotationAutomationAction(vcl::DesktopTrackBall<float> *trackball, float radiansPerFrame, vcl::Point3f axis)
    : trackball{trackball},
    radiansPerFrame{radiansPerFrame},
    around{axis}
    {};

    void start() override
    {
        Parent::start();
        notifyStarted();
        trackball->startIgnoringTrackBallEvents();
    };

    void update() override
    {
        auto rotation = vcl::Quaternion<float>(radiansPerFrame, around);

        trackball->rotate(rotation);
    };

    void end() override
    {
        Parent::end();
        notifyEnded();
        if(!isAnyActive()){
            trackball->stopIgnoringTrackBallEvents();
        }
    };

    std::shared_ptr<AutomationAction> clone() const & override
    {
        return std::make_shared<PerFrameRotationAutomationAction>(*this);
    }

    std::shared_ptr<AutomationAction> clone() && override
    {
        return std::make_shared<PerFrameRotationAutomationAction>(std::move(*this));
    }
};

#endif