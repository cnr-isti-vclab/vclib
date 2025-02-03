/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_BGFX_DRAWABLE_LINES_LINES_LINES_H
#define VCL_BGFX_DRAWABLE_LINES_LINES_LINES_H

#include "lines_settings.h"

namespace vcl::lines {

class Lines
{
    LinesSettings mSettings;

public:
    // TODO: remove this getter
    // add proper methots to set the settings that apply for lines
    // do then a similar class for polylines
    LinesSettings& settings() { return mSettings; }

    const LinesSettings& settings() const { return mSettings; }

    void swap(Lines& other)
    {
        using std::swap;

        swap(mSettings, other.mSettings);
    }

    friend void swap(Lines& a, Lines& b) { a.swap(b); }

protected:
    void bindSettingsUniformLines() const
    {
        mSettings.bindUniformLines();
    }

    void bindSettingsUniformPolylines() const
    {
        mSettings.bindUniformPolylines();
    }

    template<typename T>
    static std::pair<T*, bgfx::ReleaseFn> getAllocatedBufferAndReleaseFn(
        uint size)
    {
        T* buffer = new T[size];

        return std::make_pair(buffer, [](void* ptr, void*) {
            delete[] static_cast<T*>(ptr);
        });
    }
};

} // namespace vcl::lines

#endif // VCL_BGFX_DRAWABLE_LINES_LINES_LINES_H
