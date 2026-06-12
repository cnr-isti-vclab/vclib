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

#ifndef VCL_RENDER_SELECTION_SELECTION_MODE_H
#define VCL_RENDER_SELECTION_SELECTION_MODE_H

namespace vcl {

/**
 * @brief The mesh primitive being selected.
 *
 * Extend this enum to support new primitive types (e.g. EDGE) without
 * touching the action enum or the SelectionMode struct.
 */
enum class SelectionPrimitive { VERTEX, FACE };

/**
 * @brief The selection operation to perform.
 *
 * REGULAR / ADD / SUBTRACT require a selection box.
 * ALL / NONE / INVERT are atomic (no box needed).
 */
enum class SelectionAction {
    REGULAR,
    ADD,
    SUBTRACT,
    ALL,
    NONE,
    INVERT
};

/**
 * @brief Describes a single selection operation.
 *
 * Composed of three orthogonal dimensions:
 *   - primitive: what to select (vertex, face, …)
 *   - action:    how to select (regular, add, subtract, all, none, invert)
 *   - visible:   whether to consider only screen-visible primitives (only
 *                meaningful for faces)
 *
 * Braced initialization:
 *   SelectionMode mode{SelectionPrimitive::FACE, SelectionAction::ADD, true};
 */
class SelectionMode
{
public:
    SelectionPrimitive primitive = SelectionPrimitive::VERTEX;
    SelectionAction    action    = SelectionAction::REGULAR;
    bool               visible   = false;

    SelectionMode() = default;

    constexpr SelectionMode(
        SelectionPrimitive primitive,
        SelectionAction    action,
        bool               visible = false)
        : primitive(primitive), action(action), visible(visible)
    {
    }

    /// Returns true for ALL / NONE / INVERT actions (no selection box needed).
    constexpr bool isAtomicAction() const
    {
        switch (action) {
        case SelectionAction::ALL:
        case SelectionAction::NONE:
        case SelectionAction::INVERT: return true;
        default: return false;
        }
    }

    constexpr bool isVertexSelection() const
    {
        return primitive == SelectionPrimitive::VERTEX;
    }

    constexpr bool isFaceSelection() const
    {
        return primitive == SelectionPrimitive::FACE;
    }

    constexpr bool isVisibleSelection() const
    {
        return visible && primitive == SelectionPrimitive::FACE;
    }
};

} // namespace vcl

#endif // VCL_RENDER_SELECTION_SELECTION_MODE_H
