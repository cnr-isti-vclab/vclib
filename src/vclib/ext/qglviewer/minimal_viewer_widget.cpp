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

#include <vclib/ext/qglviewer/minimal_viewer_widget.h>

#include <vclib/math/min_max.h>
#include <vclib/space/box.h>

namespace vcl::qgl {

MinimalViewerWidget::MinimalViewerWidget(QWidget* parent) : QGLViewer(parent)
{
    mDrawList = std::make_shared<DrawableObjectVector>();
}

MinimalViewerWidget::MinimalViewerWidget(
    std::shared_ptr<DrawableObjectVector> v,
    QWidget*                              parent) :
        QGLViewer(parent),
        mDrawList(v)
{
}

void MinimalViewerWidget::init()
{
    for (DrawableObjectI* d : *mDrawList) {
        d->init();
    }
}

void MinimalViewerWidget::setDrawableObjectVector(
    std::shared_ptr<DrawableObjectVector> v)
{
    mDrawList = v;
}

std::shared_ptr<const DrawableObjectVector> MinimalViewerWidget::
    drawableObjectVector() const
{
    return mDrawList;
}

void MinimalViewerWidget::fitScene()
{
    Box3d   bb          = mDrawList->boundingBox();
    Point3d sceneCenter = bb.center();
    double  sceneRadius = bb.diagonal() / 2;

    setSceneCenter(
        qglviewer::Vec(sceneCenter.x(), sceneCenter.y(), sceneCenter.z()));
    setSceneRadius(sceneRadius);
    showEntireScene();
}

void MinimalViewerWidget::draw()
{
    for (DrawableObjectI* obj : *mDrawList)
        obj->draw(0);
}

} // namespace vcl::qgl
