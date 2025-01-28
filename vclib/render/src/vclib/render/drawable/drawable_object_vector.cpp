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

void DrawableObjectVector::draw(uint viewId) const
{
    if (isVisible()) {
        for (const auto& p : *this) {
            p->draw(viewId);
        }
    }
}

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

std::shared_ptr<DrawableObject> DrawableObjectVector::clone() const
{
    return std::make_shared<DrawableObjectVector>(*this);
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
