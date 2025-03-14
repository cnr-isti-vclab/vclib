#ifndef TIME_LIMITED_AUTOMATION_ACTION_H
#define TIME_LIMITED_AUTOMATION_ACTION_H

#include "wrapper_automation_action.h"
#include <vclib/misc/timer.h>
#include <chrono>

class TimeLimitedAutomationAction : public WrapperAutomationAction
{
    using Parent = WrapperAutomationAction;
    float durationSeconds;
    vcl::Timer timer;


    public:

    TimeLimitedAutomationAction(AutomationAction* innerAction, float durationSeconds = 5.5f)
    : Parent(innerAction), 
    durationSeconds{durationSeconds}
    {};

    void start() override
    {
        timer.start();
        Parent::start();
    }

    void update() override
    {   
        if(timer.delay() >= durationSeconds){
            end();
            return;
        }
        Parent::update();
    }

    void end() override
    {
        Parent::end();
        timer.stop();
    }
};

#endif