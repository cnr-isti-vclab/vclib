// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_INPUT_BINDING_MAP_H
#define VCL_RENDER_INPUT_BINDING_MAP_H

#include <map>
#include <optional>

namespace vcl {

/**
 * @brief A class that maps input events to actions.
 *
 * This class provides a bidirectional mapping between input events (like key
 * presses or mouse clicks) and the corresponding actions they trigger. It is
 * designed to be efficient for event handling (Input -> Action lookup) while
 * also providing the ability to retrieve and modify bindings easily for user
 * interfaces (Action -> Input lookup).
 *
 * Internally, it uses a single std::map to ensure that event handling lookups
 * are performed in O(log N) time, while inverse lookups and updates are
 * performed linearly, which is perfectly acceptable given the typically small
 * number of bindings.
 *
 * @tparam Input The type representing the input event (e.g., a pair of key and
 * modifiers).
 * @tparam Action The type representing the action triggered by the input.
 */
template<typename Input, typename Action>
class BindingMap
{
private:
    std::map<Input, Action> mMap;

public:
    /**
     * @brief Default constructor.
     */
    BindingMap() = default;

    /**
     * @brief Constructor with an initializer list of bindings.
     *
     * @param[in] init: Initializer list of input-action pairs.
     */
    BindingMap(std::initializer_list<std::pair<const Input, Action>> init) :
            mMap(init)
    {
    }

    /**
     * @brief Gets the action associated with a specific input.
     *
     * @param[in] input: The input event.
     * @return An std::optional containing the associated action, or
     * std::nullopt if the input is not bound to any action.
     */
    std::optional<Action> action(const Input& input) const
    {
        auto it = mMap.find(input);
        if (it != mMap.end())
            return it->second;
        return std::nullopt;
    }

    /**
     * @brief Gets the input associated with a specific action.
     *
     * @param[in] action: The action to look up.
     * @return An std::optional containing the associated input, or std::nullopt
     * if the action has no binding.
     */
    std::optional<Input> input(const Action& action) const
    {
        for (const auto& [inp, act] : mMap) {
            if (act == action)
                return inp;
        }
        return std::nullopt;
    }

    /**
     * @brief Sets or updates the binding for a specific action.
     *
     * If the action was previously bound to another input, the old binding is
     * removed. If the new input was already bound to a different action, it
     * will be reassigned to the new action (collision resolution).
     *
     * @param[in] action: The action to bind.
     * @param[in] input: The input event to associate with the action.
     */
    void setBinding(const Action& action, const Input& input)
    {
        for (auto it = mMap.begin(); it != mMap.end();) {
            if (it->second == action) {
                it = mMap.erase(it);
            } else {
                ++it;
            }
        }
        mMap[input] = action;
    }

    /**
     * @brief Returns the underlying map of input-action bindings.
     *
     * @return A constant reference to the underlying std::map.
     */
    const std::map<Input, Action>& map() const { return mMap; }
};

} // namespace vcl

#endif // VCL_RENDER_INPUT_BINDING_MAP_H
