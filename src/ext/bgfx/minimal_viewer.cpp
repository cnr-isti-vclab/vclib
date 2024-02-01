/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <vclib/ext/bgfx/minimal_viewer.h>

#include <vclib/math/min_max.h>
#include <vclib/render/generic_drawable_mesh.h>
#include <vclib/space/box.h>

namespace vcl::bgf {

MinimalViewer::MinimalViewer(uint width, uint height) : DTB(width, height)
{
    cameraUniforms.updateCamera(DTB::camera());
    directionalLightUniforms.updateLight(DTB::light());
    axis.setShaderProgram(axisProgram);
}

MinimalViewer::MinimalViewer(
    std::shared_ptr<DrawableObjectVector> v,
    uint                                  width,
    uint                                  height) :
        MinimalViewer(width, height)
{
    setDrawableObjectVector(v);
}

std::shared_ptr<const DrawableObjectVector> MinimalViewer::
    drawableObjectVector() const
{
    return drawList;
}

void MinimalViewer::setDrawableObjectVector(
    std::shared_ptr<DrawableObjectVector> v)
{
    drawList = v;

    for (DrawableObject* obj : *drawList) {
        obj->init();

        GenericDrawableMesh* mesh = dynamic_cast<GenericDrawableMesh*>(obj);
        if (mesh) {
            mesh->setShaderProgram(meshProgram);
        }
    }
}

void MinimalViewer::fitScene()
{
    Box3d   bb          = drawList->boundingBox();
    Point3f sceneCenter = bb.center().cast<float>();
    float   sceneRadius = bb.diagonal() / 2;

    DTB::setTrackBall(sceneCenter, sceneRadius);
}

void MinimalViewer::draw(uint viewID)
{
    bgfx::setViewTransform(
        viewID, viewMatrix().data(), projectionMatrix().data());

    cameraUniforms.updateCamera(DTB::camera());
    cameraUniforms.bind();

    directionalLightUniforms.bind();

    for (DrawableObject* obj : *drawList)
        obj->draw(viewID);

    if (axis.isVisible()) {
        axis.draw(viewID);
    }
}

void MinimalViewer::keyPress(Key key)
{
    if (key == KEY_A) {
        toggleAxisVisibility();
    }
    else {
        DTB::keyPress(key);
    }
}

} // namespace vcl::bgf
