// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/render/drawable/drawable_object_vector.h>

namespace vcl {

void DrawableObjectVector::init()
{
    for (auto& p : *this) {
        p->init();
    }
}

void DrawableObjectVector::draw(const DrawObjectSettings& settings)
{
    if (isVisible()) {
        for (const auto& p : *this) {
            if (p->isVisible())
                p->draw(settings);
        }
    }
}

void DrawableObjectVector::drawId(const DrawObjectSettings& settings)
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
