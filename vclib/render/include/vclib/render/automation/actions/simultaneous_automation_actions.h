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

#include <vclib/render/automation/actions/abstract_automation_action.h>

#include <vclib/space/core/vector/polymorphic_object_vector.h>

namespace vcl {

/**
 * The SimultaneousAutomationActions class is an automation which represents
 * a group of actions executed "at the same time"
 *
 * @note Since this counts as a single action, the metrics will work as they do
 * for a single action even though they are multiple
 */
class SimultaneousAutomationActions : public AbstractAutomationAction
{
    PolymorphicObjectVector<AbstractAutomationAction> mAutomations;
    using Parent = AbstractAutomationAction;

public:
    SimultaneousAutomationActions(
        std::initializer_list<std::shared_ptr<AbstractAutomationAction>> init)
    {
        for (auto el = init.begin(); el < init.end(); el++) {
            mAutomations.pushBack(*el);
        }
    };

    SimultaneousAutomationActions() {};

    void addAutomation(const AbstractAutomationAction& automation)
    {
        mAutomations.pushBack(automation);
    }

    void start() override
    {
        Parent::start();
        for (size_t i = 0; i < mAutomations.size(); i++) {
            mAutomations[i]->start();
        }
    }

    void doAction() override
    {
        Parent::doAction();
        for (const auto aut : mAutomations) {
            if (aut->isActive()) {
                aut->doAction();
            }
        }
    }

    void end() override
    {
        Parent::end();
        for (size_t i = 0; i < mAutomations.size(); i++) {
            if (mAutomations[i]->isActive()) {
                mAutomations[i]->end();
            }
        }
    }

    std::shared_ptr<AbstractAutomationAction> clone() const& override
    {
        return std::make_shared<SimultaneousAutomationActions>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<SimultaneousAutomationActions>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
