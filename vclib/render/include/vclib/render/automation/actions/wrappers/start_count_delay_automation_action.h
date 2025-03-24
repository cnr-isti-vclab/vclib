#ifndef VCL_START_COUNT_DELAY_AUTOMATION_ACTION_H
#define VCL_START_COUNT_DELAY_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>

namespace vcl{

/*
    Automation that allows you to add a delay (in terms of number of start() calls) to an action,
    so that it starts only after the delay has elapsed
*/
class StartCountDelayAutomationAction : public WrapperAutomationAction
{
    using Parent = WrapperAutomationAction;

    uint32_t waitStarts;
    uint32_t currentStarts = 0;

    public:

    StartCountDelayAutomationAction(const AbstractAutomationAction &innerAction, uint32_t waitStarts)
    : Parent(innerAction),
    waitStarts{waitStarts}
    {};

    void start() override
    {
        if(currentStarts < waitStarts)
        {
            AbstractAutomationAction::start();
            currentStarts++;
            return;
        }
        Parent::start();
    };

    void doAction() override
    {
        Parent::doAction();
        if(!innerAction->isActive())
        {
            end();
        }
    };

    void end() override
    {
        Parent::end();
    };

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<StartCountDelayAutomationAction>(*this);
    };

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<StartCountDelayAutomationAction>(std::move(*this));
    };
};

}

#endif