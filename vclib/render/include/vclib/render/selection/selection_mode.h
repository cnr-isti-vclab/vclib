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
        FACE_INVERT,
        FACE_VISIBLE_REGULAR,
        FACE_VISIBLE_ADD,
        FACE_VISIBLE_SUBTRACT
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

    // I could cast to int and then do a range check and then re-cast to Enum,
    // but this is more comprehensible
    constexpr bool isAtomicMode() const
    {
        switch (mEnum) {
        case Enum::VERTEX_ALL:
        case Enum::VERTEX_INVERT:
        case Enum::VERTEX_NONE:
        case Enum::FACE_ALL:
        case Enum::FACE_INVERT:
        case Enum::FACE_NONE: return true;
        default: return false;
        }
    }

    // I could cast to int and then do a range check and then re-cast to Enum,
    // but this is more comprehensible
    constexpr bool isVertexSelection() const
    {
        switch (mEnum) {
        case Enum::VERTEX_REGULAR:
        case Enum::VERTEX_ADD:
        case Enum::VERTEX_SUBTRACT:
        case Enum::VERTEX_ALL:
        case Enum::VERTEX_NONE:
        case Enum::VERTEX_INVERT: return true;
        default: return false;
        }
    }

    // I could cast to int and then do a range check and then re-cast to Enum,
    // but this is more comprehensible
    constexpr bool isFaceSelection() const
    {
        switch (mEnum) {
        case Enum::FACE_REGULAR:
        case Enum::FACE_ADD:
        case Enum::FACE_SUBTRACT:
        case Enum::FACE_ALL:
        case Enum::FACE_NONE:
        case Enum::FACE_INVERT:
        case Enum::FACE_VISIBLE_REGULAR:
        case Enum::FACE_VISIBLE_ADD:
        case Enum::FACE_VISIBLE_SUBTRACT: return true;
        default: return false;
        }
    }

    constexpr bool isVisibleSelection() const
    {
        switch (mEnum) {
        case Enum::FACE_VISIBLE_REGULAR:
        case Enum::FACE_VISIBLE_ADD:
        case Enum::FACE_VISIBLE_SUBTRACT: return true;
        default: return false;
        }
    }

    // I could cast to int and then do an arithmetic operation and then re-cast
    // to Enum, but this is more comprehensible
    constexpr SelectionMode correspondingVertexMode() const
    {
        switch (mEnum) {
        case Enum::FACE_REGULAR: return SelectionMode(Enum::VERTEX_REGULAR);
        case Enum::FACE_ADD: return SelectionMode(Enum::VERTEX_ADD);
        case Enum::FACE_SUBTRACT: return SelectionMode(Enum::VERTEX_SUBTRACT);
        case Enum::FACE_ALL: return SelectionMode(Enum::VERTEX_ALL);
        case Enum::FACE_NONE: return SelectionMode(Enum::VERTEX_NONE);
        case Enum::FACE_INVERT: return SelectionMode(Enum::VERTEX_INVERT);
        default: return *this;
        }
    }

    // I could cast to int and then do an arithmetic operation and then re-cast
    // to Enum, but this is more comprehensible
    constexpr SelectionMode correspondingFaceMode() const
    {
        switch (mEnum) {
        case Enum::VERTEX_REGULAR: return SelectionMode(Enum::FACE_REGULAR);
        case Enum::VERTEX_ADD: return SelectionMode(Enum::FACE_ADD);
        case Enum::VERTEX_SUBTRACT: return SelectionMode(Enum::FACE_SUBTRACT);
        case Enum::VERTEX_ALL: return SelectionMode(Enum::FACE_ALL);
        case Enum::VERTEX_NONE: return SelectionMode(Enum::FACE_NONE);
        case Enum::VERTEX_INVERT: return SelectionMode(Enum::FACE_INVERT);
        default: return *this;
        }
    }

private:
    Enum mEnum;
};
} // namespace vcl

#endif
