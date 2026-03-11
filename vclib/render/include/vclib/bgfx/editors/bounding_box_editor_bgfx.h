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

#ifndef VCL_BGFX_EDITORS_BOUNDING_BOX_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_BOUNDING_BOX_EDITOR_BGFX_H

#include <vclib/bgfx/drawable/drawable_box3.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/render/editors/editor.h>

namespace vcl {

template<typename ViewerDrawer>
class BoundingBoxEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

    std::vector<DrawableBox3> mBoxes;

public:
    BoundingBoxEditorBGFX()
    {
        Base::settings().customSettings["color"] = vcl::Color();
        Base::settings().customSettings["thickness"] = 2.0f;
    };

    void setActive(bool active) override
    {
        Base::setActive(active);
        Base::viewerUpdate();
    }

    void refresh() override
    {
        mBoxes.clear();

        assert(Base::settings().customSettings["color"].has_value());
        assert(Base::settings().customSettings["thickness"].has_value());

        Color c =
            std::any_cast<Color>(Base::settings().customSettings["color"]);
        float thickness = std::any_cast<float>(
            Base::settings().customSettings["thickness"]);

        for (const auto& drawable : *Base::drawList()) {

            const AbstractDrawableMesh* m =
                dynamic_cast<const AbstractDrawableMesh*>(drawable.get());
            if (m) {
                // note: the bounding box is already trasformed if the mesh has
                // a transform matrix
                mBoxes.push_back(DrawableBox3(m->boundingBox(), c, thickness));
            }
            else {
                mBoxes.push_back(DrawableBox3(Box3f()));
            }
        }
        Base::viewerUpdate();
    }

    void refreshSettings() override
    {
        assert(Base::settings().customSettings["color"].has_value());
        assert(Base::settings().customSettings["thickness"].has_value());

        Color c =
            std::any_cast<Color>(Base::settings().customSettings["color"]);
        float thickness = std::any_cast<float>(
            Base::settings().customSettings["thickness"]);

        for (auto& b : mBoxes) {
            b.setColor(c);
            b.setThickness(thickness);
        }
        Base::viewerUpdate();
    }

    void draw(uint viewId) const override
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
                bool show = (Base::settings().editMode == VISIBLE_OBJECTS) ?
                                Base::drawList()->at(i)->isVisible() :
                                true;

                if (show) {
                    mBoxes[i].draw(settings);
                }
            }
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_BOUNDING_BOX_EDITOR_BGFX_H
