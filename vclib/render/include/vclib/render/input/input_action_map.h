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
        ActionID             id;
        std::string          name;
        std::optional<Input> input;
        std::optional<Input> defaultInput;
    };

    std::string               mMapName;
    std::vector<ActionDef>    mDefs;
    std::map<Input, ActionID> mBindings;

public:
    struct InitData
    {
        ActionID             id;
        std::string          name;
        std::optional<Input> defaultInput = std::nullopt;
    };

    InputActionMap(const std::string& name = "") : mMapName(name) {}

    void registerAction(
        const ActionID&      id,
        const std::string&   name,
        std::optional<Input> defaultInput = std::nullopt)
    {
        for (auto& def : mDefs) {
            if (def.id == id) {
                def.name         = name;
                def.defaultInput = defaultInput;
                if (!def.input.has_value() && defaultInput.has_value()) {
                    def.input = defaultInput;
                }
                updateBindings();
                return;
            }
        }
        mDefs.push_back({id, name, defaultInput, defaultInput});
        updateBindings();
    }

    void registerActions(std::initializer_list<InitData> actions)
    {
        for (const auto& a : actions) {
            registerAction(a.id, a.name, a.defaultInput);
        }
    }

    void resetToDefaults()
    {
        for (auto& def : mDefs) {
            def.input = def.defaultInput;
        }
        updateBindings();
    }

    std::string mapName() const override { return mMapName; }

    InputType inputType() const override
    {
        if constexpr (std::is_same_v<Input, std::pair<Key::Enum, KeyModifiers>> || 
                      std::is_same_v<Input, Key::Enum>) {
            return InputType::KEY;
        }
        else if constexpr (std::is_same_v<Input, std::pair<MouseButton::Enum, KeyModifiers>> ||
                           std::is_same_v<Input, MouseButton::Enum>) {
            return InputType::MOUSE_BUTTON;
        }
        else if constexpr (std::is_same_v<Input, std::pair<ScrollAxis::Enum, KeyModifiers>> ||
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
            std::string inputStr =
                def.input.has_value() ? toString(def.input.value()) : "";
            res.push_back({toString(def.id), def.name, inputStr});
        }
        return res;
    }

    void setBinding(const std::string& actionId, const std::string& inputStr)
        override
    {
        ActionID id = vcl::fromString<ActionID>(actionId);

        for (auto& def : mDefs) {
            if (def.id == id) {
                if (inputStr.empty()) {
                    def.input = std::nullopt;
                }
                else {
                    Input in  = vcl::fromString<Input>(inputStr);
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

    std::optional<Input> input(const ActionID& id) const
    {
        for (const auto& def : mDefs) {
            if (def.id == id) {
                return def.input;
            }
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
