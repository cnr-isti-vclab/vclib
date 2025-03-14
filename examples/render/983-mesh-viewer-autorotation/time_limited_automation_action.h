#ifndef TIME_LIMITED_AUTOMATION_ACTION_H
#define TIME_LIMITED_AUTOMATION_ACTION_H

#include "automation_action.h"
#include <vclib/misc/timer.h>
#include <chrono>

class TimeLimitedAutomationAction : public AutomationAction
{
    using Parent = AutomationAction;

    AutomationAction *innerAction;
    float durationSeconds;
    vcl::Timer timer;


    public:

    TimeLimitedAutomationAction(AutomationAction* innerAction, float durationSeconds = 5.5f)
    : durationSeconds{durationSeconds},
    innerAction{innerAction}
    {};

    void start() override
    {
        Parent::start();
        timer.start();
        innerAction->start();
    }

    void update() override
    {   
        if(timer.delay() >= durationSeconds){
            end();
            return;
        }

        if(!innerAction->isActive()){
            return;
        }
        innerAction->update();
    }

    void end() override
    {
        Parent::end();
        innerAction->end();
        timer.stop();
    }
};

#endif