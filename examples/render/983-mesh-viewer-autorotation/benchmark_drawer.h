#ifndef BENCHMARK_DRAWER_H
#define BENCHMARK_DRAWER_H

#include "automation_action.h"
#include <bgfx/bgfx.h>
#include <vector>
#include <chrono>
#include <iostream>
#include <vclib/misc/timer.h>

#define BENCHMARK_DRAWER_REPEAT_FOREVER 0

template<typename DerivedDrawer>
class BenchmarkDrawer : public vcl::PlainDrawer<DerivedDrawer>
{
    using Parent = vcl::PlainDrawer<DerivedDrawer>;
    std::vector<AutomationAction*> automations;
    size_t currentAutomationIndex = 0;
    uint32_t repeatTimes = 1;
    uint32_t repeatCount = 0;
    vcl::Timer currentAutomationTimer;
    uint32_t currentAutomationFrames = 0;
    bool firstCall = true;
    bool allDone = false;

    void benchmarkLoop()
    {
        currentAutomationIndex = 0;
        allDone = false;
    };

    bool isLastLoop()
    {
        return repeatTimes != BENCHMARK_DRAWER_REPEAT_FOREVER && repeatCount >= repeatTimes; 
    }

    void onAutomationEnd()
    {
        currentAutomationTimer.stop();
        double currentAutomationSeconds = currentAutomationTimer.delay();
        printf("Loop %u, automation %zu : %.4f fps average\n", repeatCount, currentAutomationIndex, (double)currentAutomationFrames / currentAutomationSeconds);
        currentAutomationFrames = 0;
        currentAutomationIndex++;
    }

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
            currentAutomationTimer.start();
        }
        if(!automations[currentAutomationIndex]->isActive()){

            onAutomationEnd();

            if(allDone = (currentAutomationIndex >= automations.size())){
                repeatCount++;
                if(isLastLoop()){
                    printf("All benchmarks done.\n");
                    return;
                }
                benchmarkLoop();
            }
            currentAutomationTimer.start();
            automations[currentAutomationIndex]->start();
        }
        currentAutomationFrames++;
        if(automations[currentAutomationIndex]->isActive()){
            automations[currentAutomationIndex]->update();
        }
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
};

#endif