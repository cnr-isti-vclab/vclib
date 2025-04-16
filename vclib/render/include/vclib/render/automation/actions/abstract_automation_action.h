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

#ifndef VCL_AUTOMATION_ACTION_H
#define VCL_AUTOMATION_ACTION_H

#include <chrono>
#include <exception>
#include <memory>

namespace vcl {

/**
 * The AbstractAutomationAction is a class that represents something to be done
 * by a BenchmarkDrawer. All classes that derive from this one should declare
 * AutomationAttorney as a friend
 */
template<typename BmarkDrawer>
class AbstractAutomationAction
{
    bool mActive = false;

    class UpdatedInactiveException : std::exception
    {
        const char* what() const throw() override
        {
            return "UpdatedInactiveException: Do not call doAction() "
                   "on inactive actions!\n";
        }
    };

protected:
    BmarkDrawer* benchmarkDrawer = NULL;

public:
    virtual void setBenchmarkDrawer(BmarkDrawer* drawer)
    {
        benchmarkDrawer = drawer;
    }

    virtual std::string getDescription() = 0;

    /**
     * Only ever called by outside sources
     *
     * An automation SHOULD NOT call start on itself during an update
     * A call to start SHOULD guarantee that the action is considered active at
     * least until the next doAction
     */
    virtual void start() { mActive = true; };

    /**
     * Called every frame (as long as the action is active)
     *
     * An automation may call end() on itself during a doAction(),
     * and it is therefore important to check whether the automation is active
     * before calling doAction()
     *
     * @throws UpdatedIncativeException If it is called on an inactive action
     */
    virtual void doAction()
    {
        if (!isActive()) {
            throw UpdatedInactiveException();
        }
    };

    /**
     * Either called by the automation itself or by outside sources
     *
     * This method ALSO has to reset everything in THIS object so that a
     * subsequent start() call is like calling start() on a new object equal to
     * this one (unless it makes no sense for the action to be started multiple
     * times)
     */
    virtual void end() { mActive = false; };

    virtual bool isActive() const { return mActive; }

    operator std::shared_ptr<AbstractAutomationAction<BmarkDrawer>>()
    {
        return clone();
    };

    virtual std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& = 0;
    virtual std::shared_ptr<AbstractAutomationAction<BmarkDrawer>>
    clone() && = 0;
};

} // namespace vcl

#endif
