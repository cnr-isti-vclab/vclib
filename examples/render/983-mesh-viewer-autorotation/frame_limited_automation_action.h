#ifndef FRAME_LIMITED_AUTOMATION_ACTION_H
#define FRAME_LIMITED_AUTOMATION_ACTION_H

#include "automation_action.h"
#include <chrono>

class FrameLimitedAutomationAction : public AutomationAction
{
    using Parent = AutomationAction;

    AutomationAction *innerAction;
    uint32_t currentFrames;
    uint32_t durationFrames;

    public:

    FrameLimitedAutomationAction(AutomationAction* innerAction, uint32_t durationFrames = 400.f)
    : durationFrames{durationFrames},
    currentFrames{0},
    innerAction{innerAction}
    {};

    void start()
    {
        Parent::start();
        innerAction->start();
    }

    void update()
    {
        currentFrames++;
        if(currentFrames >= durationFrames){
            end();
            return;
        }
        innerAction->update();
    }

    void end() override
    {
        Parent::end();
        innerAction->end();
        currentFrames = 0;
    }
};

#endif