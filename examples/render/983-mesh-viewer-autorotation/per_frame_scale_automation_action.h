#ifndef PER_FRAME_SCALE_AUTOMATION_ACTION_H
#define PER_FRAME_SCALE_AUTOMATION_ACTION_H

#include "automation_action.h"
#include <vclib/render/viewer/desktop_trackball.h>
#include <bx/bx.h>

class PerFrameScaleAutomationAction : public AutomationAction
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
    float pixelDeltaPerFrame;
    float totalPixelDelta;

    public:

    PerFrameScaleAutomationAction(vcl::DesktopTrackBall<float> *trackball, float pixelDeltaPerFrame)
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

    std::shared_ptr<AutomationAction> clone() const & override
    {
        return std::make_shared<PerFrameScaleAutomationAction>(*this);
    }

    std::shared_ptr<AutomationAction> clone() && override
    {
        return std::make_shared<PerFrameScaleAutomationAction>(std::move(*this));
    }
};

#endif