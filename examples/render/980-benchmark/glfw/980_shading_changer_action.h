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

#ifndef VCL_980_SHADING_CHANGER_AUTOMATION_ACTION_H
#define VCL_980_SHADING_CHANGER_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/automation/metrics/null_benchmark_metric.h>
#include <vclib/render/drawers/benchmark_viewer_drawer.h>

#include <vclib/space/core/quaternion.h>

#include <iomanip>
#include <numbers>
#include <sstream>

namespace vcl {

class ShadingChangerMetric: public NullBenchmarkMetric {
    MeshRenderInfo::Surface mShad;
    mutable uint mPrintWaitCount;

    public:
    ShadingChangerMetric(MeshRenderInfo::Surface shad, uint printWaitCount) : mShad(shad), mPrintWaitCount(printWaitCount) {};

    std::string getUnitOfMeasure() const override {
        if (mPrintWaitCount >= 0) {
            mPrintWaitCount --;
        }
        if (mPrintWaitCount != -1) {
            return "__skip";
        }
        using MRIS = MeshRenderInfo::Surface;
        std::string str;
        switch (mShad) {
            case MRIS::COLOR_FACE:
                str = "col face";
                break;
            case MRIS::COLOR_MESH:
                str = "col mesh";
                break;
            case MRIS::COLOR_USER:
                str = "col user";
                break;
            case MRIS::COLOR_VERTEX:
                str = "col vert";
                break;
            case MRIS::COLOR_VERTEX_TEX:
                str = "tex vert";
                break;
            case MRIS::COLOR_WEDGE_TEX:
                str = "tex wedge";
                break;
            case MRIS::SHADING_FLAT:
                str = "flat";
                break;
            case MRIS::SHADING_NONE:
                str = "none";
                break;
            case MRIS::SHADING_SMOOTH:
                str = "smooth";
                break;
            case MRIS::VISIBLE:
                str = "visible";
                break;
            default:
                str = "unknown";
                break;
        }
        return str;
    }

    bool isNull() const override { return false; }

    std::shared_ptr<BenchmarkMetric> clone() const& override
    {
        return std::make_shared<ShadingChangerMetric>(*this);
    };

    std::shared_ptr<BenchmarkMetric> clone() && override
    {
        return std::make_shared<ShadingChangerMetric>(std::move(*this));
    };
};

/**
 * The PerFrameRotationAutomationAction is an automation that represents a
 * rotation, with the strength of the rotation measured
 * per-frame
 */
template<typename BmarkDrawer, typename BmarkViewerDrawer>
class ShadingChangerAutomationAction :
        public AbstractAutomationAction<BmarkDrawer>
{
    using Parent = AbstractAutomationAction<BmarkDrawer>;
    using Parent::benchmarkDrawer;
    std::shared_ptr<DrawableObjectVector> mVec;
    MeshRenderInfo::Surface               mShad;
    BmarkViewerDrawer*                    mViewerDrawer;

public:
    ShadingChangerAutomationAction(MeshRenderInfo::Surface shad, BmarkViewerDrawer* viewerDrawer): mShad(shad), mViewerDrawer(viewerDrawer) {}

    

    std::string getDescription() override
    {
        using MRIS = MeshRenderInfo::Surface;
        std::string str;
        switch (mShad) {
            case MRIS::COLOR_FACE:
                str = "col face";
                break;
            case MRIS::COLOR_MESH:
                str = "col mesh";
                break;
            case MRIS::COLOR_USER:
                str = "col user";
                break;
            case MRIS::COLOR_VERTEX:
                str = "col vert";
                break;
            case MRIS::COLOR_VERTEX_TEX:
                str = "tex vert";
                break;
            case MRIS::COLOR_WEDGE_TEX:
                str = "tex wedge";
                break;
            case MRIS::SHADING_FLAT:
                str = "flat";
                break;
            case MRIS::SHADING_NONE:
                str = "none";
                break;
            case MRIS::SHADING_SMOOTH:
                str = "smooth";
                break;
            case MRIS::VISIBLE:
                str = "visible";
                break;
            default:
                str = "unknown";
                break;
        }
        std::ostringstream temp;
        temp << "Change shading to" << str;
        return temp.str();
    }

    void doAction() override
    {
        Parent::doAction();
        mViewerDrawer->setShading(mShad);
        Parent::end();
    };

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<ShadingChangerAutomationAction<BmarkDrawer, BmarkViewerDrawer>>(
            *this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<ShadingChangerAutomationAction<BmarkDrawer, BmarkViewerDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
