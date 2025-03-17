#ifndef MESH_CAHNGER_AUTOMATION_ACTION_H
#define MESH_CHANGER_AUTOMATION_ACTION_H

#include "automation_action.h"
#include <vclib/render/drawers/abstract_viewer_drawer.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/drawable/drawable_object_vector.h>


template<typename DerivedRenderApp, typename MeshType>
class MeshChangerAutomationAction : public AutomationAction
{
    vcl::AbstractViewerDrawer<DerivedRenderApp>* avd;
    std::shared_ptr<vcl::DrawableObjectVector> objectVector;
    bool completedOnce =  false;

    using Parent = AutomationAction;

    public:

    MeshChangerAutomationAction(
        vcl::AbstractViewerDrawer<DerivedRenderApp>* avd,
        vcl::DrawableMesh<MeshType> &mesh
    )
    : avd{avd},
    objectVector{std::make_shared<vcl::DrawableObjectVector>(vcl::DrawableObjectVector())}
    {
        objectVector->pushBack(mesh);
    }

    void start() override
    {
        Parent::start();
    }

    void update() override
    {
        if(completedOnce){
            end();
            return;
        }
        avd->setDrawableObjectVector(objectVector);
        avd->fitScene();
        end();
    }

    void end()
    {
        Parent::end();
        completedOnce = true;
    }

    std::shared_ptr<AutomationAction> clone() const & override
    {
        return std::make_shared<MeshChangerAutomationAction>(*this);
    }

    std::shared_ptr<AutomationAction> clone() && override
    {
        return std::make_shared<MeshChangerAutomationAction>(std::move(*this));
    }
};

#endif