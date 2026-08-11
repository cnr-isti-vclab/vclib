// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_SETTINGS_DIALOG_DATA_H
#define VCL_QT_GUI_SETTINGS_DIALOG_DATA_H

#include "settings_dialog_tab.h"

#include <vector>
#include <memory>

namespace vcl::qt {

class SettingsDialogData {
    std::vector<std::shared_ptr<SettingsDialogTab>> mTabs;

public:
    SettingsDialogData() = default;

    void addTab(std::shared_ptr<SettingsDialogTab> tab) {
        mTabs.push_back(tab);
    }

    const std::vector<std::shared_ptr<SettingsDialogTab>>& tabs() const {
        return mTabs;
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_SETTINGS_DIALOG_DATA_H
