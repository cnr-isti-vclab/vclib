#ifndef FRAME_LIMITED_AUTOMATION_ACTION_H
#define FRAME_LIMITED_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/wrappers/wrapper_automation_action.h>

namespace vcl{

class FrameLimitedAutomationAction : public WrapperAutomationAction
{
    using Parent = WrapperAutomationAction;

    uint32_t currentFrames = 0;
    uint32_t durationFrames;

    public:

    FrameLimitedAutomationAction(const AbstractAutomationAction &innerAction, uint32_t durationFrames = 400.f)
    : Parent(innerAction),
    durationFrames{durationFrames},
    currentFrames{0}
    {};

    void start()
    {
        Parent::start();
    }

    void doAction()
    {
        currentFrames++;
        if(currentFrames >= durationFrames){
            end();
            return;
        }
        Parent::doAction();
    }

    void end() override
    {
        Parent::end();
        currentFrames = 0;
    }

    std::shared_ptr<AbstractAutomationAction> clone() const & override
    {
        return std::make_shared<FrameLimitedAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<FrameLimitedAutomationAction>(std::move(*this));
    }
};

}

#endif