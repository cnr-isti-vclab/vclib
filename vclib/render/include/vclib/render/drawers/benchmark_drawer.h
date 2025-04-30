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
#include <vclib/render/automation/metrics/null_benchmark_metric.h>
#include <vclib/render/automation/printers/stdout_benchmark_printer.h>
#include <vclib/render/drawers/event_drawer.h>
#include <vclib/render/viewer/camera.h>
#include <vclib/render/viewer/lights/directional_light.h>
#include <vclib/space/core/quaternion.h>
#include <vclib/space/core/vector/polymorphic_object_vector.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define VCL_BENCHMARK_DRAWER_REPEAT_FOREVER 0

namespace vcl {

/**
 * The BenchmarkDrawer is a class that combines a BenchmarkPrinter, a
 * BenchmarkMetric and a Vector of Automations to measure and write (somewhere)
 * the performance of each Automation.
 */
template<typename DerivedDrawer>
class BenchmarkDrawer : public vcl::EventDrawer<DerivedDrawer>
{
public:
    using ScalarType = float;
    using PointType  = Point3<ScalarType>;
    using MatrixType = Matrix44<ScalarType>;

private:
    using AutomationType =
        AbstractAutomationAction<BenchmarkDrawer<DerivedDrawer>>;
    using Parent = vcl::EventDrawer<DerivedDrawer>;

    DirectionalLight<ScalarType> mLight;
    Camera<ScalarType>           mCamera;
    Affine3<ScalarType>          mTransform = Affine3<ScalarType>::Identity();
    bool                         mTerminateUponCompletion = false;

    /*
    What are these variables for? To avoid the slight freeze some time after
    starting. It's not about "frames" but about "seconds" (Lower framerate =
    less frames that you need to wait to avoid the freeze, tested so far only on
    2 devices) What causes the freeze? no clue.
    */
    bool   mBeforeStartWaitTimerStarted = false;
    Timer  mBeforeStartWaitTimer;
    double mBeforeStartWaitSeconds = 0.4f;

    PolymorphicObjectVector<AutomationType>  mAutomations;
    PolymorphicObjectVector<BenchmarkMetric> mMetrics;

    size_t mCurrentAutomationIndex = 0;

    uint mRepeatTimes = 1;
    uint mRepeatCount = 0;

    std::shared_ptr<BenchmarkMetric>  mMetric = FpsBenchmarkMetric().clone();
    std::shared_ptr<BenchmarkPrinter> mPrinter =
        StdoutBenchmarkPrinter().clone();

    ScalarType mCurrentPreScale = 1.f;

    bool mFirstCall = true;
    bool mAllDone   = false;

    void benchmarkLoop()
    {
        mCurrentAutomationIndex = 0;
        mAllDone                = false;
    };

    bool isLastLoop()
    {
        return mRepeatTimes != VCL_BENCHMARK_DRAWER_REPEAT_FOREVER &&
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
    // using Parent::onDraw;
    using Parent::onInit;
    using Parent::onPostDraw;
    using Parent::Parent;

    BenchmarkDrawer(uint width, uint height) : Parent(width, height)
    {
        onResize(width, height);
    }

    MatrixType viewMatrix()
    {
        return mCamera.viewMatrix() * mTransform.matrix();
    }

    MatrixType projectionMatrix() { return mCamera.projectionMatrix(); }

    Camera<ScalarType> camera() { return mCamera; }

    DirectionalLight<ScalarType> light() { return mLight; }

    void onResize(uint width, uint height) override
    {
        Parent::onResize(width, height);
        mCamera.aspectRatio() = ScalarType(double(width) / height);
    }

    void reset() {}

    void focus(PointType p) {}

    void fitScene(PointType p, ScalarType s)
    {
        assert(s != 0);
        mCurrentPreScale = 1.5 / s;
        mTransform.scale(1.5 / s);
        mTransform.translate(-p);
    }

    /**
     * Set how many times the entire sequence of automations should be repeated
     *
     * WARNING: repeating forever does not work properly with printers. It is
     * recommended to only repeat forever with a NullBenchmarkPrinter
     */
    void setRepeatTimes(float repeatTimes) { this->mRepeatTimes = repeatTimes; }

    void onDrawContent(uint viewId) override
    {
        if (!mBeforeStartWaitTimerStarted) {
            mBeforeStartWaitTimer.start();
            mBeforeStartWaitTimerStarted = true;
        }
        if (mBeforeStartWaitTimer.delay() < mBeforeStartWaitSeconds) {
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
        if (mAutomations[mCurrentAutomationIndex]->isActive()) {
            mMetrics[mCurrentAutomationIndex]->measure();
            mAutomations[mCurrentAutomationIndex]->doAction();
        }
    };

    /**
     * Add an automation to the back of the list of automations. The metric
     * measured for this automation is the last one that was set through
     * setMetric(...)
     */
    size_t addAutomation(const AutomationType& action)
    {
        std::shared_ptr<AutomationType> cloned = action.clone();
        cloned->setBenchmarkDrawer(this);
        mAutomations.pushBack(std::move(*cloned));
        mMetrics.pushBack(*mMetric);
        return mAutomations.size() - 1;
    }

    /**
     * Add an automation and measure the chosen metric for it. The metric change
     * only applies to this automation.
     */
    size_t addAutomation(
        const AutomationType&  action,
        const BenchmarkMetric& metric)
    {
        std::shared_ptr<AutomationType> cloned = action.clone();
        cloned->setBenchmarkDrawer(this);
        mAutomations.pushBack(std::move(*cloned));
        mMetrics.pushBack(metric);
        return mAutomations.size() - 1;
    }

    /**
     * Syntactic sugar for adding an automation whose measured metric is
     * NullBenchmarkMetric
     */
    size_t addAutomationNoMetric(const AutomationType& action)
    {
        addAutomation(action, NullBenchmarkMetric());
        return mAutomations.size() - 1;
    }

    /**
     * Sets the BenchmarkMetric that will be used by all the Automations added
     * after this call. It has no effect on Automations added before this call.
     *
     * @param[in] bm The BenchmarkMetric to set
     */
    void setMetric(const BenchmarkMetric& bm) { mMetric = bm.clone(); }

    void setPrinter(const BenchmarkPrinter& bp) { mPrinter = bp.clone(); };

    uint getRepeatCount() { return mRepeatCount; };

    size_t getCurrentAutomationIndex() { return mCurrentAutomationIndex; };

    void terminateUponCompletion(bool b = true)
    {
        mTerminateUponCompletion = b;
    }

    void rotate(Quaternion<float> rot) { mTransform.prerotate(rot); }

    /**
     * Changes scale in an Additive way (given a current scaling factor of 10x
     * increasing by 1x brings the scale to 11x)
     *
     * @note Additive scaling is represented by the following formula:
     * finalScale = initialScale + (deltaScale * duration)
     */
    void changeScaleAdditive(ScalarType deltaS)
    {
        ScalarType scalingFactor =
            (mCurrentPreScale + deltaS) / mCurrentPreScale;
        mTransform.prescale(scalingFactor);
        mCurrentPreScale = mCurrentPreScale + deltaS;
    }

    /**
     * Changes scale in a Multiplicative way (given a current scaling factor of
     * 10x increasing by 1x brings the scale to 20x)
     *
     *  @note Multiplicative scaling is represented by the following formula:
     *  finalScale = initialScale * (1 + (deltaScale * duration))
     */
    void changeScaleMultiplicative(ScalarType deltaS)
    {
        ScalarType additiveIncrease = deltaS * mCurrentPreScale;
        changeScaleAdditive(additiveIncrease);
    }

    /**
     * Changes scale in a Multiplicative way using base as the scaling factor to
     * multiply off of instead of the current scaling factor (given a current
     * scaling factor of 10x and base 5x increasing by 1x brings the scale to
     * 15x)
     *
     *  @note Multiplicative scaling is represented by the following formula:
     *  finalScale = initialScale * (1 + (deltaScale * duration))
     */
    void changeScaleMultiplicative(ScalarType deltaS, ScalarType base)
    {
        ScalarType additiveIncrease = deltaS * base;
        changeScaleAdditive(additiveIncrease);
    }

    ScalarType getScale() { return mCurrentPreScale; }
};

} // namespace vcl

#endif
