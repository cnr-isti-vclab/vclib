// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
 * This enum defines how a selection interacts with the current selection state.
 * The actions fall into two categories:
 *
 *   1. Box-based actions (REGULAR, ADD, SUBTRACT) - these require a
 *      user-drawn selection box (e.g. rectangle or lasso) to determine
 *      which primitives are affected.
 *
 *   2. Atomic actions (ALL, NONE, INVERT) - these operate on the entire
 *      selection set without needing any spatial input.
 */
enum class SelectionAction {
    REGULAR, ///< Replace the current selection with primitives inside the
             ///< selection box. This is the default interaction: clicking and
             ///< dragging a box selects only the primitives that fall within
             ///< it, clearing any previous selection.

    ADD, ///< Add primitives inside the selection box to the existing selection.
         ///< Primitives already selected remain selected; new ones inside the
         ///< box are appended to the selection set. Typically triggered with a
         ///< modifier key (e.g. Ctrl+drag).

    SUBTRACT, ///< Remove primitives inside the selection box from the existing
              ///< selection. Primitives that fall within the box are
              ///< deselected; others remain unaffected. Typically triggered
              ///< with a modifier key (e.g. Ctrl+Shift+drag).

    ALL, ///< Select all primitives of the specified type, regardless of
         ///< visibility or position. No selection box is needed or used.

    NONE, ///< Deselect all primitives, clearing the entire selection set.
          ///< No selection box is needed or used. Also known as "clear" or
          ///< "deselect all".

    INVERT ///< Toggle the selection state of every primitive of the specified
           ///< type. Selected primitives become deselected and vice versa. No
           ///< selection box is needed or used. Useful for quickly selecting
           ///< everything except what is currently chosen.
};

/**
 * @brief Describes a single selection operation.
 *
 * Composed of three orthogonal dimensions:
 *   - primitive: what to select (vertex, face, ...)
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
        bool               visible = false) :
            primitive(primitive), action(action), visible(visible)
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
