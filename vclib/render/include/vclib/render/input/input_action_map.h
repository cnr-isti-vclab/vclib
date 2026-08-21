// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_INPUT_INPUT_ACTION_MAP_H
#define VCL_RENDER_INPUT_INPUT_ACTION_MAP_H

#include <vclib/render/input/abstract_input_action_map.h>
#include <vclib/render/input/input.h>

#include <vclib/base.h>

#include <nlohmann/json.hpp>

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace vcl {

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
        ActionID           id;
        std::string        name;
        std::vector<Input> inputs;
        std::vector<Input> defaultInputs;
    };

    std::string               mMapName;
    std::vector<ActionDef>    mDefs;
    std::map<Input, ActionID> mBindings;

public:
    struct InitData
    {
        ActionID           id;
        std::string        name;
        std::vector<Input> defaultInputs = {};
    };

    InputActionMap(const std::string& name = "") : mMapName(name) {}

    void registerAction(
        const ActionID&           id,
        const std::string&        name,
        const std::vector<Input>& defaultInputs = {})
    {
        for (auto& def : mDefs) {
            if (def.id == id) {
                def.name          = name;
                def.defaultInputs = defaultInputs;
                if (def.inputs.empty() && !defaultInputs.empty()) {
                    def.inputs = defaultInputs;
                }
                updateBindings();
                return;
            }
        }
        mDefs.push_back({id, name, defaultInputs, defaultInputs});
        updateBindings();
    }

    void registerActions(std::initializer_list<InitData> actions)
    {
        for (const auto& a : actions) {
            registerAction(a.id, a.name, a.defaultInputs);
        }
    }

    void resetToDefaults() override
    {
        for (auto& def : mDefs) {
            def.inputs = def.defaultInputs;
        }
        updateBindings();
    }

    std::string mapName() const override { return mMapName; }

    InputType inputType() const override
    {
        if constexpr (
            std::is_same_v<Input, std::pair<Key::Enum, KeyModifiers>> ||
            std::is_same_v<Input, Key::Enum>) {
            return InputType::KEY;
        }
        else if constexpr (
            std::is_same_v<Input, std::pair<MouseButton::Enum, KeyModifiers>> ||
            std::is_same_v<Input, MouseInput> ||
            std::is_same_v<Input, MouseButton::Enum>) {
            return InputType::MOUSE_BUTTON;
        }
        else if constexpr (
            std::is_same_v<Input, std::pair<ScrollAxis::Enum, KeyModifiers>> ||
            std::is_same_v<Input, ScrollAxis::Enum>) {
            return InputType::SCROLL_AXIS;
        }
        else {
            return InputType::UNKNOWN;
        }
    }

    std::vector<AbstractInputActionMap::ActionInfo> actions() const override
    {
        std::vector<AbstractInputActionMap::ActionInfo> res;
        res.reserve(mDefs.size());
        for (const auto& def : mDefs) {
            AbstractInputActionMap::ActionInfo info;
            info.id   = toString(def.id);
            info.name = def.name;
            for (const auto& in : def.inputs) {
                info.inputs.push_back(toString(in));
            }
            for (const auto& in : def.defaultInputs) {
                info.defaultInputs.push_back(toString(in));
            }
            res.push_back(info);
        }
        return res;
    }

    void setBindings(
        const std::string&              actionId,
        const std::vector<std::string>& inputStrs) override
    {
        ActionID id = vcl::fromString<ActionID>(actionId);

        for (auto& def : mDefs) {
            if (def.id == id) {
                def.inputs.clear();
                for (const auto& inStr : inputStrs) {
                    if (!inStr.empty()) {
                        def.inputs.push_back(vcl::fromString<Input>(inStr));
                    }
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

    std::vector<Input> inputs(const ActionID& id) const
    {
        for (const auto& def : mDefs) {
            if (def.id == id) {
                return def.inputs;
            }
        }
        return {};
    }

    void loadSettings(const nlohmann::json& j) override
    {
        if (j.contains(mMapName)) {
            for (const auto& [actionId, jValue] : j[mMapName].items()) {
                std::vector<std::string> inputStrs;
                if (jValue.is_array()) {
                    for (const auto& item : jValue) {
                        inputStrs.push_back(item.template get<std::string>());
                    }
                }
                else if (jValue.is_string()) {
                    std::string str = jValue.template get<std::string>();
                    if (!str.empty()) {
                        inputStrs.push_back(str);
                    }
                }
                setBindings(actionId, inputStrs);
            }
        }
    }

    void saveSettings(nlohmann::json& j) const override
    {
        auto& mapJson = j[mMapName];
        for (const auto& def : mDefs) {
            std::vector<std::string> strs;
            for (const auto& in : def.inputs) {
                strs.push_back(toString(in));
            }
            mapJson[toString(def.id)] = strs;
        }
    }

private:
    // rebuilds the reverse (input -> action) lookup table used by action().
    // Each action can be bound to multiple Inputs; if two actions are bound to
    // the same Input, the last one in mDefs silently wins the lookup (the UI
    // is expected to warn about such conflicts, see checkConflicts()).
    void updateBindings()
    {
        mBindings.clear();
        for (const auto& def : mDefs) {
            for (const auto& in : def.inputs) {
                mBindings[in] = def.id;
            }
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_INPUT_INPUT_ACTION_MAP_H
