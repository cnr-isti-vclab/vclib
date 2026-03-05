/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include <vclib/bgfx/editors/bounding_box_editor.h>

namespace vcl {

void BoundingBoxEditor::refresh()
{
    mBoxes.clear();

    for (const std::shared_ptr<DrawableObject>& drawable : *Base::drawList()) {

        const AbstractDrawableMesh* m =
            dynamic_cast<const AbstractDrawableMesh*>(drawable.get());
        if (m) {
            mBoxes.push_back(DrawableBox3(m->boundingBox(), Color::Red, 2.0f));
        }
        else {
            mBoxes.push_back(DrawableBox3(Box3f()));
        }
    }
}

void BoundingBoxEditor::draw(uint viewId) const
{
    using enum EditorSettings::EditMode;

    DrawObjectSettings settings;
    settings.viewId = viewId;

    if (Base::settings().editMode == NONE)
        return;

    if (Base::settings().editMode == CURRENT_OBJECT) {
        uint id = Base::drawList()->selectedObjectId();
        if (id < mBoxes.size()) {
            mBoxes[id].draw(settings);
        }
    }
    else {
        for (uint i = 0; i < mBoxes.size(); ++i) {
            bool show = (Base::settings().editMode == VISIBLE_OBJECTS)
                            ? Base::drawList()->at(i)->isVisible()
                            : true;

            if (show) {
                const AbstractDrawableMesh* m = getDrawableMesh(i);

                if (m) {
                    Matrix44f transform = m->transformMatrix().cast<float>();
                    bgfx::setTransform(transform.data());
                }

                mBoxes[i].draw(settings);
            }
        }
    }
}

const AbstractDrawableMesh* BoundingBoxEditor::getDrawableMesh(uint i) const
{
    if (i < Base::drawList()->size()) {
         return dynamic_cast<const AbstractDrawableMesh*>(
             Base::drawList()->at(i).get());
    }
    else {
        return nullptr;
    }
}

AbstractDrawableMesh* BoundingBoxEditor::getDrawableMesh(uint i)
{
    if (i < Base::drawList()->size()) {
        return dynamic_cast<AbstractDrawableMesh*>(
            Base::drawList()->at(i).get());
    }
    else {
        return nullptr;
    }
}

} // namespace vcl
