#ifndef TIME_DELAY_AUTOMATION_ACTION_H
#define TIME_DELAY_AUTOMATION_ACTION_H

#include "wrapper_automation_action.h"
#include <vclib/misc/timer.h>

class TimeDelayAutomationAction : public WrapperAutomationAction
{
    vcl::Timer timer;
    float delaySeconds;
    bool innerStarted = false;
    using Parent = WrapperAutomationAction;

    public:

    TimeDelayAutomationAction(const AutomationAction &action, float delaySeconds)
    : Parent(action),
    delaySeconds{delaySeconds}
    {};

    void start() override
    {
        AutomationAction::start();
        timer.start();
    }

    void doAction() override
    {
        AutomationAction::doAction();
        if(timer.delay() < delaySeconds){
            return;
        }
        if(!innerStarted){
            innerAction->start();
            innerStarted = true;
        }
        if(innerAction->isActive()){
            innerAction->doAction();
            return;
        }
        end();
    }

    void end() override
    {
        Parent::end();
        timer.stop();
        innerStarted = false;
    }

    std::shared_ptr<AutomationAction> clone() const & override
    {
        return std::make_shared<TimeDelayAutomationAction>(*this);
    }

    std::shared_ptr<AutomationAction> clone() && override
    {
        return std::make_shared<TimeDelayAutomationAction>(std::move(*this));
    }
};

#endif