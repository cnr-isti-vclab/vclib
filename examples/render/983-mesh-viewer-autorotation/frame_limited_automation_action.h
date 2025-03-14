#ifndef FRAME_LIMITED_AUTOMATION_ACTION_H
#define FRAME_LIMITED_AUTOMATION_ACTION_H

#include "automation_action.h"
#include "wrapper_automation_action.h"

class FrameLimitedAutomationAction : public WrapperAutomationAction
{
    using Parent = WrapperAutomationAction;

    uint32_t currentFrames = 0;
    uint32_t durationFrames;

    public:

    FrameLimitedAutomationAction(AutomationAction* innerAction, uint32_t durationFrames = 400.f)
    : Parent(innerAction),
    durationFrames{durationFrames},
    currentFrames{0}
    {};

    void start()
    {
        Parent::start();
    }

    void update()
    {
        currentFrames++;
        if(currentFrames >= durationFrames){
            end();
            return;
        }
        Parent::update();
    }

    void end() override
    {
        Parent::end();
        currentFrames = 0;
    }
};

#endif