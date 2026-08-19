// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_VIEWER_SETTINGS_H
#define VCL_RENDER_SETTINGS_VIEWER_SETTINGS_H

#include <vclib/base.h>
#include <vclib/render/settings/render_mode.h>
#include <vclib/render/settings/trackball_settings.h>

#include <vclib/space/core/color.h>

#include <nlohmann/json.hpp>

namespace vcl {

/**
 * @brief Contains the settings for the viewer, such as the active render
 * mode, exposure, and background visualization options.
 */
struct ViewerSettings : public TrackballSettings
{
    /**
     * @brief Global actions registered by the viewer or editors.
     */
    using ViewerGlobalActionMap =
        BindingMap<std::pair<Key::Enum, KeyModifiers>, std::string>;

    /**
     * @brief The tone mapping operators available when rendering.
     */
    enum class ToneMapping {
        /** @brief No tone mapping is applied. */
        NONE,
        /** @brief Basic (Reinhard) tone mapping. */
        BASIC,
        /** @brief ACES Filmic Tone Mapping (Hill). */
        ACES_HILL,
        /** @brief ACES Filmic Tone Mapping (Hill) with pre-exposure boost. */
        ACES_HILL_EXPOSURE_BOOST,
        /** @brief ACES Filmic Tone Mapping (Narkowicz). */
        ACES_NARKOWICZ,
        /** @brief Khronos PBR Neutral tone mapping. */
        KHRONOS_PBR_NEUTRAL,
        /** @brief Number of tone mapping operators. */
        COUNT
    };

    static constexpr const char*
        TONE_MAPPING_STRINGS[toUnderlying(ToneMapping::COUNT)] = {
            "None",
            "Basic",
            "ACES Hill",
            "ACES Hill Exposure Boost",
            "ACES Narkowicz",
            "Khronos PBR Neutral"};

    /**
     * @brief Option that tells which render mode the viewer should use.
     */
    RenderMode renderMode = RenderMode::CLASSIC;

    /**
     * @brief Option that tells whether to use image based lighting (IBL)
     * or not.
     *
     * @note this option is ignored if no panorama (environment map) is set.
     */
    bool imageBasedLighting = false;

    /**
     * @brief Option that tells whether to render the background panorama or
     * not.
     */
    bool renderBackgroundPanorama = false;

    /**
     * @brief The exposure value to use.
     */
    float exposure = 1.0f;

    /**
     * @brief The tone mapping operator to use.
     */
    ToneMapping toneMapping = ToneMapping::ACES_HILL;

    /**
     * @brief The background color of the canvas.
     */
    vcl::Color backgroundColor = vcl::Color::DarkGray;

    /**
     * @brief The path to the panorama environment map.
     */
    std::string panoramaPath = "";

    /**
     * @brief Resets the settings to their default values.
     */
    void resetDefaults()
    {
        renderMode               = RenderMode::CLASSIC;
        imageBasedLighting       = false;
        renderBackgroundPanorama = false;
        exposure                 = 1.0f;
        toneMapping              = ToneMapping::ACES_HILL;
        backgroundColor          = vcl::Color::DarkGray;
        panoramaPath             = "";
    }

    /**
     * @brief Loads the settings from a JSON object.
     * @param[in] j: the JSON object to read from.
     */
    void loadSettings(const nlohmann::json& j)
    {
        if (j.contains("ViewerSettings")) {
            const auto& js = j["ViewerSettings"];
            renderMode     = static_cast<RenderMode>(
                js.value("renderMode", static_cast<int>(renderMode)));
            imageBasedLighting =
                js.value("imageBasedLighting", imageBasedLighting);
            renderBackgroundPanorama =
                js.value("renderBackgroundPanorama", renderBackgroundPanorama);
            exposure    = js.value("exposure", exposure);
            toneMapping = static_cast<ToneMapping>(
                js.value("toneMapping", static_cast<int>(toneMapping)));
            backgroundColor = js.value("backgroundColor", backgroundColor);
            panoramaPath    = js.value("panoramaPath", panoramaPath);
        }
    }

    /**
     * @brief Saves the settings to a JSON object.
     * @param[out] j: the JSON object to write to.
     */
    void saveSettings(nlohmann::json& j) const
    {
        j["ViewerSettings"]["renderMode"] = static_cast<int>(renderMode);
        j["ViewerSettings"]["imageBasedLighting"] = imageBasedLighting;
        j["ViewerSettings"]["renderBackgroundPanorama"] =
            renderBackgroundPanorama;
        j["ViewerSettings"]["exposure"]        = exposure;
        j["ViewerSettings"]["toneMapping"]     = static_cast<int>(toneMapping);
        j["ViewerSettings"]["backgroundColor"] = backgroundColor;
        j["ViewerSettings"]["panoramaPath"]    = panoramaPath;
    }

    ViewerGlobalActionMap globalActionMap;
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_VIEWER_SETTINGS_H
