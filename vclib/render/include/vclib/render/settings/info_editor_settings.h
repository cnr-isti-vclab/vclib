// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_INFO_EDITOR_SETTINGS_H
#define VCL_RENDER_SETTINGS_INFO_EDITOR_SETTINGS_H

#include <vclib/render/settings/editor_settings.h>

#include <vclib/space/core.h>

#include <nlohmann/json.hpp>

namespace vcl {

struct InfoEditorSettings : public EditorSettings
{
    vcl::Color color     = vcl::Color::Red;
    float      thickness = 5.0f;
    vcl::Color textColor = vcl::Color::Black;
    int        textSize  = 20;

    /**
     * @brief Resets the settings to their default values.
     */
    void resetDefaults()
    {
        color     = vcl::Color::Red;
        thickness = 5.0f;
        textColor = vcl::Color::Black;
        textSize  = 20;
    }

    /**
     * @brief Loads the settings from a JSON object.
     * @param[in] j: the JSON object to read from.
     */
    void loadSettings(const nlohmann::json& j)
    {
        if (j.contains("InfoEditor")) {
            const auto& jBox = j["InfoEditor"];
            color     = jBox.value("color", color);
            thickness = jBox.value("thickness", thickness);
            textColor = jBox.value("textColor", textColor);
            textSize  = jBox.value("textSize", textSize);
        }
    }

    /**
     * @brief Saves the settings to a JSON object.
     * @param[out] j: the JSON object to write to.
     */
    void saveSettings(nlohmann::json& j) const
    {
        j["InfoEditor"]["color"]     = color;
        j["InfoEditor"]["thickness"] = thickness;
        j["InfoEditor"]["textColor"] = textColor;
        j["InfoEditor"]["textSize"]  = textSize;
    }
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_INFO_EDITOR_SETTINGS_H
