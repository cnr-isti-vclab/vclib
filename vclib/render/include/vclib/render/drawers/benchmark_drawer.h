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

    /*
    What are these variables for? To avoid the slight freeze some time after
    starting. It's not about "frames" but about "seconds" (Lower framerate =
    less frames that you need to wait to avoid the freeze, tested so far only on
    2 devices) What causes the freeze? no clue.
    */
    bool       mBeforeStartWaitTimerStarted = false;
    vcl::Timer mBeforeStartWaitTimer;
    double     mBeforeStartWaitSeconds = 0.4f;

    vcl::PolymorphicObjectVector<AutomationType> mAutomations;

    /**
     * This vector's purpose is to keep track of whether to write the metric
     * measured for the automation at the same index in the automations vector
     */
    std::vector<bool> mRelevancies;

    size_t mCurrentAutomationIndex = 0;

    uint32_t mRepeatTimes = 1;
    uint32_t mRepeatCount = 0;

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
        mMetric->end();
        if (mRelevancies[mCurrentAutomationIndex]) {
            mPrinter->print(*mMetric);
        }
        mCurrentAutomationIndex++;
    }

public:
    // using Parent::onDraw;
    using Parent::onInit;
    using Parent::onPostDraw;
    using Parent::onResize;
    using Parent::Parent;

    MatrixType viewMatrix()
    {
        return mCamera.viewMatrix() * mTransform.matrix();
    }

    MatrixType projectionMatrix() { return mCamera.projectionMatrix(); }

    Camera<ScalarType> camera() { return mCamera; }

    DirectionalLight<ScalarType> light() { return mLight; }

    void onDraw(uint viewId) override { Parent::onDraw(viewId); }

    void reset() { mCamera.center() = {0.f, 0.f, 0.f}; }

    void focus(PointType p) { mCamera.center() = p; }

    void fitScene(PointType p, ScalarType s)
    {
        mCurrentPreScale = s;
        mTransform.scale(s);
        mTransform.translate(-p);
    }

    /**
     * Set how many times the entire sequence of automations should be repeated
     *
     * WARNING: repeating forever does not work properly with printers. It is
     * recommended to only repeat forever with a NullBenchmarkPrinter
     */
    void setRepeatTimes(float repeatTimes) { this->mRepeatTimes = repeatTimes; }

    void onDrawContent(vcl::uint viewId) override
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
            return;
        }
        if (mFirstCall) {
            mAutomations[0]->start();
            mMetric->start();
            mFirstCall = false;
        }
        if (!mAutomations[mCurrentAutomationIndex]->isActive()) {
            onAutomationEnd();
            mAllDone = mCurrentAutomationIndex >= mAutomations.size();
            if (mAllDone) {
                mRepeatCount++;
                if (isLastLoop()) {
                    std::cerr << "All benchmarks done." << std::endl;
                    mPrinter->finish(*mMetric);
                    return;
                }
                benchmarkLoop();
                mPrinter->onBenchmarkLoop();
            }
            mMetric->start();
            mAutomations[mCurrentAutomationIndex]->start();
        }
        if (mAutomations[mCurrentAutomationIndex]->isActive()) {
            mMetric->measure();
            mAutomations[mCurrentAutomationIndex]->doAction();
        }
    };

    size_t addAutomation(const AutomationType& action, bool relevancy = true)
    {
        std::shared_ptr<AutomationType> cloned = action.clone();
        cloned->setBenchmarkDrawer(this);
        mAutomations.pushBack(std::move(*cloned));
        mRelevancies.push_back(relevancy);
        return mAutomations.size() - 1;
    }

    void setMetric(const BenchmarkMetric& bm) { mMetric = bm.clone(); };

    void setPrinter(const BenchmarkPrinter& bp) { mPrinter = bp.clone(); };

    uint32_t getRepeatCount() { return mRepeatCount; };

    size_t getCurrentAutomationIndex() { return mCurrentAutomationIndex; };

    void rotate(Quaternion<float> rot) { mTransform.prerotate(rot); }

    // Possibly rename to changeScaleAbsolute? Possibly create actions that
    // scale in different ways (and also rename the action)?
    /**
     * Changes scale by an absolute amount (i.e. if the current scale is 2.0 and
     * deltaS is 0.4, the new scale will be 2.4)
     */
    void scale(float deltaS)
    {
        ScalarType scalingFactor =
            (mCurrentPreScale + deltaS) / mCurrentPreScale;
        mTransform.prescale(scalingFactor);
        mCurrentPreScale = mCurrentPreScale * scalingFactor;
    }
};

} // namespace vcl

#endif
