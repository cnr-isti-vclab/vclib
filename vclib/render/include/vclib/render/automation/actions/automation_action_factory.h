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

// TODO (maybe): find a way to remove template on Automations and remove this
// class?
/**
 * The AutomationActionFactory class is a factory class that allows the creation
 * of Automations without having to specify the template each time and with
 * shorter names.
 *
 */
template<typename DerivedRenderApp>
class AutomationActionFactory
{
    using TemplPar = DerivedRenderApp;

public:
    PerFrameRotationAutomationAction<TemplPar> createPerFrameRotation(
        float   framesPerRotation,
        Point3f axis)
    {
        return PerFrameRotationAutomationAction<TemplPar>(
            framesPerRotation, axis);
    }

    PerFrameChangeScaleAdditiveAutomationAction<TemplPar>
    createPerFrameChangeScaleAdditive(float pixelDeltaPerFrame)
    {
        return PerFrameChangeScaleAdditiveAutomationAction<TemplPar>(
            pixelDeltaPerFrame);
    }

    PerFrameChangeScaleMultiplicativeAutomationAction<TemplPar>
    createPerFrameChangeScaleMultiplicative(float pixelDeltaPerFrame)
    {
        return PerFrameChangeScaleMultiplicativeAutomationAction<TemplPar>(
            pixelDeltaPerFrame);
    }

    RotationAutomationAction<TemplPar> createRotation(
        float   framesPerRotation,
        Point3f axis)
    {
        return RotationAutomationAction<TemplPar>(framesPerRotation, axis);
    }

    ChangeScaleAdditiveAutomationAction<TemplPar> createChangeScaleAdditive(
        float pixelDeltaPerSecond)
    {
        return ChangeScaleAdditiveAutomationAction<TemplPar>(
            pixelDeltaPerSecond);
    }

    ChangeScaleMultiplicativeAutomationAction<TemplPar>
    createChangeScaleMultiplicative(float pixelDeltaPerSecond)
    {
        return ChangeScaleMultiplicativeAutomationAction<TemplPar>(
            pixelDeltaPerSecond);
    }

    SequentialAutomationActions<TemplPar> createSequential(
        std::initializer_list<
            std::shared_ptr<AbstractAutomationAction<TemplPar>>> init)
    {
        return SequentialAutomationActions(init);
    }

    SimultaneousAutomationActions<TemplPar> createSimultaneous(
        std::initializer_list<
            std::shared_ptr<AbstractAutomationAction<TemplPar>>> init)
    {
        return SimultaneousAutomationActions<TemplPar>(init);
    }

    FrameDelayAutomationAction<TemplPar> createFrameDelay(
        const AbstractAutomationAction<TemplPar>& action,
        uint32_t                                  delayFrames)
    {
        return FrameDelayAutomationAction<TemplPar>(action, delayFrames);
    }

    FrameLimitedAutomationAction<TemplPar> createFrameLimited(
        const AbstractAutomationAction<TemplPar>& innerAction,
        uint32_t                                  durationFrames)
    {
        return FrameLimitedAutomationAction<TemplPar>(
            innerAction, durationFrames);
    }

    StartCountDelayAutomationAction<TemplPar> createStartCountDelay(
        const AbstractAutomationAction<TemplPar>& innerAction,
        uint32_t                                  waitStarts)
    {
        return StartCountDelayAutomationAction<TemplPar>(
            innerAction, waitStarts);
    }

    StartCountLimitedAutomationAction<TemplPar> createStartCountLimited(
        const AbstractAutomationAction<TemplPar>& innerAction,
        uint32_t                                  maximumStarts)
    {
        return StartCountLimitedAutomationAction<TemplPar>(
            innerAction, maximumStarts);
    }

    TimeDelayAutomationAction<TemplPar> createTimeDelay(
        const AbstractAutomationAction<TemplPar>& action,
        float                                     delaySeconds)
    {
        return TimeDelayAutomationAction<TemplPar>(action, delaySeconds);
    }

    TimeLimitedAutomationAction<TemplPar> createTimeLimited(
        const AbstractAutomationAction<TemplPar>& action,
        float                                     durationSeconds)
    {
        return TimeLimitedAutomationAction<TemplPar>(action, durationSeconds);
    }
};

} // namespace vcl

#endif
