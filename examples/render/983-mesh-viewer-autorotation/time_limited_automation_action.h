#ifndef TIME_LIMITED_AUTOMATION_ACTION_H
#define TIME_LIMITED_AUTOMATION_ACTION_H

#include "automation_action.h"
#include <chrono>

class TimeLimitedAutomationAction : public AutomationAction
{
    using Parent = AutomationAction;

    AutomationAction *innerAction;
    std::chrono::high_resolution_clock::time_point startTime;
    float durationSeconds;
    bool firstUpdate = true;

    public:

    TimeLimitedAutomationAction(AutomationAction* innerAction, float durationSeconds = 5.5f)
    : durationSeconds{durationSeconds},
    innerAction{innerAction}
    {};

    void start()
    {
        Parent::start();
        innerAction->start();
    }

    void update()
    {
        if(firstUpdate)
        {
            startTime = std::chrono::high_resolution_clock::now();
            firstUpdate = false;
        }
        float timePassedSeconds = 
            (float)std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - startTime
            )
            .count() 
            / 
            1e3f;
        
        if(timePassedSeconds >= durationSeconds){
            end();
            return;
        }

        innerAction->update();
    }

    void end() override
    {
        Parent::end();
        innerAction->end();
        firstUpdate = true;
        
    }
};

#endif