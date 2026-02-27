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

#ifndef VCL_WRAPPER_AUTOMATION_ACTION_H
#define VCL_WRAPPER_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>

namespace vcl {

/**
 * The WrapperAutomationAction is a class that represents an automation whose
 * only purpose is to add functionality to another automation
 */
template<typename BmarkDrawer>
class WrapperAutomationAction : public AbstractAutomationAction<BmarkDrawer>
{
    using Parent = AbstractAutomationAction<BmarkDrawer>;

protected:
    using Parent::benchmarkDrawer;
    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> innerAction;

public:
    using Parent::getDescription;

    WrapperAutomationAction(
        const AbstractAutomationAction<BmarkDrawer>& action) :
            innerAction {action.clone()}
    {
    }

    void setBenchmarkDrawer(BmarkDrawer* drawer) override
    {
        this->benchmarkDrawer = drawer;
        innerAction->setBenchmarkDrawer(drawer);
    }

    void start() override
    {
        Parent::start();
        innerAction->start();
    }

    void doAction() override
    {
        Parent::doAction();
        if (!innerAction->isActive()) {
            return;
        }
        innerAction->doAction();
    };

    void end() override
    {
        if (innerAction->isActive()) {
            innerAction->end();
        }
        Parent::end();
    }
};

} // namespace vcl

#endif
