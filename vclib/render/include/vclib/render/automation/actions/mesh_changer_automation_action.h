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

#ifndef VCL_MESH_CHANGER_AUTOMATION_ACTION_H
#define VCL_MESH_CHANGER_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/drawers/abstract_viewer_drawer.h>

namespace vcl {

/*
    Automation that represents a singular change of mesh in a viewer drawer.
*/
template<typename DerivedRenderApp, typename MeshType>
class MeshChangerAutomationAction : public AbstractAutomationAction
{
    vcl::AbstractViewerDrawer<DerivedRenderApp>* avd;
    std::shared_ptr<vcl::DrawableObjectVector>   objectVector;

    using Parent = AbstractAutomationAction;

public:
    MeshChangerAutomationAction(
        vcl::AbstractViewerDrawer<DerivedRenderApp>* avd,
        vcl::DrawableMesh<MeshType>&                 mesh) :
            avd {avd},
            objectVector {std::make_shared<vcl::DrawableObjectVector>(
                vcl::DrawableObjectVector())}
    {
        objectVector->pushBack(mesh);
    }

    void doAction() override
    {
        Parent::doAction();
        avd->setDrawableObjectVector(objectVector);
        avd->fitScene();
        end();
    }

    void end() override { Parent::end(); }

    std::shared_ptr<AbstractAutomationAction> clone() const& override
    {
        return std::make_shared<MeshChangerAutomationAction>(*this);
    }

    std::shared_ptr<AbstractAutomationAction> clone() && override
    {
        return std::make_shared<MeshChangerAutomationAction>(std::move(*this));
    }
};

} // namespace vcl

#endif
