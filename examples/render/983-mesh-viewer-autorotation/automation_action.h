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

        virtual void update() = 0;

        void end()
        {
            active = false;
        };

        virtual bool isActive()
        {
            return active;
        }
};

#endif