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

#include <vclib/ext/bgfx/minimal_viewer.h>

#include <vclib/math/min_max.h>
#include <vclib/render/interfaces/drawable_mesh_i.h>
#include <vclib/space/box.h>

namespace vcl::bgf {

MinimalViewer::MinimalViewer(uint width, uint height) : DTB(width, height)
{
    cameraUniforms.updateCamera(DTB::camera());
    cameraUniforms.updateNearFar(nearPlane(), farPlane());
    directionalLightUniforms.updateLight(DTB::light());
    axis.setShaderProgram(axisProgram);
    directionalLight.setShaderProgram(directionalLightProgram);
}

MinimalViewer::MinimalViewer(
    std::shared_ptr<DrawableObjectVector> v,
    uint                                  width,
    uint                                  height) :
        MinimalViewer(width, height)
{
    setDrawableObjectVector(v);
}

const DrawableObjectVector& MinimalViewer::drawableObjectVector() const
{
    return *drawList;
}

void MinimalViewer::setDrawableObjectVector(
    std::shared_ptr<DrawableObjectVector> v)
{
    drawList = v;

    for (DrawableObjectI* obj : *drawList) {
        initDrawableObject(*obj);
    }
}

uint MinimalViewer::pushDrawableObject(const DrawableObjectI& obj)
{
    drawList->pushBack(obj);
    initDrawableObject(drawList->back());
    return drawList->size() - 1;
}

void MinimalViewer::fitScene()
{
    Box3d   bb          = drawList->boundingBox();
    Point3f sceneCenter = bb.center().cast<float>();
    float   sceneRadius = bb.diagonal() / 2;

    DTB::setTrackBall(sceneCenter, sceneRadius);
}

void MinimalViewer::draw(uint viewId)
{
    bgfx::setViewTransform(
        viewId, viewMatrix().data(), projectionMatrix().data());

    cameraUniforms.updateCamera(DTB::camera());
    cameraUniforms.updateNearFar(nearPlane(), farPlane());
    cameraUniforms.bind();

    directionalLightUniforms.bind();

    for (DrawableObjectI* obj : *drawList)
        obj->draw(viewId);

    if (axis.isVisible()) {
        axis.draw(viewId);
    }

    if (directionalLight.isVisible()) {
        directionalLight.draw(viewId);
    }
}

void MinimalViewer::initDrawableObject(DrawableObjectI& obj)
{
    obj.init();

    DrawableMeshI* mesh = dynamic_cast<DrawableMeshI*>(&obj);
    if (mesh) {
        mesh->setShaderProgram(meshProgram);
    }
}

} // namespace vcl::bgf
