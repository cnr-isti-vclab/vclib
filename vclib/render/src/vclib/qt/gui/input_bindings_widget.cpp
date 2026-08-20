// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/input_bindings_widget.h>

#include "ui_input_bindings_widget.h"

#include <vclib/qt/gui/shortcut_button.h>
#include <vclib/render/input/abstract_input_action_map.h>

#include <QHeaderView>
#include <QLabel>
#include <QToolButton>
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
    resetBtn->setObjectName("resetDefaultButton");
    resetBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    // The layout is automatically created by setupUi
    if (QVBoxLayout* l = qobject_cast<QVBoxLayout*>(this->layout())) {
        l->addWidget(resetBtn, 0, Qt::AlignRight);
    }

    connect(resetBtn, &QPushButton::clicked, this, [this]() {
        const auto actions = mMap.get().actions();
        for (const auto& action : actions) {
            mPendingBindings[action.id] = action.defaultInput;

            // Find the button and update its text
            for (int i = 0; i < mUI->bindingsTable->rowCount(); ++i) {
                QTableWidgetItem* nameItem = mUI->bindingsTable->item(i, 0);
                if (nameItem &&
                    nameItem->data(Qt::UserRole).toString().toStdString() ==
                        action.id) {
                    QWidget* w = mUI->bindingsTable->cellWidget(i, 1);
                    if (w) {
                        if (QPushButton* btn =
                                w->findChild<QPushButton*>("shortcutButton")) {
                            btn->setText(
                                QString::fromStdString(
                                    action.defaultInput.empty() ?
                                        "None" :
                                        action.defaultInput));
                        }
                    }
                    break;
                }
            }
        }
        emit bindingsChanged();
    });
}

InputBindingsWidget::~InputBindingsWidget() = default;

void InputBindingsWidget::applySettings()
{
    for (const auto& [id, inputStr] : mPendingBindings) {
        mMap.get().setBinding(id, inputStr);
    }
}

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

        // Column 1: Action Binding (Button + Unbind)
        QWidget*     bindingWidget = new QWidget(mUI->bindingsTable);
        QHBoxLayout* bindingLayout = new QHBoxLayout(bindingWidget);
        bindingLayout->setContentsMargins(0, 0, 0, 0);
        bindingLayout->setSpacing(5);

        ShortcutButton* bindingBtn = new ShortcutButton(
            mMap.get().inputType(),
            QString::fromStdString(
                action.input.empty() ? "None" : action.input),
            bindingWidget);
        bindingBtn->setObjectName("shortcutButton");

        bindingBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        QLabel* warningLabel = new QLabel("⚠️", bindingWidget);
        warningLabel->setObjectName("warningLabel");
        // Orange warning sign, no bold needed since emoji
        warningLabel->hide();

        QToolButton* unbindBtn = new QToolButton(bindingWidget);
        unbindBtn->setText("❌");
        unbindBtn->setToolTip("Unbind this action");
        unbindBtn->setStyleSheet(
            "QToolButton { color: red; border: none; font-weight: bold; }");

        bindingLayout->addWidget(warningLabel);
        bindingLayout->addWidget(bindingBtn);
        bindingLayout->addWidget(unbindBtn);

        // We connect this button for event interception
        connect(bindingBtn, &QPushButton::clicked, this, [bindingBtn]() {
            bindingBtn->startListening();
        });

        bindingBtn->onInputCaptured =
            [this, id = action.id](const std::string& inputStr) {
                mPendingBindings[id] = inputStr;
                emit bindingsChanged();
            };

        connect(unbindBtn, &QToolButton::clicked, this, [bindingBtn]() {
            bindingBtn->setText("None");
            if (bindingBtn->onInputCaptured) {
                bindingBtn->onInputCaptured("");
            }
        });

        mUI->bindingsTable->setCellWidget(i, 1, bindingWidget);
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

int InputBindingsWidget::inputType() const
{
    return static_cast<int>(mMap.get().inputType());
}

std::string InputBindingsWidget::mapName() const
{
    return mMap.get().mapName();
}

std::vector<InputBindingsWidget::ActionInfo> InputBindingsWidget::getActions() const
{
    std::vector<ActionInfo> actions;
    for (const auto& a : mMap.get().actions()) {
        actions.push_back(ActionInfo{a.id, a.name});
    }
    return actions;
}

std::string InputBindingsWidget::currentInput(const std::string& actionId) const
{
    if (mPendingBindings.count(actionId))
        return mPendingBindings.at(actionId);
        
    for (const auto& a : mMap.get().actions()) {
        if (a.id == actionId) {
            return a.input;
        }
    }
    return "";
}

void InputBindingsWidget::setConflict(
    const std::string& actionId,
    bool               hasConflict,
    const QString&     tooltip)
{
    for (int i = 0; i < mUI->bindingsTable->rowCount(); ++i) {
        QTableWidgetItem* nameItem = mUI->bindingsTable->item(i, 0);
        if (nameItem &&
            nameItem->data(Qt::UserRole).toString().toStdString() == actionId) {
            QWidget* w = mUI->bindingsTable->cellWidget(i, 1);
            if (w) {
                if (QLabel* warningLabel =
                        w->findChild<QLabel*>("warningLabel")) {
                    warningLabel->setVisible(hasConflict);
                    warningLabel->setToolTip(tooltip);
                }
            }
            break;
        }
    }
}

void InputBindingsWidget::clearAllConflicts()
{
    for (int i = 0; i < mUI->bindingsTable->rowCount(); ++i) {
        QWidget* w = mUI->bindingsTable->cellWidget(i, 1);
        if (w) {
            if (QLabel* warningLabel = w->findChild<QLabel*>("warningLabel")) {
                warningLabel->setVisible(false);
                warningLabel->setToolTip("");
            }
        }
    }
}

} // namespace vcl::qt
