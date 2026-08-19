// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_EDITORS_INFO_EDITOR_H
#define VCL_RENDER_EDITORS_INFO_EDITOR_H

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/editors/info_editor_bgfx.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include "editor.h"

#include <vclib/render/settings/info_editor_settings.h>
#endif

namespace vcl {

/**
 * @brief The InfoEditor class provides a viewer editor for displaying
 * information about the mesh elements (e.g. vertices, edges, faces) being
 * clicked.
 *
 * It retrieves the object and element ID beneath the mouse cursor from the GPU
 * readback, reads the element details from the mesh provider, and overlays this
 * data in the viewer as transient text and highlighting.
 *
 * @ingroup render_editors
 */
#ifdef VCLIB_RENDER_BACKEND_BGFX
template<typename ViewerDrawer>
using InfoEditor = InfoEditorBGFX<ViewerDrawer>;
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
// TODO: implement InfoEditorOpenGL2
template<typename ViewerDrawer>
class InfoEditor : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;
    InfoEditorSettings mSettings;

public:
    InfoEditor() = default;

    std::string name() const override { return "Info"; }

    InfoEditorSettings& settings() override { return mSettings; }

    const InfoEditorSettings& settings() const override { return mSettings; }

    void loadSettings(const nlohmann::json& j) override
    {
        mSettings.loadSettings(j);
    }

    void saveSettings(nlohmann::json& j) const override
    {
        mSettings.saveSettings(j);
    }

    void setActive(bool active) override
    {
        Base::setActive(active);
        Base::viewerUpdate();
    }

    void draw(uint) override {}
};
#endif

} // namespace vcl

#endif // VCL_RENDER_EDITORS_INFO_EDITOR_H
