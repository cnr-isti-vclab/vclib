#ifndef TIME_DELAY_AUTOMATION_ACTION_H
#define TIME_DELAY_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>
#include <vclib/misc/timer.h>

namespace vcl{

/*
    Automation that allows you to add a delay (in terms of time) to an action,
    so that it starts only after the delay has elapsed
*/
class TimeDelayAutomationAction : public WrapperAutomationAction
{
    Timer timer;
    float delaySeconds;
    bool innerStarted = false;
    using Parent = WrapperAutomationAction;

    public:

    TimeDelayAutomationAction(const AbstractAutomationAction &action, float delaySeconds)
    : Parent(action),
    delaySeconds{delaySeconds}
    {};

    void start() override
    {
        AbstractAutomationAction::start();
        timer.start();
    }

    void doAction() override
    {
        AbstractAutomationAction::doAction();
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

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<TimeDelayAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<TimeDelayAutomationAction>(std::move(*this));
    }
};

}

#endif