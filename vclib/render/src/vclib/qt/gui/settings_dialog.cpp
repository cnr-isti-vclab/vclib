// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/settings_dialog.h>

#include "ui_settings_dialog.h"

#include <vclib/render/settings.h>

#include <vclib/base.h>

#include <nlohmann/json.hpp>

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QVBoxLayout>

#include <fstream>

namespace vcl::qt {

SettingsDialog::SettingsDialog(
    const SettingsDialogData& data,
    QWidget* parent) : QDialog(parent), mUI(new Ui::SettingsDialog), mData(data)
{
    mUI->setupUi(this);

    // Populate Editors tab
    for (const auto& tab : mData.tabs()) {
        QWidget*     page   = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(page);
        layout->addWidget(tab->createWidget(page));
        layout->addStretch();

        mUI->editorsTabWidget->addTab(page, tab->name());
    }

    // Sync QListWidget selection to QStackedWidget page
    connect(
        mUI->categoryList,
        &QListWidget::currentRowChanged,
        mUI->stackedWidget,
        &QStackedWidget::setCurrentIndex);

    // Apply button
    QPushButton* applyBtn = mUI->buttonBox->button(QDialogButtonBox::Apply);
    connect(
        applyBtn, &QPushButton::clicked, this, &SettingsDialog::onApplyClicked);

    // Save Defaults button
    connect(
        mUI->saveDefaultsButton,
        &QPushButton::clicked,
        this,
        &SettingsDialog::onSaveDefaultsClicked);
}

SettingsDialog::~SettingsDialog()
{
    delete mUI;
}

// Removed hardcoded getters

void SettingsDialog::onApplyClicked()
{
    emit applied();
}

void SettingsDialog::onSaveDefaultsClicked()
{
    nlohmann::json j;
    for (const auto& tab : mData.tabs()) {
        tab->saveSettings(j["Editors"]);
    }

    std::filesystem::path configDir = vcl::appConfigDirectory("vclib");
    std::string filePath = (configDir / vcl::RENDER_SETTINGS_FILE_NAME).string();

    // Ensure directory exists
    QDir dir;
    dir.mkpath(QString::fromStdString(configDir.string()));

    std::ofstream out(filePath);
    if (out.is_open()) {
        out << j.dump(4);
        out.close();
        QMessageBox::information(
            this,
            "Settings Saved",
            "Default settings have been successfully saved.");
    }
    else {
        QMessageBox::warning(
            this, "Save Failed", "Failed to save default settings to file.");
    }
}

} // namespace vcl::qt
