/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_SIMULTANEOUS_AUTOMATION_ACTIONS_H
#define VCL_SIMULTANEOUS_AUTOMATION_ACTIONS_H

#include <vclib/space/core/vector/polymorphic_object_vector.h>
#include <vclib/render/automation/actions/abstract_automation_action.h>

namespace vcl{


/*
    An automation which represents a group of action executed "simultaneously". Keep in mind
    that since this is considered as a single action, metrics are only measured
    once for the entire group
*/
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