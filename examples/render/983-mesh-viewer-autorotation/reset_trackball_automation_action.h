#ifndef RESET_TRACKBALL_AUTOMATION_ACTION_H
#define RESET_TRACKBALL_AUTOMATION_ACTION_H

#include "automation_action.h"
#include <vclib/render/viewer/desktop_trackball.h>

class ResetTrackBallAutomationAction : public AutomationAction
{
    vcl::DesktopTrackBall<float> *trackball;
    using Parent = AutomationAction;

    public:

    ResetTrackBallAutomationAction(vcl::DesktopTrackBall<float> *trackball)
    : trackball{trackball}
    {}

    using Parent::start;
    using Parent::end;

    void update() override
    {
        trackball->resetTrackBall();
        end();
    }
};

#endif