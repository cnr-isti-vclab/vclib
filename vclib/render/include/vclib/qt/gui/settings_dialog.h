// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_SETTINGS_DIALOG_H
#define VCL_QT_GUI_SETTINGS_DIALOG_H

#include "settings_dialog/settings_dialog_data.h"

#include <QDialog>
#include <QToolBar>

namespace vcl::qt {

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

    Ui::SettingsDialog* mUI;

    const SettingsDialogData& mData;

public:
    explicit SettingsDialog(
        const SettingsDialogData& data,
        QWidget*                  parent = nullptr);
    ~SettingsDialog();

signals:
    void applied();

private slots:
    void onApplyClicked();
    void onSaveDefaultsClicked();

};

} // namespace vcl::qt

#endif // VCL_QT_GUI_SETTINGS_DIALOG_H
