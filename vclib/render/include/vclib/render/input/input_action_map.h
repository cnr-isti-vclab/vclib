// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_INPUT_INPUT_ACTION_MAP_H
#define VCL_RENDER_INPUT_INPUT_ACTION_MAP_H

#include <vclib/render/input/input.h>
#include <vclib/base/concepts/serialization.h>

#include <map>
#include <optional>
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
     * @brief A Data Transfer Object (DTO) containing string-based information
     * about a registered action. Used by the UI to list and edit bindings
     * without knowing the underlying types.
     */
    struct ActionInfo
    {
        std::string id;
        std::string name;
        std::string input;
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
     * @brief Retrieves the list of all registered actions and their current
     * input bindings.
     *
     * Implementations should return a vector of `ActionInfo` describing each
     * action. The `input` string in `ActionInfo` should represent the current
     * physical input bound to the action, or be empty if no input is currently
     * bound.
     *
     * @return A vector containing the information for all registered actions.
     */
    virtual std::vector<ActionInfo> actions() const = 0;

    /**
     * @brief Assigns a new physical input to a specific action.
     *
     * This method is called by the UI when the user assigns a new shortcut to
     * an action. Implementations should parse `inputStr` back into their
     * physical input type and update the internal binding map.
     *
     * @param[in] actionId The unique string identifier of the action (from
     * `ActionInfo::id`).
     * @param[in] inputStr The string representation of the physical input (e.g.
     * "Ctrl+Left"). If empty, the binding should be removed.
     */
    virtual void setBinding(
        const std::string& actionId,
        const std::string& inputStr) = 0;
};

/**
 * @brief A map that associates specific Input events to Action identifiers.
 *
 * This class acts as the single source of truth for input actions, mapping a
 * physical input (like a mouse click or a keyboard key) to a specific action.
 * Actions must be explicitly registered using `registerAction()`.
 *
 * @tparam Input The type of the input event (e.g. `std::pair<MouseButton::Enum,
 * KeyModifiers>`).
 * It must support `vcl::toString()` and `vcl::fromString()`.
 * @tparam ActionID The type identifying the action. It can be an `enum class`
 * (e.g., `TrackballMotionType`), a `std::string`, or any other type.
 * It must support `vcl::toString()` and `vcl::fromString()`.
 */
template<StringConvertible Input, StringConvertible ActionID>
class InputActionMap : public AbstractInputActionMap
{
    struct ActionDef
    {
        ActionID             id;
        std::string          name;
        std::optional<Input> input;
    };

    std::string               mMapName;
    std::vector<ActionDef>    mDefs;
    std::map<Input, ActionID> mBindings;

public:
    InputActionMap(const std::string& name = "") : mMapName(name) {}

    void registerAction(
        const ActionID&      id,
        const std::string&   name,
        std::optional<Input> defaultInput)
    {
        mDefs.push_back({id, name, defaultInput});
        updateBindings();
    }

    std::string mapName() const override { return mMapName; }

    std::vector<AbstractInputActionMap::ActionInfo> actions() const override
    {
        std::vector<AbstractInputActionMap::ActionInfo> res;
        res.reserve(mDefs.size());
        for (const auto& def : mDefs) {
            std::string inputStr =
                def.input.has_value() ? vcl::toString(def.input.value()) : "";
            res.push_back({vcl::toString(def.id), def.name, inputStr});
        }
        return res;
    }

    void setBinding(const std::string& actionId, const std::string& inputStr)
        override
    {
        ActionID id;
        vcl::fromString(actionId, id);

        for (auto& def : mDefs) {
            if (def.id == id) {
                if (inputStr.empty()) {
                    def.input = std::nullopt;
                }
                else {
                    Input in;
                    vcl::fromString(inputStr, in);
                    def.input = in;
                }
                updateBindings();
                return;
            }
        }
    }

    bool hasBinding(const Input& in) const
    {
        return mBindings.find(in) != mBindings.end();
    }

    std::optional<ActionID> action(const Input& in) const
    {
        auto it = mBindings.find(in);
        if (it != mBindings.end()) {
            return it->second;
        }
        return std::nullopt;
    }

private:
    void updateBindings()
    {
        mBindings.clear();
        for (const auto& def : mDefs) {
            if (def.input.has_value()) {
                mBindings[def.input.value()] = def.id;
            }
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_INPUT_INPUT_ACTION_MAP_H
