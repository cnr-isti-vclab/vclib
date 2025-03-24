#ifndef ROTATION_AUTOMATION_ACTION_H
#define ROTATION_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/viewer/desktop_trackball.h>
#include <vclib/space/core/quaternion.h>
#include <vclib/misc/timer.h>
#include <bx/bx.h>

namespace vcl{

class RotationAutomationAction: public AbstractAutomationAction
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

    using Parent = AbstractAutomationAction;
    DesktopTrackBall<float> *trackball;
    float radiansPerSecond;
    Point3f around;
    Timer timer;


    public:

    static RotationAutomationAction fromSecondsPerRotation(DesktopTrackBall<float> *trackball, float secondsPerRotation, Point3f axis)
    {
        return RotationAutomationAction(trackball, bx::kPi2 / secondsPerRotation, axis);
    }

    static RotationAutomationAction* ptrFromSecondsPerRotation(DesktopTrackBall<float> *trackball, float secondsPerRotation, Point3f axis)
    {
        return new RotationAutomationAction(trackball, bx::kPi2 / secondsPerRotation, axis);
    }

    RotationAutomationAction(DesktopTrackBall<float> *trackball, float radiansPerSecond, Point3f axis)
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
        auto rotation = Quaternion<float>(radiansPerSecond * timer.delay(), around);

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

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<RotationAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<RotationAutomationAction>(std::move(*this));
    }
};

}

#endif