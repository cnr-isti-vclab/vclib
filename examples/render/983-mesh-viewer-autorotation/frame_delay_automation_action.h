#ifndef FRAME_DELAY_AUTOMATION_ACTION_H
#define FRAME_DELAY_AUTOMATION_ACTION_H

#include "wrapper_automation_action.h"

class FrameDelayAutomationAction : public WrapperAutomationAction
{
    uint32_t currentFrames = 0;
    uint32_t delayFrames;
    bool innerStarted = false;
    using Parent = WrapperAutomationAction;

    public:

    FrameDelayAutomationAction(const AutomationAction &action, uint32_t delayFrames)
    : Parent(action),
    delayFrames{delayFrames}
    {};

    void start() override
    {
        AutomationAction::start();
    }

    void update() override
    {
        if(currentFrames < delayFrames){
            currentFrames++;
            return;
        }
        if(!innerStarted){
            innerAction->start();
            innerStarted = true;
        }
        if(innerAction->isActive()){
            innerAction->update();
            return;
        }
        end();
    }

    void end() override
    {
        Parent::end();
        currentFrames = 0;
        innerStarted = false;
    }

    std::shared_ptr<AutomationAction> clone() const & override
    {
        return std::make_shared<FrameDelayAutomationAction>(*this);
    }

    std::shared_ptr<AutomationAction> clone() && override
    {
        return std::make_shared<FrameDelayAutomationAction>(std::move(*this));
    }
};

#endif