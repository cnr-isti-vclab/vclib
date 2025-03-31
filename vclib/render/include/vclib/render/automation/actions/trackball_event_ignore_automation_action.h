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

#ifndef VCL_TRACKBALL_EVENT_IGNORE_AUTOMATION_ACTION_H
#define VCL_TRACKBALL_EVENT_IGNORE_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>

#include <vclib/render/viewer/desktop_trackball.h>

namespace vcl {

/**
 * The TrackballEventIgnoreAutomationAction is an automation which represents
 * the request to a DesktopTrackball to stop/start listening to events
 */
class TrackballEventIgnoreAutomationAction : public AbstractAutomationAction
{
    using Parent = AbstractAutomationAction;
    DesktopTrackBall<float>* trackball;
    bool                     ignoreEvents;

public:
    TrackballEventIgnoreAutomationAction(
        DesktopTrackBall<float>* trackball,
        bool                     ignoreEvents) :
            trackball {trackball}, ignoreEvents {ignoreEvents} {};

    void doAction() override
    {
        Parent::doAction();
        trackball->ignoreTrackBallEvents(ignoreEvents);
        end();
    };

    std::shared_ptr<AbstractAutomationAction> clone() const& override
    {
        return std::make_shared<TrackballEventIgnoreAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<TrackballEventIgnoreAutomationAction>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
