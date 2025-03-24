#ifndef VCL_START_COUNT_LIMITED_AUTOMATION_ACTION_H
#define VCL_START_COUNT_LIMITED_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>

namespace vcl{

/*
    Automation that allows you to add a maximum duration (in terms of start() calls) to an automation,
    so that after the chosen duration has elapsed the automation is guaranteed to be over
*/
class StartCountLimitedAutomationAction : public WrapperAutomationAction
{
    using Parent = WrapperAutomationAction;

    uint32_t maximumStarts;
    uint32_t currentStarts = 0;

    public:

    StartCountLimitedAutomationAction(const AbstractAutomationAction &innerAction, uint32_t maximumStarts)
    : Parent(innerAction),
    maximumStarts{maximumStarts}
    {};

    void start() override
    {
        if(currentStarts >= maximumStarts)
        {
            AbstractAutomationAction::start();
            return;
        }
        Parent::start();
        currentStarts++;
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
        return std::make_shared<StartCountLimitedAutomationAction>(*this);
    };

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<StartCountLimitedAutomationAction>(std::move(*this));
    };
};

}

#endif