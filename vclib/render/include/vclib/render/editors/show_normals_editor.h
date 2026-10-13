// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_EDITORS_SHOW_NORMALS_EDITOR_H
#define VCL_RENDER_EDITORS_SHOW_NORMALS_EDITOR_H

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/editors/show_normals_editor_bgfx.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include "editor.h"

#include <vclib/render/settings/show_normals_editor_settings.h>
#endif

#include <nlohmann/json.hpp>

namespace vcl {

#ifdef VCLIB_RENDER_BACKEND_BGFX
template<typename ViewerDrawer>
using ShowNormalsEditor = ShowNormalsEditorBGFX<ViewerDrawer>;
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
// TODO: implement ShowNormalsEditorOpenGL2
template<typename ViewerDrawer>
class ShowNormalsEditor : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;
    ShowNormalsEditorSettings mSettings;

public:
    ShowNormalsEditor() = default;

    std::string name() const override { return "Show Normals"; }

    ShowNormalsEditorSettings& settings() override { return mSettings; }

    const ShowNormalsEditorSettings& settings() const override
    {
        return mSettings;
    }

    void loadSettings(const nlohmann::json& j) override
    {
        mSettings.loadSettings(j);
    }

    void saveSettings(nlohmann::json& j) const override
    {
        mSettings.saveSettings(j);
    }

    void draw(uint) override {}
};
#endif

} // namespace vcl

#endif // VCL_RENDER_EDITORS_SHOW_NORMALS_EDITOR_H
