#ifndef MESH_CAHNGER_AUTOMATION_ACTION_H
#define MESH_CHANGER_AUTOMATION_ACTION_H

#include "automation_action.h"
#include <vclib/render/drawers/abstract_viewer_drawer.h>
#include <vclib/render/drawable/drawable_mesh.h>

template<typename DerivedRenderApp, typename MeshType>
class MeshChangerAutomationAction : public AutomationAction
{
    vcl::AbstractViewerDrawer<DerivedRenderApp>* avd;
    vcl::DrawableMesh<MeshType> *mesh;
    bool completedOnce =  false;

    using Parent = AutomationAction;

    public:

    MeshChangerAutomationAction(vcl::AbstractViewerDrawer<DerivedRenderApp>* avd, vcl::DrawableMesh<MeshType> *mesh)
    : avd{avd},
    mesh{mesh}
    {}

    void start() override
    {
        active = !completedOnce;
    }

    void update() override
    {
        avd->clearDrawableObjectVector();
        avd->pushDrawableObject(*mesh);
        avd->fitScene();
        end();
    }

    void end()
    {
        Parent::end();
        completedOnce = true;
    }
};

#endif