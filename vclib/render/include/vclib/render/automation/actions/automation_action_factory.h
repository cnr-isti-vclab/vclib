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

#ifndef VCL_AUTOMATION_ACTION_FACTORY_H
#define VCL_AUTOMATION_ACTION_FACTORY_H

#include <vclib/render/automation/actions.h>
#include <vclib/render/automation/metrics/benchmark_metric.h>
#include <vclib/render/drawers/benchmark_drawer.h>

namespace vcl {

/**
 * The AutomationActionFactory class is a factory class that allows the creation
 * of Automations without having to specify the template each time and with
 * shorter names
 */
template<typename DerivedRenderApp>
class AutomationActionFactory
{
    using TemplPar = BenchmarkDrawer<DerivedRenderApp>;

public:
    MetricChangerAutomationAction<TemplPar> MetricChanger(
        const BenchmarkMetric& metric)
    {
        return MetricChangerAutomationAction<TemplPar>(metric);
    }

    PerFrameRotationAutomationAction<TemplPar> PerFrameRotation(
        float   framesPerRotation,
        Point3f axis)
    {
        return PerFrameRotationAutomationAction<TemplPar>(
            framesPerRotation, axis);
    }

    PerFrameScaleAutomationAction<TemplPar> PerFrameScale(
        float pixelDeltaPerFrame)
    {
        return PerFrameScaleAutomationAction<TemplPar>(pixelDeltaPerFrame);
    }

    RotationAutomationAction<TemplPar> Rotation(
        float   framesPerRotation,
        Point3f axis)
    {
        return RotationAutomationAction<TemplPar>(framesPerRotation, axis);
    }

    ScaleAutomationAction<TemplPar> Scale(float pixelDeltaPerSecond)
    {
        return ScaleAutomationAction<TemplPar>(pixelDeltaPerSecond);
    }

    SequentialAutomationActions<TemplPar> Sequential(
        std::initializer_list<
            std::shared_ptr<AbstractAutomationAction<TemplPar>>> init)
    {
        return SequentialAutomationActions(init);
    }

    SimultaneousAutomationActions<TemplPar> Simultaneous(
        std::initializer_list<
            std::shared_ptr<AbstractAutomationAction<TemplPar>>> init)
    {
        return SimultaneousAutomationActions<TemplPar>(init);
    }

    FrameDelayAutomationAction<TemplPar> FrameDelay(
        const AbstractAutomationAction<TemplPar>& action,
        uint32_t                                  delayFrames)
    {
        return FrameDelayAutomationAction<TemplPar>(action, delayFrames);
    }

    FrameLimitedAutomationAction<TemplPar> FrameLimited(
        const AbstractAutomationAction<TemplPar>& innerAction,
        uint32_t                                  durationFrames = 400.f)
    {
        return FrameLimitedAutomationAction<TemplPar>(
            innerAction, durationFrames);
    }

    StartCountDelayAutomationAction<TemplPar> StartCountDelay(
        const AbstractAutomationAction<TemplPar>& innerAction,
        uint32_t                                  waitStarts)
    {
        return StartCountDelayAutomationAction<TemplPar>(
            innerAction, waitStarts);
    }

    StartCountLimitedAutomationAction<TemplPar> StartCountLimited(
        const AbstractAutomationAction<TemplPar>& innerAction,
        uint32_t                                  maximumStarts)
    {
        return StartCountLimitedAutomationAction<TemplPar>(
            innerAction, maximumStarts);
    }

    TimeDelayAutomationAction<TemplPar> TimeDelay(
        const AbstractAutomationAction<TemplPar>& action,
        float                                     delaySeconds)
    {
        return TimeDelayAutomationAction<TemplPar>(action, delaySeconds);
    }

    TimeLimitedAutomationAction<TemplPar> TimeLimited(
        const AbstractAutomationAction<TemplPar>& action,
        float                                     durationSeconds = 5.5f)
    {
        return TimeLimitedAutomationAction<TemplPar>(action, durationSeconds);
    }
};

} // namespace vcl

#endif
