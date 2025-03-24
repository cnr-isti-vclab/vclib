#ifndef VCL_PER_FRAME_ROTATION_AUTOMATION_ACTION_H
#define VCL_PER_FRAME_ROTATION_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/viewer/desktop_trackball.h>
#include <vclib/space/core/quaternion.h>
#include <bx/bx.h>

namespace vcl{

/*
    An action that represent the rotation of a DesktopTrackball, with the intensity of the rotation
    calculated per frame
*/
class PerFrameRotationAutomationAction : public AbstractAutomationAction
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
    float radiansPerFrame;
    Point3f around;

    public:

    static PerFrameRotationAutomationAction fromFramesPerRotation(DesktopTrackBall<float> *trackball, float framesPerRotation, Point3f axis)
    { 
        return PerFrameRotationAutomationAction(trackball, bx::kPi2 / framesPerRotation, axis);
    }

    static PerFrameRotationAutomationAction* ptrFromFramesPerRotation(DesktopTrackBall<float> *trackball, float framesPerRotation, Point3f axis)
    {
        return new PerFrameRotationAutomationAction(trackball, bx::kPi2 / framesPerRotation, axis);
    }

    PerFrameRotationAutomationAction(DesktopTrackBall<float> *trackball, float radiansPerFrame, Point3f axis)
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

    void doAction() override
    {
        Parent::doAction();
        auto rotation = Quaternion<float>(radiansPerFrame, around);

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