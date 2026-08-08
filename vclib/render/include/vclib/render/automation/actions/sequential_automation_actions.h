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
 * a sequence of automations.
 *
 * @note Since this counts as a single automation, the metrics will work as they
 * do for a single automation even though they are multiple
 */
template<typename BmarkEditor>
class SequentialAutomationActions : public AbstractAutomationAction<BmarkEditor>
{
    PolymorphicObjectVector<AbstractAutomationAction<BmarkEditor>> mAutomations;
    using Parent = AbstractAutomationAction<BmarkEditor>;
    using Parent::benchmarkEditor;

    uint mCurrentIndex = 0;

public:
    SequentialAutomationActions(
        std::initializer_list<
            std::shared_ptr<AbstractAutomationAction<BmarkEditor>>> init)
    {
        for (auto el = init.begin(); el < init.end(); el++) {
            (*el)->setBenchmarkEditor(this->benchmarkEditor);
            mAutomations.pushBack(*el);
        }
    };

    SequentialAutomationActions() {};

    std::string getDescription() override
    {
        std::ostringstream temp;
        temp << "Sequential[ ";
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

    void addAutomation(const AbstractAutomationAction<BmarkEditor>& automation)
    {
        automation.setBenchmarkEditor(this->benchmarkEditor);
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

    void setBenchmarkEditor(BmarkEditor* drawer) override
    {
        this->benchmarkEditor = drawer;
        for (auto& automation : mAutomations) {
            automation->setBenchmarkEditor(this->benchmarkEditor);
        }
    }

    std::shared_ptr<AbstractAutomationAction<BmarkEditor>> clone()
        const& override
    {
        return std::make_shared<SequentialAutomationActions<BmarkEditor>>(
            *this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkEditor>> clone() && override
    {
        return std::make_shared<SequentialAutomationActions<BmarkEditor>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
