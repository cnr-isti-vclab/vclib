// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/input_bindings_widget.h>
#include <vclib/qt/gui/settings_dialog/settings_dialog_data.h>
#include <vclib/qt/gui/settings_dialog/shortcuts_settings_tab.h>
#include <vclib/render/input/abstract_input_action_map.h>

#include <QGroupBox>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

namespace vcl::qt {

QString ShortcutsSettingsTab::category() const
{
    return "Shortcuts";
}

QString ShortcutsSettingsTab::name() const
{
    return "Key Bindings";
}

QWidget* ShortcutsSettingsTab::createWidget(QWidget* parent)
{
    QScrollArea* scrollArea = new QScrollArea(parent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget*     scrollContent = new QWidget(scrollArea);
    QVBoxLayout* layout        = new QVBoxLayout(scrollContent);

    auto groups = mProvider();
    for (const auto& group : groups) {
        QLabel* titleLabel =
            new QLabel(QString::fromStdString(group.name), scrollContent);
        QFont font = titleLabel->font();
        font.setBold(true);
        font.setPointSize(font.pointSize() + 2);
        titleLabel->setFont(font);
        layout->addWidget(titleLabel);

        for (const auto& mapRef : group.maps) {
            QGroupBox* groupBox = new QGroupBox(
                QString::fromStdString(mapRef.get().mapName()), scrollContent);
            QVBoxLayout* groupLayout = new QVBoxLayout(groupBox);

            InputBindingsWidget* bindingsWidget =
                new InputBindingsWidget(mapRef, groupBox);
            groupLayout->addWidget(bindingsWidget);
            mWidgets.push_back(bindingsWidget);

            layout->addWidget(groupBox);
        }

        layout->addSpacing(10);
    }

    if (groups.empty()) {
        QLabel* placeholderLabel =
            new QLabel("No shortcut available.", scrollContent);
        placeholderLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(placeholderLabel);
    }

    layout->addStretch();
    scrollArea->setWidget(scrollContent);

    return scrollArea;
}

void ShortcutsSettingsTab::applySettings()
{
    for (auto* w : mWidgets) {
        // w->applySettings(); // uncomment this line when step 13 is
        // implemented
    }
}

void ShortcutsSettingsTab::saveSettings(nlohmann::json& /*j*/) const
{
}

} // namespace vcl::qt
