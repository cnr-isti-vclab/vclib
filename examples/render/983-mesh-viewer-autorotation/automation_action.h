#ifndef AUTOMATION_ACTION_H
#define AUTOMATION_ACTION_H

#include<chrono>

class AutomationAction
{
    protected:
        std::chrono::high_resolution_clock::time_point prev;
        bool active = false;

    public:
        virtual void start()
        {
            prev = std::chrono::high_resolution_clock::now();
            active = true;
        };

        virtual void update() {};

        //this method ALSO has to reset everything in THIS object so that a subsequent start() call
        //is like calling start() on a new object equal to this one
        //or maybe just declare a reset() method that does so...TODO?
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