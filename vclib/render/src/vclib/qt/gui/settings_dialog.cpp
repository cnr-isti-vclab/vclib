// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/settings_dialog.h>
#include "ui_settings_dialog.h"

#include <vclib/qt/gui/toolbar_frames/settings/bounding_box_editor_settings_frame.h>
#include <vclib/qt/gui/toolbar_frames/settings/selection_editor_settings_frame.h>

#include <vclib/base/system.h>

#include <QFile>
#include <QDir>
#include <QMessageBox>

#include <fstream>
#include <nlohmann/json.hpp>

namespace vcl::qt {

SettingsDialog::SettingsDialog(
    const SelectionEditorSettings&   selSts,
    const BoundingBoxEditorSettings& bboxSts,
    QWidget*                         parent) :
        QDialog(parent),
        mUI(new Ui::SettingsDialog),
        mSelSts(selSts),
        mBBoxSts(bboxSts)
{
    mUI->setupUi(this);

    // Set up the frames inside the tabs
    auto* selFrame = new SelectionEditorSettingsFrame(mSelSts, this);
    mUI->verticalLayout_selection->addWidget(selFrame);
    mUI->verticalLayout_selection->addStretch(); // push widgets to the top

    auto* bboxFrame = new BoundingBoxEditorSettingsFrame(mBBoxSts, this);
    mUI->verticalLayout_bbox->addWidget(bboxFrame);
    mUI->verticalLayout_bbox->addStretch(); // push widgets to the top

    // Sync QListWidget selection to QStackedWidget page
    connect(mUI->categoryList, &QListWidget::currentRowChanged,
            mUI->stackedWidget, &QStackedWidget::setCurrentIndex);

    // Apply button
    QPushButton* applyBtn = mUI->buttonBox->button(QDialogButtonBox::Apply);
    connect(applyBtn, &QPushButton::clicked, this, &SettingsDialog::onApplyClicked);

    // Save Defaults button
    connect(mUI->saveDefaultsButton, &QPushButton::clicked,
            this, &SettingsDialog::onSaveDefaultsClicked);
}

SettingsDialog::~SettingsDialog()
{
    delete mUI;
}

const SelectionEditorSettings& SettingsDialog::selectionSettings() const
{
    return mSelSts;
}

const BoundingBoxEditorSettings& SettingsDialog::boundingBoxSettings() const
{
    return mBBoxSts;
}

void SettingsDialog::onApplyClicked()
{
    emit applied();
}

void SettingsDialog::onSaveDefaultsClicked()
{
    nlohmann::json j;
    mSelSts.saveSettings(j["Editors"]);
    mBBoxSts.saveSettings(j["Editors"]);

    std::filesystem::path configDir = vcl::appConfigDirectory("vclib");
    std::string filePath = (configDir / "render_settings.json").string();

    // Ensure directory exists
    QDir dir;
    dir.mkpath(QString::fromStdString(configDir.string()));

    std::ofstream out(filePath);
    if (out.is_open()) {
        out << j.dump(4);
        out.close();
        QMessageBox::information(this, "Settings Saved", 
            "Default settings have been successfully saved.");
    } else {
        QMessageBox::warning(this, "Save Failed", 
            "Failed to save default settings to file.");
    }
}

} // namespace vcl::qt
