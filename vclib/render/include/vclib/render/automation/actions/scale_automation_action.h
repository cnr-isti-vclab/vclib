#ifndef SCALE_AUTOMATION_ACTION_H
#define SCALE_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/viewer/desktop_trackball.h>
#include <vclib/misc/timer.h>
#include <bx/bx.h>

namespace vcl{

class ScaleAutomationAction : public AbstractAutomationAction
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
    float pixelDeltaPerSecond;
    float totalPixelDelta;
    Timer timer;

    public:

    ScaleAutomationAction(DesktopTrackBall<float> *trackball, float pixelDeltaPerSecond)
    : trackball{trackball},
    pixelDeltaPerSecond{pixelDeltaPerSecond},
    totalPixelDelta{0}
    {}

    void start() override
    {
        Parent::start();
        notifyStarted();
        timer.start();
        trackball->startIgnoringTrackBallEvents();
    }

    void doAction() override
    {
        Parent::doAction();
        totalPixelDelta += pixelDeltaPerSecond * timer.delay();

        trackball->performScale(totalPixelDelta);

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
        totalPixelDelta = 0;
    };

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<ScaleAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<ScaleAutomationAction>(std::move(*this));
    }
};

}

#endif