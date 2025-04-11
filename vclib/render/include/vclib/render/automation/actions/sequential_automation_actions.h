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

#include <sstream>

namespace vcl {

/**
 * The SequentialAutomationActions class is an automation which represents
 * a sequence of actions.
 *
 * @note Since this counts as a single action, the metrics will work as they do
 * for a single action even though they are multiple
 */
template<typename BmarkDrawer>
class SequentialAutomationActions : public AbstractAutomationAction<BmarkDrawer>
{
    PolymorphicObjectVector<AbstractAutomationAction<BmarkDrawer>> mAutomations;
    using Parent = AbstractAutomationAction<BmarkDrawer>;
    using Parent::benchmarkDrawer;

    uint32_t mCurrentIndex = 0;

public:
    SequentialAutomationActions(
        std::initializer_list<
            std::shared_ptr<AbstractAutomationAction<BmarkDrawer>>> init)
    {
        for (auto el = init.begin(); el < init.end(); el++) {
            (*el)->setBenchmarkDrawer(this->benchmarkDrawer);
            mAutomations.pushBack(*el);
        }
    };

    SequentialAutomationActions() {};

    std::string getDescription()
    {
        std::ostringstream temp;
        temp << "Sequential automations: [ ";
        bool isFirst = true;
        mAutomations[0];
        for (const auto& automation : mAutomations) {
            if (!isFirst) {
                temp << ", ";
            }
            isFirst = false;
            temp << automation->getDescription();
        }
        temp << " ]";
        return temp.str();
    }

    void addAutomation(const AbstractAutomationAction<BmarkDrawer>& automation)
    {
        automation.setBenchmarkDrawer(this->benchmarkDrawer);
        mAutomations.pushBack(automation);
    }

    void start() override
    {
        Parent::start();
        mAutomations[0]->start();
    }

    void doAction() override
    {
        Parent::doAction();
        if (!mAutomations[mCurrentIndex]->isActive()) {
            if (mCurrentIndex == mAutomations.size() - 1) {
                end();
                return;
            }
            mCurrentIndex++;
            mAutomations[mCurrentIndex]->start();
        }
        if (mAutomations[mCurrentIndex]->isActive()) {
            mAutomations[mCurrentIndex]->doAction();
        }
    }

    void end() override
    {
        Parent::end();
        if (mAutomations[mCurrentIndex]->isActive()) {
            mAutomations[mCurrentIndex]->end();
        }
        mCurrentIndex = 0;
    }

    void setBenchmarkDrawer(BmarkDrawer* drawer) override
    {
        this->benchmarkDrawer = drawer;
        for (auto& automation : mAutomations) {
            automation->setBenchmarkDrawer(this->benchmarkDrawer);
        }
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<SequentialAutomationActions<BmarkDrawer>>(
            *this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<SequentialAutomationActions<BmarkDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
