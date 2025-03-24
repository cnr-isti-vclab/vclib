#ifndef ROTATION_AUTOMATION_ACTION_H
#define ROTATION_AUTOMATION_ACTION_H

#include "automation_action.h"
#include <vclib/render/viewer/desktop_trackball.h>
#include <vclib/space/core/quaternion.h>
#include <vclib/misc/timer.h>
#include <bx/bx.h>

class RotationAutomationAction: public AutomationAction
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
    float radiansPerSecond;
    vcl::Point3f around;
    vcl::Timer timer;


    public:

    static RotationAutomationAction fromSecondsPerRotation(vcl::DesktopTrackBall<float> *trackball, float secondsPerRotation, vcl::Point3f axis)
    {
        return RotationAutomationAction(trackball, bx::kPi2 / secondsPerRotation, axis);
    }

    static RotationAutomationAction* ptrFromSecondsPerRotation(vcl::DesktopTrackBall<float> *trackball, float secondsPerRotation, vcl::Point3f axis)
    {
        return new RotationAutomationAction(trackball, bx::kPi2 / secondsPerRotation, axis);
    }

    RotationAutomationAction(vcl::DesktopTrackBall<float> *trackball, float radiansPerSecond, vcl::Point3f axis)
    : trackball{trackball},
    radiansPerSecond{radiansPerSecond},
    around{axis}
    {};

    void start() override
    {
        Parent::start();
        notifyStarted();
        timer.start();
        trackball->startIgnoringTrackBallEvents();
    };

    void doAction() override
    {
        Parent::doAction();
        auto rotation = vcl::Quaternion<float>(radiansPerSecond * timer.delay(), around);

        trackball->rotate(rotation);

        timer.start();
    };

    void end() override
    {
        Parent::end();
        notifyEnded();
        timer.stop();
        if(!isAnyActive()){
            trackball->stopIgnoringTrackBallEvents();
        }
    };

    std::shared_ptr<AutomationAction> clone() const & override
    {
        return std::make_shared<RotationAutomationAction>(*this);
    }

    std::shared_ptr<AutomationAction> clone() && override
    {
        return std::make_shared<RotationAutomationAction>(std::move(*this));
    }
};

#endif