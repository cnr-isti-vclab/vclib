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

#ifndef VCL_RENDER_SETTINGS_PBR_VIEWER_SETTINGS_H
#define VCL_RENDER_SETTINGS_PBR_VIEWER_SETTINGS_H

#include <vclib/base.h>

namespace vcl {

struct PBRViewerSettings
{
    enum class ToneMapping
    {
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
     * @brief Option that tells whether to render the background panorama or
     * not.
     */
    bool renderBackground = false;

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
