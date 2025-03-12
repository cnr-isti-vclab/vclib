#ifndef SCALE_AUTOMATION_ACTION_H
#define SCALE_AUTOMATION_ACTION_H

#include "automation_action.h"
#include <vclib/render/viewer/desktop_trackball.h>
#include <bx/bx.h>

class ScaleAutomationAction : public AutomationAction
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
    float pixelDeltaPerNS;
    float totalPixelDelta;

    public:

    ScaleAutomationAction(vcl::DesktopTrackBall<float> *trackball, float pixelDeltaPerSecond)
    : trackball{trackball},
    pixelDeltaPerNS{pixelDeltaPerSecond / 1e9f},
    totalPixelDelta{0}
    {}

    void start() override
    {
        Parent::start();
        notifyStarted();
        trackball->startIgnoringTrackBallEvents();
    }

    void update() override
    {
        std::chrono::high_resolution_clock::time_point now = 
            std::chrono::high_resolution_clock::now();
        
        auto deltaT = std::chrono::duration_cast<std::chrono::nanoseconds>(now-prev);

        totalPixelDelta += pixelDeltaPerNS * deltaT.count();

        trackball->performScale(totalPixelDelta);

        prev = now;
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
};

#endif