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
#include <QPushButton>
#include <QVBoxLayout>

#include <fstream>

namespace vcl::qt {

SettingsDialog::SettingsDialog(
    const SettingsDialogData& data,
    QWidget* parent) : QDialog(parent), mUI(new Ui::SettingsDialog), mData(data)
{
    mUI->setupUi(this);

    mUI->categoryList->clear();
    while (mUI->stackedWidget->count() > 0) {
        QWidget* widget = mUI->stackedWidget->widget(0);
        mUI->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }

    std::map<QString, QTabWidget*> categoryTabs;

    for (const auto& tab : mData.tabs()) {
        QString cat = tab->category();
        if (categoryTabs.find(cat) == categoryTabs.end()) {
            mUI->categoryList->addItem(cat);
            QTabWidget* tabWidget = new QTabWidget();
            mUI->stackedWidget->addWidget(tabWidget);
            categoryTabs[cat] = tabWidget;
        }

        QWidget*     page   = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(page);

        QWidget* tabWidgetInstance = tab->createWidget(page);

        if (tabWidgetInstance->sizePolicy().verticalPolicy() ==
                QSizePolicy::Expanding ||
            tabWidgetInstance->sizePolicy().verticalPolicy() ==
                QSizePolicy::MinimumExpanding) {
            layout->addWidget(tabWidgetInstance, 1);
        }
        else {
            layout->addWidget(tabWidgetInstance, 0);
            layout->addStretch();
        }

        categoryTabs[cat]->addTab(page, tab->name());
    }

    if (mUI->categoryList->count() > 0) {
        mUI->categoryList->setCurrentRow(0);
        mUI->stackedWidget->setCurrentIndex(0);
    }

    // Sync QListWidget selection to QStackedWidget page
    connect(
        mUI->categoryList,
        &QListWidget::currentRowChanged,
        mUI->stackedWidget,
        &QStackedWidget::setCurrentIndex);

    // Apply and OK buttons
    QPushButton* applyBtn = mUI->buttonBox->button(QDialogButtonBox::Apply);
    connect(
        applyBtn, &QPushButton::clicked, this, &SettingsDialog::onApplyClicked);
    connect(this, &QDialog::accepted, this, &SettingsDialog::onApplyClicked);

    // Reset All Defaults button
    connect(mUI->resetAllDefaultsButton, &QPushButton::clicked, this, [this]() {
        const QList<QPushButton*> buttons =
            this->findChildren<QPushButton*>("resetDefaultButton");
        for (QPushButton* btn : buttons) {
            btn->click();
        }
    });
}

SettingsDialog::~SettingsDialog()
{
    delete mUI;
}

void SettingsDialog::onApplyClicked()
{
    if (mUI->saveAsDefaultCheckBox->isChecked()) {
        std::filesystem::path configDir = vcl::appConfigDirectory("vclib");
        std::string           filePath =
            (configDir / vcl::RENDER_SETTINGS_FILE_NAME).string();

        nlohmann::json j;

        // Read existing file to preserve other settings like ViewerSettings
        std::ifstream in(filePath);
        if (in.is_open()) {
            try {
                in >> j;
            }
            catch (...) {
                // Ignore parse errors and overwrite by resetting to an empty
                // object
                j = nlohmann::json::object();
            }
            in.close();
        }

        for (const auto& tab : mData.tabs()) {
            if (tab->category() == "Editors") {
                tab->saveSettings(j["Editors"]);
            }
            else {
                tab->saveSettings(j);
            }
        }

        // Ensure directory exists
        QDir dir;
        dir.mkpath(QString::fromStdString(configDir.string()));

        std::ofstream out(filePath);
        if (out.is_open()) {
            out << j.dump(4);
            out.close();
        }
        else {
            QMessageBox::warning(
                this,
                "Save Failed",
                "Failed to save default settings to file.");
        }
    }

    emit applied();
}

} // namespace vcl::qt
