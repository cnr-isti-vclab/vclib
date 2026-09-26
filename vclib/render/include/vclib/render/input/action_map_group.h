// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_INPUT_ACTION_MAP_GROUP_H
#define VCL_RENDER_INPUT_ACTION_MAP_GROUP_H

#include <functional>
#include <string>
#include <vector>

namespace vcl {

class AbstractInputActionMap;

/**
 * @brief A struct that groups a list of action maps under a common owner name.
 *
 * This structure is primarily used to aggregate all the action maps exported 
 * by a specific owner (such as the Viewer or a specific Editor) so that 
 * they can be dynamically exposed to the UI (e.g., the Settings Dialog) 
 * in an organized and categorized manner.
 */
struct ActionMapGroup {
    /** 
     * @brief The name of the group (e.g., "Viewer", "Mesh Selector Editor"). 
     * This is used as the category title in the UI. 
     */
    std::string name;

    /** 
     * @brief A list of mutable references to the action maps owned by this group.
     * The UI can use these references to display and modify the key bindings 
     * in real time.
     */
    std::vector<std::reference_wrapper<AbstractInputActionMap>> maps;
};

} // namespace vcl

#endif // VCL_RENDER_INPUT_ACTION_MAP_GROUP_H
