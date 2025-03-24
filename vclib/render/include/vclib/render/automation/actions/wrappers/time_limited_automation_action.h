#ifndef TIME_LIMITED_AUTOMATION_ACTION_H
#define TIME_LIMITED_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>
#include <vclib/misc/timer.h>
#include <chrono>

namespace vcl{

class TimeLimitedAutomationAction : public WrapperAutomationAction
{
    using Parent = WrapperAutomationAction;
    float durationSeconds;
    Timer timer;


    public:

    TimeLimitedAutomationAction(const AbstractAutomationAction &innerAction, float durationSeconds = 5.5f)
    : Parent(innerAction), 
    durationSeconds{durationSeconds}
    {};

    void start() override
    {
        timer.start();
        Parent::start();
    }

    void doAction() override
    {   
        AbstractAutomationAction::doAction();
        if(timer.delay() >= durationSeconds){
            end();
            return;
        }
        Parent::doAction();
    }

    void end() override
    {
        Parent::end();
        timer.stop();
    }

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<TimeLimitedAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<TimeLimitedAutomationAction>(std::move(*this));
    }
};

}

#endif