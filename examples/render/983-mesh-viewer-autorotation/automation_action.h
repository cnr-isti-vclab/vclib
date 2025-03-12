#ifndef AUTOMATION_ACTION_H
#define AUTOMATION_ACTION_H

#include<chrono>

class AutomationAction
{
    protected:
        std::chrono::high_resolution_clock::time_point prev;
        bool active = false;

    public:

        //An action SHOULD NOT call start on itself during an update
        //A call to start does NOT guarantee that the action is active, as subclasses may set it to false based on their own state
        virtual void start()
        {
            prev = std::chrono::high_resolution_clock::now();
            active = true;
        };


        //Outside callers SHOULD ALWAYS check if the action is active before calling
        virtual void update() {};

        //this method ALSO has to reset everything in THIS object so that a subsequent start() call
        //is like calling start() on a new object equal to this one (unless it makes no sense for the action to be started multiple times)
        //An action MAY call end on itself during an update and therefore outside callers should always check if the action is active before calling
        virtual void end()
        {
            active = false;
        };

        virtual bool isActive()
        {
            return active;
        }
};

#endif