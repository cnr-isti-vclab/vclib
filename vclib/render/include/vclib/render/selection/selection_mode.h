// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SELECTION_SELECTION_MODE_H
#define VCL_RENDER_SELECTION_SELECTION_MODE_H

#include <stdexcept>
#include <string>
#include <variant>

namespace vcl {

/**
 * @brief The mesh primitive being selected.
 *
 * Extend this enum to support new primitive types (e.g. EDGE) without
 * touching the action enum or the SelectionMode struct.
 */
enum class SelectionPrimitive { VERTEX, FACE };

/**
 * @brief Atomic selection operations that do not require spatial input.
 *
 * These operate on the entire selection set without needing any selection box.
 */
enum class SelectionAtomicAction {
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

inline std::string toString(SelectionAtomicAction action)
{
    switch (action) {
    case SelectionAtomicAction::ALL: return "Select All";
    case SelectionAtomicAction::NONE: return "Deselect All";
    case SelectionAtomicAction::INVERT: return "Invert Selection";
    default: return "Unknown";
    }
}

inline void fromString(const std::string& str, SelectionAtomicAction& out)
{
    if (str == "Select All")
        out = SelectionAtomicAction::ALL;
    else if (str == "Deselect All")
        out = SelectionAtomicAction::NONE;
    else if (str == "Invert Selection")
        out = SelectionAtomicAction::INVERT;
    else
        throw std::invalid_argument(
            "Invalid SelectionAtomicAction string: " + str);
}

/**
 * @brief Box-based selection operations.
 *
 * These require a user-drawn selection box (e.g. rectangle or lasso) to
 * determine which primitives are affected.
 */
enum class SelectionDragAction {
    REGULAR, ///< Replace the current selection with primitives inside the
             ///< selection box. This is the default interaction: clicking and
             ///< dragging a box selects only the primitives that fall within
             ///< it, clearing any previous selection.

    ADD, ///< Add primitives inside the selection box to the existing selection.
         ///< Primitives already selected remain selected; new ones inside the
         ///< box are appended to the selection set. Typically triggered with a
         ///< modifier key (e.g. Ctrl+drag).

    SUBTRACT ///< Remove primitives inside the selection box from the existing
             ///< selection. Primitives that fall within the box are
             ///< deselected; others remain unaffected. Typically triggered
             ///< with a modifier key (e.g. Ctrl+Shift+drag).
};

inline std::string toString(SelectionDragAction action)
{
    switch (action) {
    case SelectionDragAction::REGULAR: return "Regular Selection";
    case SelectionDragAction::ADD: return "Add to Selection";
    case SelectionDragAction::SUBTRACT: return "Subtract from Selection";
    default: return "Unknown";
    }
}

inline void fromString(const std::string& str, SelectionDragAction& out)
{
    if (str == "Regular Selection")
        out = SelectionDragAction::REGULAR;
    else if (str == "Add to Selection")
        out = SelectionDragAction::ADD;
    else if (str == "Subtract from Selection")
        out = SelectionDragAction::SUBTRACT;
    else
        throw std::invalid_argument(
            "Invalid SelectionDragAction string: " + str);
}

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
 *   SelectionMode mode{SelectionPrimitive::FACE, SelectionDragAction::ADD,
 * true};
 */
class SelectionMode
{
public:
    using SelectionAction =
        std::variant<SelectionDragAction, SelectionAtomicAction>;

    SelectionPrimitive primitive = SelectionPrimitive::VERTEX;
    SelectionAction    action    = SelectionDragAction::REGULAR;
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
        return std::holds_alternative<SelectionAtomicAction>(action);
    }

    constexpr bool isAction(SelectionDragAction a) const
    {
        return std::holds_alternative<SelectionDragAction>(action) &&
               std::get<SelectionDragAction>(action) == a;
    }

    constexpr bool isAction(SelectionAtomicAction a) const
    {
        return std::holds_alternative<SelectionAtomicAction>(action) &&
               std::get<SelectionAtomicAction>(action) == a;
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
