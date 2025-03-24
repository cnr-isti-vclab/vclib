#ifndef SIMULTANEOUS_AUTOMATION_ACTIONS_H
#define SIMULTANEOUS_AUTOMATION_ACTIONS_H

#include <vclib/space/core/vector/polymorphic_object_vector.h>
#include <vclib/render/automation/actions/abstract_automation_action.h>

namespace vcl{

class SimultaneousAutomationActions : public AbstractAutomationAction
{
    PolymorphicObjectVector<AbstractAutomationAction> automations;
    using Parent = AbstractAutomationAction;

    public:
    
    SimultaneousAutomationActions(std::initializer_list<std::shared_ptr<AbstractAutomationAction>> init)
    {
        for(auto el = init.begin(); el < init.end(); el++)
        {
            automations.pushBack(*el);
        }
    };

    SimultaneousAutomationActions() {};

    void addAutomation(const AbstractAutomationAction &automation)
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

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<SimultaneousAutomationActions>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<SimultaneousAutomationActions>(std::move(*this));
    }
};

}

#endif