#ifndef BENCHMARK_DRAWER_H
#define BENCHMARK_DRAWER_H

#include "automation_action.h"
#include <vector>

#define BENCHMARK_DRAWER_REPEAT_FOREVER 0

template<typename DerivedDrawer>
class BenchmarkDrawer : public vcl::PlainDrawer<DerivedDrawer>
{
    using Parent = vcl::PlainDrawer<DerivedDrawer>;
    std::vector<AutomationAction*> automations;
    size_t currentAutomationIndex = 0;
    uint32_t repeatTimes = 1;
    uint32_t repeatCount = 0;
    bool firstCall = true;
    bool allDone = false;

public:

    using Parent::Parent;
    using Parent::onDraw;
    using Parent::onInit;
    using Parent::onResize;
    using Parent::onPostDraw;

    void setRepeatTimes(float repeatTimes)
    {
        this->repeatTimes = repeatTimes;
    }

    void onDrawContent(uint viewId) override
    {
        if(automations.size() == 0){
            allDone = true;
        }
        if(allDone){
            return;
        }
        if(firstCall){
            automations[0]->start();
            firstCall = false;
        }
        if(!automations[currentAutomationIndex]->isActive()){
            currentAutomationIndex++;
            if(allDone = (currentAutomationIndex >= automations.size())){
                repeatCount++;
                if(isLastLoop()){
                    return;
                }
                benchmarkLoop();
            }
            automations[currentAutomationIndex]->start();
        }
        automations[currentAutomationIndex]->update();
    };

    size_t addAutomation(AutomationAction *action)
    {
        automations.push_back(action);
        return automations.size()-1;
    }

    void restartBenchmark()
    {
        currentAutomationIndex = 0;
        firstCall = true;
        allDone = false;
        repeatCount = 0;
    };

    void benchmarkLoop()
    {
        currentAutomationIndex = 0;
        allDone = false;
    };

    bool isLastLoop()
    {
        return repeatTimes != BENCHMARK_DRAWER_REPEAT_FOREVER && repeatCount >= repeatTimes; 
    }
};

#endif