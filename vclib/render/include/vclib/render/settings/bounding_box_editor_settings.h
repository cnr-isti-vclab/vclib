// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_BOUNDING_BOX_EDITOR_SETTINGS_H
#define VCL_RENDER_SETTINGS_BOUNDING_BOX_EDITOR_SETTINGS_H

#include <vclib/render/settings/editor_settings.h>

#include <vclib/space/core.h>

#include <nlohmann/json.hpp>

namespace vcl {

struct BoundingBoxEditorSettings : public EditorSettings
{
    vcl::Color color     = vcl::Color();
    float      thickness = 2.0f;

    void loadSettings(const nlohmann::json& j)
    {
        color     = j.value("color", color);
        thickness = j.value("thickness", thickness);
    }

    void saveSettings(nlohmann::json& j) const
    {
        j["color"]     = color;
        j["thickness"] = thickness;
    }
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_BOUNDING_BOX_EDITOR_SETTINGS_H
