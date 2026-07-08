/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_BENCHMARK_EDITOR_H
#define VCL_BENCHMARK_EDITOR_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/automation/metrics/fps_benchmark_metric.h>
#include <vclib/render/automation/metrics/null_benchmark_metric.h>
#include <vclib/render/automation/printers/stdout_benchmark_printer.h>
#include <vclib/render/editors/editor.h>
#include <vclib/render/viewer/trackball.h>

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <iostream>

#define VCL_BENCHMARK_EDITOR_REPEAT_FOREVER 0

namespace vcl {

template<typename ViewerDrawer>
class BenchmarkEditor : public vcl::Editor<ViewerDrawer>
{
public:
    using ScalarType = typename ViewerDrawer::ScalarType;
    using PointType  = Point3<ScalarType>;
    using MatrixType = Matrix44<ScalarType>;

private:
    using AutomationType =
        AbstractAutomationAction<BenchmarkEditor<ViewerDrawer>>;
    using Parent = vcl::Editor<ViewerDrawer>;

    bool mTerminateUponCompletion = false;

    bool   mBeforeStartWaitTimerStarted = false;
    Timer  mBeforeStartWaitTimer;
    double mBeforeStartWaitSeconds = 0.6f;

    PolymorphicObjectVector<AutomationType>  mAutomations;
    PolymorphicObjectVector<BenchmarkMetric> mMetrics;

    size_t mCurrentAutomationIndex = 0;

    uint mRepeatTimes = 1;
    uint mRepeatCount = 0;

    std::shared_ptr<BenchmarkMetric>  mMetric = FpsBenchmarkMetric().clone();
    std::shared_ptr<BenchmarkPrinter> mPrinter =
        StdoutBenchmarkPrinter().clone();

    bool mFirstCall = true;
    bool mAllDone   = false;

    void benchmarkLoop()
    {
        mCurrentAutomationIndex = 0;
        mAllDone                = false;
    }

    bool isLastLoop()
    {
        return mRepeatTimes != VCL_BENCHMARK_EDITOR_REPEAT_FOREVER &&
               mRepeatCount >= mRepeatTimes;
    }

    void onAutomationEnd()
    {
        mMetrics[mCurrentAutomationIndex]->end();
        if (!mMetrics[mCurrentAutomationIndex]->isNull()) {
            mPrinter->print(
                *mMetrics[mCurrentAutomationIndex],
                mAutomations[mCurrentAutomationIndex]->getDescription());
        }
        mCurrentAutomationIndex++;
    }

public:
    BenchmarkEditor() = default;

    void draw(uint viewId) override
    {
        if (!mBeforeStartWaitTimerStarted) {
            mBeforeStartWaitTimer.start();
            mBeforeStartWaitTimerStarted = true;
        }
        if (mBeforeStartWaitTimer.delay() < mBeforeStartWaitSeconds) {
            Parent::viewerUpdate(); // Force update to loop
            return;
        }
        if (mAutomations.size() == 0) {
            mAllDone = true;
        }
        if (mAllDone) {
            if (mTerminateUponCompletion)
                exit(0);
            return;
        }
        if (mFirstCall) {
            Parent::viewerSetContinuousRedraw(true);
            mAutomations[0]->start();
            mMetrics[0]->start();
            mFirstCall = false;
        }
        if (!mAutomations[mCurrentAutomationIndex]->isActive()) {
            onAutomationEnd();
            mAllDone = mCurrentAutomationIndex >= mAutomations.size();
            if (mAllDone) {
                mRepeatCount++;
                if (isLastLoop()) {
                    Parent::viewerSetContinuousRedraw(false);
                    std::cerr << "All benchmarks done." << std::endl;
                    mPrinter->finish();
                    return;
                }
                benchmarkLoop();
                mPrinter->onBenchmarkLoop();
            }
            mMetrics[mCurrentAutomationIndex]->start();
            mAutomations[mCurrentAutomationIndex]->start();
        }
        if (!mAllDone && mAutomations[mCurrentAutomationIndex]->isActive()) {
            mMetrics[mCurrentAutomationIndex]->measure();
            mAutomations[mCurrentAutomationIndex]->doAction();
            Parent::viewerUpdate(); // Force redraw for benchmarking
        }
    }

    size_t addAutomation(const AutomationType& action)
    {
        std::shared_ptr<AutomationType> cloned = action.clone();
        cloned->setBenchmarkEditor(this);
        mAutomations.pushBack(std::move(*cloned));
        mMetrics.pushBack(*mMetric);
        return mAutomations.size() - 1;
    }

    size_t addAutomation(
        const AutomationType&  action,
        const BenchmarkMetric& metric)
    {
        std::shared_ptr<AutomationType> cloned = action.clone();
        cloned->setBenchmarkEditor(this);
        mAutomations.pushBack(std::move(*cloned));
        mMetrics.pushBack(metric);
        return mAutomations.size() - 1;
    }

    size_t addAutomationNoMetric(const AutomationType& action)
    {
        addAutomation(action, NullBenchmarkMetric());
        return mAutomations.size() - 1;
    }

    void setMetric(const BenchmarkMetric& bm) { mMetric = bm.clone(); }

    void setPrinter(const BenchmarkPrinter& bp) { mPrinter = bp.clone(); };

    void setRepeatTimes(uint repeatTimes) { mRepeatTimes = repeatTimes; }

    uint getRepeatCount() { return mRepeatCount; };

    size_t getCurrentAutomationIndex() { return mCurrentAutomationIndex; };

    void terminateUponCompletion(bool b = true)
    {
        mTerminateUponCompletion = b;
    }

    void rotate(const PointType& axis, float angle)
    {
        using TrackBallType = vcl::TrackBall<ScalarType>;
        using Args          = typename TrackBallType::TransformArgs;
        Parent::viewerTrackball().applyAtomicMotion(
            TrackBallType::ARC, Args(axis, angle));
    }

    void scale(float amount)
    {
        using TrackBallType = vcl::TrackBall<ScalarType>;
        Parent::viewerTrackball().applyAtomicMotion(
            TrackBallType::SCALE, amount);
    }
};

} // namespace vcl

#endif
