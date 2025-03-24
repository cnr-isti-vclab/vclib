#ifndef SIMULTANEOUS_AUTOMATION_ACTIONS_H
#define SIMULTANEOUS_AUTOMATION_ACTIONS_H

#include <vclib/space/core/vector/polymorphic_object_vector.h>

#include "automation_action.h"

class SimultaneousAutomationActions : public AutomationAction
{
    vcl::PolymorphicObjectVector<AutomationAction> automations;
    using Parent = AutomationAction;

    public:
    
    SimultaneousAutomationActions(std::initializer_list<std::shared_ptr<AutomationAction>> init)
    {
        for(auto el = init.begin(); el < init.end(); el++)
        {
            automations.pushBack(*el);
        }
    };

    SimultaneousAutomationActions() {};

    void addAutomation(const AutomationAction &automation)
    {
        automations.pushBack(automation);
    }

    void start() override
    {
        Parent::start();
        for(size_t i=0; i<automations.size(); i++){
            automations[i]->start();
        }
    }

    void doAction() override
    {
        Parent::doAction();
        for (const auto aut : automations){
            if(aut->isActive()){
                aut->doAction();
            }
        }
    }

    void end() override
    {
        Parent::end();
        for(size_t i=0; i<automations.size(); i++){
            if(automations[i]->isActive()){
                automations[i]->end();
            }
        }
    }

    std::shared_ptr<AutomationAction> clone() const & override
    {
        return std::make_shared<SimultaneousAutomationActions>(*this);
    }

    std::shared_ptr<AutomationAction> clone() && override
    {
        return std::make_shared<SimultaneousAutomationActions>(std::move(*this));
    }
};

#endif