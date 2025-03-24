#ifndef VCL_FRAME_DELAY_AUTOMATION_ACTION_H
#define VCL_FRAME_DELAY_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>

namespace vcl{

/*
    Automation that allows you to add a delay (in terms of frames) to an action,
    so that it starts only after the delay has elapsed
*/
class FrameDelayAutomationAction : public WrapperAutomationAction
{
    uint32_t currentFrames = 0;
    uint32_t delayFrames;
    bool innerStarted = false;
    using Parent = WrapperAutomationAction;

    public:

    FrameDelayAutomationAction(const AbstractAutomationAction &action, uint32_t delayFrames)
    : Parent(action),
    delayFrames{delayFrames}
    {};

    void start() override
    {
        AbstractAutomationAction::start();
    }

    void doAction() override
    {
        AbstractAutomationAction::doAction();
        if(currentFrames < delayFrames){
            currentFrames++;
            return;
        }
        if(!innerStarted){
            innerAction->start();
            innerStarted = true;
        }
        if(innerAction->isActive()){
            innerAction->doAction();
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

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<FrameDelayAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<FrameDelayAutomationAction>(std::move(*this));
    }
};

}

#endif