#ifndef VCL_AUTOMATION_ACTION_H
#define VCL_AUTOMATION_ACTION_H

#include<chrono>

namespace vcl{

class AbstractAutomationAction
{
    private:
        bool active = false;

    public:

        //An action SHOULD NOT call start on itself during an update
        //A call to start SHOULD guarantee that the action is considered active (at least until the next update)
        virtual void start()
        {
            active = true;
        };


        //Outside callers SHOULD ALWAYS check if the action is active before calling
        //since the action MAY call end() on itself during an update
        virtual void doAction()
        {
            if(!isActive()){
                throw "Do not call doAction() on inactive actions!";
            }
        };

        //this method ALSO has to reset everything in THIS object so that a subsequent start() call
        //is like calling start() on a new object equal to this one (unless it makes no sense for the action to be started multiple times)
        virtual void end()
        {
            active = false;
        };

        virtual bool isActive()
        {
            return active;
        }

        operator std::shared_ptr<AbstractAutomationAction>() { return clone(); }; 

        virtual std::shared_ptr<AbstractAutomationAction> clone() const& = 0;
        virtual std::shared_ptr<AbstractAutomationAction> clone() && = 0;
};

}

#endif