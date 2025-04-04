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

#ifndef VCL_METRIC_CHANGER_AUTOMATION_ACTION_H
#define VCL_METRIC_CHANGER_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/automation/metrics/benchmark_metric.h>

namespace vcl {

/**
 * The MetricChangerAutomationAction is an automation that represents a
 * change of metric inside a BenchmarkDrawer
 */
template<typename BmarkDrawer>
class MetricChangerAutomationAction :
        public AbstractAutomationAction<BmarkDrawer>
{
    using Parent = AbstractAutomationAction<BmarkDrawer>;

    BmarkDrawer*                     mBenchmarkDrawer;
    std::shared_ptr<BenchmarkMetric> mMetric;

public:
    MetricChangerAutomationAction(const BenchmarkMetric& metric) :
            mMetric {metric.clone()} {};

    void doAction() override
    {
        mBenchmarkDrawer->setMetric(*mMetric);
        end();
    };

    void end() override { Parent::end(); }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<MetricChangerAutomationAction<BmarkDrawer>>(
            *this);
    };

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<MetricChangerAutomationAction<BmarkDrawer>>(
            std::move(*this));
    };
};

} // namespace vcl

#endif
