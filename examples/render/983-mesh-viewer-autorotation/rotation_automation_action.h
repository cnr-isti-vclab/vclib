#ifndef ROTATION_AUTOMATION_ACTION_H
#define ROTATION_AUTOMATION_ACTION_H

#include "automation_action.h"
#include <vclib/render/viewer/desktop_trackball.h>
#include <vclib/space/core/quaternion.h>

class RotationAutomationAction: public AutomationAction
{
    using Parent = AutomationAction;
    vcl::DesktopTrackBall<float> *trackball;
    vcl::Quaternion<float> rotationEachMS;

    public:

    RotationAutomationAction(const vcl::DesktopTrackBall<float> *trck, const vcl::Quaternion<float> &quat)
    {
        this->trackball = trck;
        this->rotationEachMS = quat;
    }

    void update() override
    {
        std::chrono::high_resolution_clock::time_point now = 
            std::chrono::high_resolution_clock::now();
        
        auto deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(now-prev);

        while(deltaT.count() > 1){
            deltaT -= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float, std::ratio<1,1000>>(1));
        }
    };
};

#endif