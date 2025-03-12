#ifndef AUTOMATION_ACTION_GROUP_H
#define AUTOMATION_ACTION_GROUP_H

#include <vector>

#include "automation_action.h"

class AutomationActionGroup : public AutomationAction
{
    std::vector<AutomationAction*> automations;
    using Parent = AutomationAction;

    public:

    void addAutomation(AutomationAction* automation)
    {
        automations.push_back(automation);
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
        for(size_t i=0; i<automations.size(); i++){
            if(automations[i]->isActive()){
                automations[i]->update();
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
};

class AutomationActionGroupBuilder
{
    AutomationActionGroup *group;

    public:

    AutomationActionGroupBuilder()
    {
        group = new AutomationActionGroup();
    }

    AutomationActionGroupBuilder *addAutomation(AutomationAction *automation)
    {
        group->addAutomation(automation);
        return this;
    }

    AutomationAction *finish()
    {
        return group;
    }
};

#endif