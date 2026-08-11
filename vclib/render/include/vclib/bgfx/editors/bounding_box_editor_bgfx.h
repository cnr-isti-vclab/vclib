// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_BOUNDING_BOX_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_BOUNDING_BOX_EDITOR_BGFX_H

#include <vclib/bgfx/drawable/drawable_box3.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/render/editors/editor.h>
#include <vclib/render/settings/bounding_box_editor_settings.h>

namespace vcl {

template<typename ViewerDrawer>
class BoundingBoxEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

    std::vector<DrawableBox3> mBoxes;

    BoundingBoxEditorSettings mSettings;

public:
    BoundingBoxEditorBGFX() = default;

    std::string name() const override { return "Bounding Box"; }

    BoundingBoxEditorSettings& settings() override { return mSettings; }

    const BoundingBoxEditorSettings& settings() const override
    {
        return mSettings;
    }

    void setActive(bool active) override
    {
        Base::setActive(active);
        Base::viewerUpdate();
    }

    void refresh() override
    {
        mBoxes.clear();

        Color c         = mSettings.color;
        float thickness = mSettings.thickness;

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
        Color c         = mSettings.color;
        float thickness = mSettings.thickness;

        for (auto& b : mBoxes) {
            b.setColor(c);
            b.setThickness(thickness);
        }
        Base::viewerUpdate();
    }

    void drawContent(uint viewId) override
    {
        using enum EditorSettings::EditMode;

        DrawObjectSettings settings;
        settings.viewId = viewId;

        if (mSettings.editMode == NONE)
            return;

        if (mSettings.editMode == CURRENT_OBJECT) {
            uint id = Base::drawList()->selectedObjectId();
            if (id < mBoxes.size()) {
                mBoxes[id].draw(settings);
            }
        }
        else {
            for (uint i = 0; i < mBoxes.size(); ++i) {
                bool show = (mSettings.editMode == VISIBLE_OBJECTS) ?
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
