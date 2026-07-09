// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_PBR_VIEWER_SETTINGS_H
#define VCL_RENDER_SETTINGS_PBR_VIEWER_SETTINGS_H

#include <vclib/base.h>

namespace vcl {

struct PBRViewerSettings
{
    enum class ToneMapping {
        NONE,
        BASIC,
        ACES_HILL,
        ACES_HILL_EXPOSURE_BOOST,
        ACES_NARKOWICZ,
        KHRONOS_PBR_NEUTRAL,
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
     * @brief Option that tells whether the viewer be set in PBR mode or not.
     *
     * All the options below are used only if this flag is set to true.
     */
    bool pbrMode = false;

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
     * @brief The exposure value to use in PBR mode.
     */
    float exposure = 1.0f;

    /**
     * @brief The tone mapping operator to use in PBR mode.
     */
    ToneMapping toneMapping = ToneMapping::ACES_HILL;
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_PBR_VIEWER_SETTINGS_H
