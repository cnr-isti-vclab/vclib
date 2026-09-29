// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_INPUT_ABSTRACT_INPUT_ACTION_MAP_H
#define VCL_RENDER_INPUT_ABSTRACT_INPUT_ACTION_MAP_H

#include <nlohmann/json_fwd.hpp>

#include <string>
#include <vector>

namespace vcl {

/**
 * @brief Base polymorphic interface for action maps.
 *
 * This class provides a type-erased interface for the UI to interact with
 * input bindings. It allows retrieving a list of actions and assigning new
 * string-based inputs to them, without knowing the specific Input or ActionID
 * types used internally.
 */
class AbstractInputActionMap
{
public:
    /**
     * @brief Identifies the family of physical inputs this map accepts.
     */
    enum class InputType { KEY, MOUSE_BUTTON, SCROLL_AXIS, UNKNOWN };

    /**
     * @brief A Data Transfer Object (DTO) containing string-based information
     * about a registered action. Used by the UI to list and edit bindings
     * without knowing the underlying types.
     */
    struct ActionInfo
    {
        std::string              id;
        std::string              name;
        std::vector<std::string> inputs;
        std::vector<std::string> defaultInputs;
    };

    virtual ~AbstractInputActionMap() = default;

    /**
     * @brief Returns the human-readable name of this action map.
     *
     * This name is typically used by the UI to group related actions
     * (e.g., "Viewer Actions", "Mouse Bindings").
     *
     * @return The name of the map as a string.
     */
    virtual std::string mapName() const = 0;

    /**
     * @brief Returns the type of physical input this map accepts.
     * This is used by the UI to filter incoming events when listening for a new
     * shortcut.
     */
    virtual InputType inputType() const = 0;

    /**
     * @brief Retrieves the list of all registered actions and their current
     * input bindings.
     *
     * Implementations should return a vector of `ActionInfo` describing each
     * action. The `inputs` vector in `ActionInfo` should represent the current
     * physical inputs bound to the action, or be empty if no input is currently
     * bound.
     *
     * @return A vector containing the information for all registered actions.
     */
    virtual std::vector<ActionInfo> actions() const = 0;

    /**
     * @brief Assigns a new set of physical inputs to a specific action.
     *
     * This method is called by the UI when the user assigns new shortcuts to
     * an action. Implementations should parse each string in `inputStrs` back
     * into their physical input type and update the internal binding map.
     *
     * @param[in] actionId: The unique string identifier of the action (from
     * `ActionInfo::id`).
     * @param[in] inputStrs: The string representations of the physical inputs
     * (e.g. ["Ctrl+Left", "Middle Click"]). If empty, the bindings should be
     * removed.
     */
    virtual void setBindings(
        const std::string&              actionId,
        const std::vector<std::string>& inputStrs) = 0;

    /**
     * @brief Restores all bindings in the map to their original default values.
     */
    virtual void resetToDefaults() = 0;

    /**
     * @brief Loads the bindings from a JSON object.
     */
    virtual void loadSettings(const nlohmann::json& j) = 0;

    /**
     * @brief Saves the bindings to a JSON object.
     */
    virtual void saveSettings(nlohmann::json& j) const = 0;
};

} // namespace vcl

#endif // VCL_RENDER_INPUT_ABSTRACT_INPUT_ACTION_MAP_H
