// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/input_bindings_widget.h>

#include "ui_input_bindings_widget.h"

#include <vclib/render/input/abstract_input_action_map.h>

#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>

namespace vcl::qt {

InputBindingsWidget::InputBindingsWidget(
    std::reference_wrapper<AbstractInputActionMap> map,
    QWidget*                                       parent) :
        QWidget(parent), mUI(new Ui::InputBindingsWidget), mMap(map)
{
    mUI->setupUi(this);

    // Setup table headers
    mUI->bindingsTable->setColumnCount(2);
    mUI->bindingsTable->setHorizontalHeaderLabels({"Action", "Binding"});
    mUI->bindingsTable->horizontalHeader()->setStretchLastSection(true);
    mUI->bindingsTable->verticalHeader()->setVisible(false);

    // Auto-resize columns: 50/50 split for clean alignment
    mUI->bindingsTable->horizontalHeader()->setSectionResizeMode(
        0, QHeaderView::Stretch);
    mUI->bindingsTable->horizontalHeader()->setSectionResizeMode(
        1, QHeaderView::Stretch);

    // Add alternating row colors to help the eye read across
    mUI->bindingsTable->setAlternatingRowColors(true);

    populateTable();

    QPushButton* resetBtn = new QPushButton("Reset Map Defaults", this);
    resetBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    // The layout is automatically created by setupUi
    if (QVBoxLayout* l = qobject_cast<QVBoxLayout*>(this->layout())) {
        l->addWidget(resetBtn, 0, Qt::AlignRight);
    }

    connect(resetBtn, &QPushButton::clicked, this, [this]() {
        // Step 14 logic: reset to default
        // mPendingBindings.clear();
        // mMap.get().resetToDefaults();
        // populateTable();
    });
}

InputBindingsWidget::~InputBindingsWidget() = default;

void InputBindingsWidget::populateTable()
{
    const auto actions = mMap.get().actions();

    mUI->bindingsTable->setRowCount(0);
    mUI->bindingsTable->setRowCount(actions.size());

    for (int i = 0; i < (int) actions.size(); ++i) {
        const auto& action = actions[i];

        // Column 0: Action Name
        QTableWidgetItem* nameItem =
            new QTableWidgetItem(QString::fromStdString(action.name));
        nameItem->setFlags(
            nameItem->flags() & ~Qt::ItemIsEditable); // Read-only

        // Store the action ID in the UserRole of the first column
        nameItem->setData(Qt::UserRole, QString::fromStdString(action.id));
        mUI->bindingsTable->setItem(i, 0, nameItem);

        // Column 1: Action Binding (Button)
        QPushButton* bindingBtn = new QPushButton(
            QString::fromStdString(action.input), mUI->bindingsTable);
        bindingBtn->setStyleSheet("text-align: left; padding: 2px 5px;");
        bindingBtn->setCursor(Qt::PointingHandCursor);

        // We will connect this button in Step 14 for event interception
        connect(
            bindingBtn,
            &QPushButton::clicked,
            this,
            [this, bindingBtn, id = action.id]() {
                // Temporary placeholder for Step 14
                bindingBtn->setText("Press any key...");
                // TODO: Start listening
            });

        mUI->bindingsTable->setCellWidget(i, 1, bindingBtn);
    }

    // Disable internal scrolling and fix height to content
    mUI->bindingsTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    int totalHeight = mUI->bindingsTable->horizontalHeader()->height() + 2;
    for (int i = 0; i < mUI->bindingsTable->rowCount(); ++i) {
        totalHeight += mUI->bindingsTable->rowHeight(i);
    }
    mUI->bindingsTable->setMinimumHeight(totalHeight);
    mUI->bindingsTable->setMaximumHeight(totalHeight);
}

} // namespace vcl::qt
