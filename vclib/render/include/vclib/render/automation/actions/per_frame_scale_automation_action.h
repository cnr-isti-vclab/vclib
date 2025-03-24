#ifndef VCL_PER_FRAME_SCALE_AUTOMATION_ACTION_H
#define VCL_PER_FRAME_SCALE_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/viewer/desktop_trackball.h>
#include <bx/bx.h>

namespace vcl{


/*
    An action that represent the scaling of a DesktopTrackball, with the intensity of the scaling
    calculated per frame
*/
class PerFrameScaleAutomationAction : public AbstractAutomationAction
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
    float pixelDeltaPerFrame;
    float totalPixelDelta;

    public:

    PerFrameScaleAutomationAction(DesktopTrackBall<float> *trackball, float pixelDeltaPerFrame)
    : trackball{trackball},
    pixelDeltaPerFrame{pixelDeltaPerFrame},
    totalPixelDelta{0}
    {}

    void start() override
    {
        Parent::start();
        notifyStarted();
        trackball->startIgnoringTrackBallEvents();
    }

    void doAction() override
    {
        Parent::doAction();
        totalPixelDelta += pixelDeltaPerFrame;

        trackball->performScale(totalPixelDelta);
    };

    void end() override
    {
        Parent::end();
        notifyEnded();
        if(!isAnyActive()){
            trackball->stopIgnoringTrackBallEvents();
        }
        totalPixelDelta = 0;
    };

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<PerFrameScaleAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<PerFrameScaleAutomationAction>(std::move(*this));
    }
};

}

#endif