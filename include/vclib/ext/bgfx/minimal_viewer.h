#ifndef VCL_EXT_BGFX_MINIMAL_VIEWER_H
#define VCL_EXT_BGFX_MINIMAL_VIEWER_H

#include <vclib/math/min_max.h>
#include <vclib/space/box.h>

#include <vclib/gui/desktop_trackball.h>
#include <vclib/render/drawable_object_vector.h>

#include "drawable_mesh.h"

namespace vcl::bgf {

class MinimalViewer : public vcl::DesktopTrackBall<float>
{
    // this Viewer does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> drawList;

    // the program must be created after the uniforms - bgfx issue on OpenGL
    vcl::bgf::DrawableMeshProgram program;

protected:
    using DTB = vcl::DesktopTrackBall<float>;

public:

    MinimalViewer() {}

    MinimalViewer(std::shared_ptr<DrawableObjectVector> v)
    {
        setDrawableObjectVector(v);
    }

    std::shared_ptr<const DrawableObjectVector> drawableObjectVector() const
    {
        return drawList;
    }

    void setDrawableObjectVector(std::shared_ptr<DrawableObjectVector> v)
    {
        drawList = v;

        for (DrawableObject* obj : *drawList) {
            obj->init();

            if (dynamic_cast<bgf::GenericBGFXDrawableMesh*>(obj)) {
                bgf::GenericBGFXDrawableMesh* mesh =
                    dynamic_cast<bgf::GenericBGFXDrawableMesh*>(obj);
                mesh->setProgram(program);
            }
        }
    }

    void fitScene()
    {
        Box3d   bb          = drawList->boundingBox();
        Point3f sceneCenter = bb.center().cast<float>();
        float   sceneRadius = bb.diagonal() / 2;

        DTB::setTrackBall(sceneCenter, sceneRadius);
    }

    void draw() const
    {
        // This dummy draw call is here to make sure that view 0 is cleared
        // if no other draw calls are submitted to view 0.
        bgfx::touch(0);

        for (const DrawableObject* obj : *drawList)
            obj->draw();
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_MINIMAL_VIEWER_H
