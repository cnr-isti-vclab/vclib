#ifndef BENCHMARK_DRAWER_H
#define BENCHMARK_DRAWER_H

#include "automation_action.h"
#include "fps_benchmark_metric.h"
#include "stdout_benchmark_printer.h"
#include <bgfx/bgfx.h>
#include <vector>
#include <vclib/space/core/vector/polymorphic_object_vector.h>
#include <chrono>
#include <iostream>
#include <vclib/misc/timer.h>
#include <string>
#include <fstream>

#define BENCHMARK_DRAWER_REPEAT_FOREVER 0

template<typename DerivedDrawer>
class BenchmarkDrawer : public vcl::PlainDrawer<DerivedDrawer>
{
    using Parent = vcl::PlainDrawer<DerivedDrawer>;

    //What is this for? To avoid the slight freeze some time after starting.
    //It's not about "frames" but about "seconds" (Lower framerate = less frames that you need to wait to avoid the freeze,
    //tested so far only on 2 devices)
    //What causes the freeze? no clue.
    bool beforeStartWaitTimerStarted = false;
    vcl::Timer beforeStartWaitTimer;
    double beforeStartWaitSeconds = 0.5f;

    vcl::PolymorphicObjectVector<AutomationAction> automations;
    std::vector<bool> relevancies;

    size_t currentAutomationIndex = 0;

    uint32_t repeatTimes = 1;
    uint32_t repeatCount = 0;

    std::shared_ptr<BenchmarkMetric> metric = FpsBenchmarkMetric().clone();
    std::shared_ptr<BenchmarkPrinter> printer = StdoutBenchmarkPrinter().clone();

    bool toStdOut = true;
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
        metric->end();
        if(relevancies[currentAutomationIndex]){
            printer->print(*metric);
        }
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

    void onDrawContent(vcl::uint viewId) override
    {
        if(!beforeStartWaitTimerStarted)
        {
            beforeStartWaitTimer.start();
            beforeStartWaitTimerStarted = true;
        }
        if(beforeStartWaitTimer.delay() < beforeStartWaitSeconds)
        {
            return;
        }
        if(automations.size() == 0){
            allDone = true;
        }
        if(allDone){
            return;
        }
        if(firstCall){
            automations[0]->start();
            metric->start();
            firstCall = false;
        }
        if(!automations[currentAutomationIndex]->isActive()){

            onAutomationEnd();
            allDone = currentAutomationIndex >= automations.size();
            if(allDone){
                repeatCount++;
                if(isLastLoop()){
                    printf("All benchmarks done.\n");
                    printer->finish(*metric);
                    return;
                }
                benchmarkLoop();
                printer->onBenchmarkLoop();
            }
            metric->start();
            automations[currentAutomationIndex]->start();
        }
        if(automations[currentAutomationIndex]->isActive()){
            metric->measure();
            automations[currentAutomationIndex]->doAction();
        }
    };

    size_t addAutomation(const AutomationAction &action, bool relevancy = true)
    {
        automations.pushBack(action);
        relevancies.push_back(relevancy);
        return automations.size()-1;
    }

    void restartBenchmark()
    {
        currentAutomationIndex = 0;
        firstCall = true;
        allDone = false;
        repeatCount = 0;
    };

    void useStdOut()
    {
        toStdOut = true;
    }

    void setMetric(const BenchmarkMetric &bm)
    {
        metric = bm.clone();
    };

    void setPrinter(const BenchmarkPrinter &bp)
    {
        printer = bp.clone();
    };

    void getRepeatCount()
    {
        return repeatCount;
    };

    void getCurrentAutomationIndex()
    {
        return currentAutomationIndex;
    };
};

#endif