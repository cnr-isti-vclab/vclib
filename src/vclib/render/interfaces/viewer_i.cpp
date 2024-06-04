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

#include <vclib/render/interfaces/viewer_i.h>

namespace vcl {

const DrawableObjectVector& ViewerI::drawableObjectVector() const
{
    return *mDrawList;
}

void ViewerI::setDrawableObjectVector(std::shared_ptr<DrawableObjectVector> v)
{
    mDrawList = v;

    for (DrawableObjectI* obj : *mDrawList) {
        obj->init();
    }
    fitScene();
}

uint ViewerI::pushDrawableObject(const DrawableObjectI& obj)
{
    mDrawList->pushBack(obj);
    mDrawList->back().init();
    return mDrawList->size() - 1;
}

void ViewerI::fitScene()
{
    Point3f sceneCenter;
    float   sceneRadius = 1;

    Box3d bb = mDrawList->boundingBox();

    if (!bb.isNull()) {
        sceneCenter = bb.center().cast<float>();
        sceneRadius = bb.diagonal() / 2;
    }

    DTB::setTrackBall(sceneCenter, sceneRadius);
}

} // namespace vcl
