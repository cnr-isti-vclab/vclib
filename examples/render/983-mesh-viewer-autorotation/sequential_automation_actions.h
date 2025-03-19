#ifndef SEQUENTIAL_AUTOMATION_ACTIONS_H
#define SEQUENTIAL_AUTOMATION_ACTIONS_H

#include "automation_action.h"

#include <vclib/space/core/vector/polymorphic_object_vector.h>

class SequentialAutomationActions : public AutomationAction
{
    vcl::PolymorphicObjectVector<AutomationAction> automations;
    using Parent = AutomationAction;

    uint32_t currentIndex = 0;

    public:
    
    SequentialAutomationActions(std::initializer_list<std::shared_ptr<AutomationAction>> init)
    {
        for(auto el = init.begin(); el < init.end(); el++)
        {
            automations.pushBack(*el);
        }
    };

    SequentialAutomationActions() {};

    void addAutomation(const AutomationAction &automation)
    {
        automations.pushBack(automation);
    }

    void start() override
    {
        Parent::start();
        automations[0]->start();
    }

    void update() override
    {
        Parent::update();
        if(!automations[currentIndex]->isActive())
        {
            if(currentIndex == automations.size()-1)
            {
                end();
                return;
            }
            currentIndex++;
            automations[currentIndex]->start();
        }
        if(automations[currentIndex]->isActive()){
            automations[currentIndex]->update();
        }
    }

    void end() override
    {
        Parent::end();
        if(automations[currentIndex]->isActive())
        {
            automations[currentIndex]->end();
        }
        currentIndex = 0;
    }

    std::shared_ptr<AutomationAction> clone() const & override
    {
        return std::make_shared<SequentialAutomationActions>(*this);
    }

    std::shared_ptr<AutomationAction> clone() && override
    {
        return std::make_shared<SequentialAutomationActions>(std::move(*this));
    }
};

#endif