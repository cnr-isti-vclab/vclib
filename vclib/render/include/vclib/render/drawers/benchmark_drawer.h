/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_BENCHMARK_DRAWER_H
#define VCL_BENCHMARK_DRAWER_H

#include <vclib/misc/timer.h>
#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/automation/metrics/fps_benchmark_metric.h>
#include <vclib/render/automation/printers/stdout_benchmark_printer.h>
#include <vclib/render/drawers/plain_drawer.h>
#include <vclib/space/core/vector/polymorphic_object_vector.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define VCL_BENCHMARK_DRAWER_REPEAT_FOREVER 0

namespace vcl {

/*
    Class that combines a Printer, a Metric and a Vector of Automations to
    measure and write (somewhere) the performance of each Automation.
    While it is a Drawer, it doesn't really draw anything.
*/
template<typename DerivedDrawer>
class BenchmarkDrawer : public vcl::PlainDrawer<DerivedDrawer>
{
    using Parent = vcl::PlainDrawer<DerivedDrawer>;

    /*
    What are these variables for? To avoid the slight freeze some time after
    starting. It's not about "frames" but about "seconds" (Lower framerate =
    less frames that you need to wait to avoid the freeze, tested so far only on
    2 devices) What causes the freeze? no clue.
    */
    bool       beforeStartWaitTimerStarted = false;
    vcl::Timer beforeStartWaitTimer;
    double     beforeStartWaitSeconds = 0.4f;

    vcl::PolymorphicObjectVector<AbstractAutomationAction> automations;
    /*
    This vector's purpose is to keep track of whether to write the metric
    measured for the automation at the same index in the automations vector
    */
    std::vector<bool> relevancies;

    size_t currentAutomationIndex = 0;

    uint32_t repeatTimes = 1;
    uint32_t repeatCount = 0;

    std::shared_ptr<BenchmarkMetric>  metric = FpsBenchmarkMetric().clone();
    std::shared_ptr<BenchmarkPrinter> printer =
        StdoutBenchmarkPrinter().clone();

    bool firstCall = true;
    bool allDone   = false;

    void benchmarkLoop()
    {
        currentAutomationIndex = 0;
        allDone                = false;
    };

    bool isLastLoop()
    {
        return repeatTimes != VCL_BENCHMARK_DRAWER_REPEAT_FOREVER &&
               repeatCount >= repeatTimes;
    }

    void onAutomationEnd()
    {
        metric->end();
        if (relevancies[currentAutomationIndex]) {
            printer->print(*metric);
        }
        currentAutomationIndex++;
    }

public:
    using Parent::onDraw;
    using Parent::onInit;
    using Parent::onPostDraw;
    using Parent::onResize;
    using Parent::Parent;

    /*WARNING: repeating forever does not work properly with printers. It is
    recommended to only repeat forever with a NullBenchmarkPrinter*/
    void setRepeatTimes(float repeatTimes) { this->repeatTimes = repeatTimes; }

    void onDrawContent(vcl::uint viewId) override
    {
        if (!beforeStartWaitTimerStarted) {
            beforeStartWaitTimer.start();
            beforeStartWaitTimerStarted = true;
        }
        if (beforeStartWaitTimer.delay() < beforeStartWaitSeconds) {
            return;
        }
        if (automations.size() == 0) {
            allDone = true;
        }
        if (allDone) {
            return;
        }
        if (firstCall) {
            automations[0]->start();
            metric->start();
            firstCall = false;
        }
        if (!automations[currentAutomationIndex]->isActive()) {
            onAutomationEnd();
            allDone = currentAutomationIndex >= automations.size();
            if (allDone) {
                repeatCount++;
                if (isLastLoop()) {
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
        if (automations[currentAutomationIndex]->isActive()) {
            metric->measure();
            automations[currentAutomationIndex]->doAction();
        }
    };

    size_t addAutomation(
        const AbstractAutomationAction& action,
        bool                            relevancy = true)
    {
        automations.pushBack(action);
        relevancies.push_back(relevancy);
        return automations.size() - 1;
    }

    void restartBenchmark()
    {
        currentAutomationIndex = 0;
        firstCall              = true;
        allDone                = false;
        repeatCount            = 0;
    };

    void setMetric(const BenchmarkMetric& bm) { metric = bm.clone(); };

    void setPrinter(const BenchmarkPrinter& bp) { printer = bp.clone(); };

    uint32_t getRepeatCount() { return repeatCount; };

    size_t getCurrentAutomationIndex() { return currentAutomationIndex; };
};

} // namespace vcl

#endif
