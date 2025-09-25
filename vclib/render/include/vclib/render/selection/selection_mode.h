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

#ifndef VCL_SELECTION_MODE_H
#define VCL_SELECTION_MODE_H

namespace vcl {
class SelectionMode
{
public:
    enum Enum {
        VERTEX_REGULAR,
        VERTEX_ADD,
        VERTEX_SUBTRACT,
        VERTEX_ALL,
        VERTEX_NONE,
        VERTEX_INVERT,
        FACE_REGULAR,
        FACE_ADD,
        FACE_SUBTRACT,
        FACE_ALL,
        FACE_NONE,
        FACE_INVERT
    };

    SelectionMode() = default;

    constexpr SelectionMode(Enum en) : mEnum(en) {}

    SelectionMode& operator=(SelectionMode& other)
    {
        mEnum = other.mEnum;
        return *this;
    }

    SelectionMode& operator=(Enum en)
    {
        mEnum = en;
        return *this;
    }

    constexpr operator Enum() const { return mEnum; }

    constexpr operator bool() const = delete;

    constexpr bool isAtomicMode()
    {
        switch (mEnum) {
        case VERTEX_ALL:
        case VERTEX_INVERT:
        case VERTEX_NONE:
        case FACE_ALL:
        case FACE_INVERT:
        case FACE_NONE: return true;
        default: return false;
        }
    }

    constexpr bool isVertexSelection()
    {
        switch (mEnum) {
        case VERTEX_REGULAR:
        case VERTEX_ADD:
        case VERTEX_SUBTRACT:
        case VERTEX_ALL:
        case VERTEX_NONE:
        case VERTEX_INVERT: return true;
        default: return false;
        }
    }

    constexpr bool isFaceSelection()
    {
        switch (mEnum) {
        case FACE_REGULAR:
        case FACE_ADD:
        case FACE_SUBTRACT:
        case FACE_ALL:
        case FACE_NONE:
        case FACE_INVERT: return true;
        default: return false;
        }
    }

private:
    Enum mEnum;
};
} // namespace vcl

#endif
