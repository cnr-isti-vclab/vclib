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

#ifndef VCL_SEQUENTIAL_AUTOMATION_ACTIONS_H
#define VCL_SEQUENTIAL_AUTOMATION_ACTIONS_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/space/core/vector/polymorphic_object_vector.h>

namespace vcl{

/*
    An automation which represents a sequence of actions.
*/
class SequentialAutomationActions : public AbstractAutomationAction
{
    PolymorphicObjectVector<AbstractAutomationAction> automations;
    using Parent = AbstractAutomationAction;

    uint32_t currentIndex = 0;

    public:
    
    SequentialAutomationActions(std::initializer_list<std::shared_ptr<AbstractAutomationAction>> init)
    {
        for(auto el = init.begin(); el < init.end(); el++)
        {
            automations.pushBack(*el);
        }
    };

    SequentialAutomationActions() {};

    void addAutomation(const AbstractAutomationAction &automation)
    {
        automations.pushBack(automation);
    }

    void start() override
    {
        Parent::start();
        automations[0]->start();
    }

    void doAction() override
    {
        Parent::doAction();
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
            automations[currentIndex]->doAction();
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

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<SequentialAutomationActions>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<SequentialAutomationActions>(std::move(*this));
    }
};

}

#endif