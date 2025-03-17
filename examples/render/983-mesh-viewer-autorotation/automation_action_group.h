#ifndef AUTOMATION_ACTION_GROUP_H
#define AUTOMATION_ACTION_GROUP_H

#include <vclib/space/core/vector/polymorphic_object_vector.h>

#include "automation_action.h"

class AutomationActionGroup : public AutomationAction
{
    vcl::PolymorphicObjectVector<AutomationAction> automations;
    using Parent = AutomationAction;

    public:

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

    void update() override
    {
        for (const auto aut : automations){
            if(aut->isActive()){
                aut->update();
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
        return std::make_shared<AutomationActionGroup>(*this);
    }

    std::shared_ptr<AutomationAction> clone() && override
    {
        return std::make_shared<AutomationActionGroup>(std::move(*this));
    }
};

class AutomationActionGroupBuilder
{
    AutomationActionGroup group;

    public:

    AutomationActionGroupBuilder()
    {
        group = AutomationActionGroup();
    }

    AutomationActionGroupBuilder addAutomation(const AutomationAction &automation)
    {
        group.addAutomation(automation);
        return *this;
    }

    AutomationActionGroup finish()
    {
        return group;
    }
};

#endif