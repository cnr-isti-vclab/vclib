#ifndef VCL_WRAPPER_AUTOMATION_ACTION_H
#define VCL_WRAPPER_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>

namespace vcl{

class WrapperAutomationAction : public AbstractAutomationAction
{
    using Parent = AbstractAutomationAction;

    protected:

    std::shared_ptr<AbstractAutomationAction> innerAction;

    WrapperAutomationAction(const AbstractAutomationAction &action)
    : innerAction{action.clone()}
    {}

    void start() override
    {
        Parent::start();
        innerAction->start();
    }

    void doAction() override
    {
        Parent::doAction();
        if(!innerAction->isActive()){
            return;
        }
        innerAction->doAction();
    };

    void end() override
    {
        if(innerAction->isActive()){
            innerAction->end();
        }
        Parent::end();
    }
};

}

#endif