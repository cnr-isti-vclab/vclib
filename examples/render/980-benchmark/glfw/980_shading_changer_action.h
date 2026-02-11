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

#include <vclib/space/core/quaternion.h>

#include <iomanip>
#include <numbers>
#include <sstream>

namespace vcl {

/**
 * The PerFrameRotationAutomationAction is an automation that represents a
 * rotation, with the strength of the rotation measured
 * per-frame
 */
template<typename BmarkDrawer>
class ShadingChangerAutomationAction :
        public AbstractAutomationAction<BmarkDrawer>
{
    using Parent = AbstractAutomationAction<BmarkDrawer>;
    using Parent::benchmarkDrawer;
    std::shared_ptr<DrawableObjectVector> mVec;
    MeshRenderInfo::Surface               mShad;

public:
    ShadingChangerAutomationAction(MeshRenderInfo::Surface shad): mShad(shad) {}

    std::string getDescription() override
    {
        using MRIS = MeshRenderInfo::Surface;
        std::string str;
        switch (mShad) {
            case MRIS::COLOR_FACE:
                str = "COLOR_FACE";
                break;
            case MRIS::COLOR_MESH:
                str = "COLOR_MESH";
                break;
            case MRIS::COLOR_USER:
                str = "COLOR_USER";
                break;
            case MRIS::COLOR_VERTEX:
                str = "COLOR_VERTEX";
                break;
            case MRIS::COLOR_VERTEX_TEX:
                str = "COLOR_VERTEX_TEX";
                break;
            case MRIS::COLOR_WEDGE_TEX:
                str = "COLOR_WEDGE_TEX";
                break;
            case MRIS::SHADING_FLAT:
                str = "SHADING_FLAT";
                break;
            case MRIS::SHADING_NONE:
                str = "SHADING_NONE";
                break;
            case MRIS::SHADING_SMOOTH:
                str = "SHADING_SMOOTH";
                break;
            case MRIS::VISIBLE:
                str = "VISIBLE";
                break;
            default:
                str = "NONESENSE";
                break;
        }
        std::ostringstream temp;
        temp << "Change shading to" << str;
        return temp.str();
    }

    void doAction() override
    {
        Parent::doAction();
        for (size_t i = 0; i < mVec->size(); i++) {
            auto el = mVec->at(i);
            if (auto p = dynamic_cast<AbstractDrawableMesh*>(el.get())) {
                MeshRenderSettings mrs(p->renderSettings());
                mrs.setSurface(mShad);
                p->setRenderSettings(mrs);
            }
        }
        Parent::end();
    };

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<ShadingChangerAutomationAction<BmarkDrawer>>(
            *this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<ShadingChangerAutomationAction<BmarkDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
