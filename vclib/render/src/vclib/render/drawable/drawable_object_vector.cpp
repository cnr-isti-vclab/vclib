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

#include <vclib/render/drawable/drawable_object_vector.h>

namespace vcl {

void DrawableObjectVector::init()
{
    for (auto& p : *this) {
        p->init();
    }
}

void DrawableObjectVector::draw(const DrawObjectSettings& settings) const
{
    if (isVisible()) {
        for (const auto& p : *this) {
            if (p->isVisible())
                p->draw(settings);
        }
    }
}

void DrawableObjectVector::drawId(const DrawObjectSettings& settings) const
{
    DrawObjectSettings sts = settings;
    if (isVisible()) {
        for (size_t idx = 0; idx < Base::size(); idx++) {
            // TODO: combine idx with the content of settings.objectId
            sts.objectId = idx;

            const auto& p = Base::at(idx);

            if (p->isVisible())
                p->drawId(sts);
        }
    }
}

// TODO: distinguish the box of the visible objects VS the box of all objects
Box3d DrawableObjectVector::boundingBox() const
{
    Box3d bb;
    if (Base::size() > 0) {
        uint i = firstVisibleObject();

        for (; i < Base::size(); i++) { // rest of the list
            if (Base::at(i)->isVisible()) {
                bb.add(Base::at(i)->boundingBox());
            }
        }
    }
    return bb;
}

Point3d DrawableObjectVector::center() const
{
    Box3d bb = boundingBox();
    if (bb.isNull())
        return Point3d(0, 0, 0);
    return bb.center();
}

std::shared_ptr<DrawableObject> DrawableObjectVector::clone() const&
{
    return std::make_shared<DrawableObjectVector>(*this);
}

std::shared_ptr<DrawableObject> DrawableObjectVector::clone() &&
{
    return std::make_shared<DrawableObjectVector>(std::move(*this));
}

bool DrawableObjectVector::isVisible() const
{
    return mVisible;
}

void DrawableObjectVector::setVisibility(bool vis)
{
    mVisible = vis;
}

uint DrawableObjectVector::firstVisibleObject() const
{
    for (uint i = 0; i < Base::size(); i++)
        if (Base::at(i)->isVisible())
            return i;
    return UINT_NULL;
}

} // namespace vcl
