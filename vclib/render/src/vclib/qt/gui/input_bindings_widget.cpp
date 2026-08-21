// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/input_bindings_widget.h>

#include "ui_input_bindings_widget.h"
#include <QTimer>

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
            mPendingBindings[action.id] = action.defaultInputs;
        }
        QTimer::singleShot(0, this, [this]() {
            populateTable();
        });
        emit bindingsChanged();
    });
}

InputBindingsWidget::~InputBindingsWidget() = default;

void InputBindingsWidget::applySettings()
{
    for (const auto& [id, inputStrs] : mPendingBindings) {
        mMap.get().setBindings(id, inputStrs);
    }
}

void InputBindingsWidget::populateTable()
{
    const auto actions = mMap.get().actions();

    mUI->bindingsTable->setRowCount(0);
    mUI->bindingsTable->setRowCount(actions.size());

    for (int i = 0; i < (int) actions.size(); ++i) {
        const auto& action   = actions[i];
        std::string actionId = action.id;

        // Column 0: Action Name
        QTableWidgetItem* nameItem =
            new QTableWidgetItem(QString::fromStdString(action.name));
        nameItem->setFlags(
            nameItem->flags() & ~Qt::ItemIsEditable); // Read-only

        // Store the action ID in the UserRole of the first column
        nameItem->setData(Qt::UserRole, QString::fromStdString(actionId));
        mUI->bindingsTable->setItem(i, 0, nameItem);

        // Column 1: Action Binding (Container)
        QWidget*     bindingWidget = new QWidget(mUI->bindingsTable);
        QHBoxLayout* bindingLayout = new QHBoxLayout(bindingWidget);
        bindingLayout->setContentsMargins(0, 0, 0, 0);
        bindingLayout->setSpacing(5);

        QLabel* warningLabel = new QLabel("⚠️", bindingWidget);
        warningLabel->setObjectName("warningLabel");
        warningLabel->hide();
        bindingLayout->addWidget(warningLabel);

        // Container for dynamic shortcut buttons
        QWidget*     shortcutsContainer = new QWidget(bindingWidget);
        QHBoxLayout* shortcutsLayout    = new QHBoxLayout(shortcutsContainer);
        shortcutsLayout->setContentsMargins(0, 0, 0, 0);
        shortcutsLayout->setSpacing(5);
        bindingLayout->addWidget(shortcutsContainer);

        QToolButton* addBtn = new QToolButton(bindingWidget);
        addBtn->setText("➕");
        addBtn->setToolTip("Add shortcut");
        addBtn->setStyleSheet(
            "QToolButton { color: green; border: none; font-weight: bold; }");
        bindingLayout->addWidget(addBtn);

        bindingLayout->addStretch(); // push everything left

        // Lambda to redraw the shortcuts container for this action
        auto redrawShortcuts = [this,
                                shortcutsContainer,
                                shortcutsLayout,
                                actionId]() {
            // clear layout
            QLayoutItem* item;
            while ((item = shortcutsLayout->takeAt(0)) != nullptr) {
                if (QWidget* widget = item->widget()) {
                    widget->deleteLater();
                }
                delete item;
            }

            const auto& inputs = currentInputs(actionId);

            for (size_t j = 0; j < inputs.size(); ++j) {
                const auto& inputStr = inputs[j];

                QWidget*     pairWidget = new QWidget(shortcutsContainer);
                QHBoxLayout* pairLayout = new QHBoxLayout(pairWidget);
                pairLayout->setContentsMargins(0, 0, 0, 0);
                pairLayout->setSpacing(2);

                ShortcutButton* bindingBtn = new ShortcutButton(
                    mMap.get().inputType(),
                    QString::fromStdString(
                        inputStr.empty() ? "Listening..." : inputStr),
                    pairWidget);
                bindingBtn->setObjectName("shortcutButton");
                bindingBtn->setSizePolicy(
                    QSizePolicy::Minimum, QSizePolicy::Fixed);

                QToolButton* unbindBtn = new QToolButton(pairWidget);
                unbindBtn->setText("❌");
                unbindBtn->setToolTip("Unbind this shortcut");
                unbindBtn->setStyleSheet(
                    "QToolButton { color: red; border: none; font-weight: "
                    "bold; }");

                pairLayout->addWidget(bindingBtn);
                pairLayout->addWidget(unbindBtn);

                shortcutsLayout->addWidget(pairWidget);

                connect(
                    bindingBtn, &QPushButton::clicked, this, [bindingBtn]() {
                        bindingBtn->startListening();
                    });

                bindingBtn->onInputCaptured = [this, actionId, j](
                                                  const std::string& newStr) {
                    if (mPendingBindings.find(actionId) ==
                        mPendingBindings.end()) {
                        mPendingBindings[actionId] = currentInputs(actionId);
                    }
                    mPendingBindings[actionId][j] = newStr;
                    emit bindingsChanged();
                };

                connect(
                    unbindBtn,
                    &QToolButton::clicked,
                    this,
                    [this, actionId, j]() {
                        if (mPendingBindings.find(actionId) ==
                            mPendingBindings.end()) {
                            mPendingBindings[actionId] =
                                currentInputs(actionId);
                        }
                        auto& vec = mPendingBindings[actionId];
                        if (j < vec.size()) {
                            vec.erase(vec.begin() + j);
                        }

                        // Queue a safe table redraw to reflect the deletion
                        QTimer::singleShot(0, this, [this]() {
                            populateTable();
                        });
                        emit bindingsChanged();
                    });
            }

            // Auto-start listening on the newly added empty shortcut
            if (!inputs.empty() && inputs.back().empty()) {
                if (shortcutsLayout->count() > 0) {
                    QWidget* lastPair =
                        shortcutsLayout->itemAt(shortcutsLayout->count() - 1)
                            ->widget();
                    if (QPushButton* pBtn = lastPair->findChild<QPushButton*>(
                            "shortcutButton")) {
                        if (ShortcutButton* btn =
                                static_cast<ShortcutButton*>(pBtn)) {
                            btn->startListening();
                        }
                    }
                }
            }
        };

        connect(
            addBtn,
            &QToolButton::clicked,
            this,
            [this, actionId, redrawShortcuts]() {
                if (mPendingBindings.find(actionId) == mPendingBindings.end()) {
                    mPendingBindings[actionId] = currentInputs(actionId);
                }
                mPendingBindings[actionId].push_back(""); // Add empty slot
                redrawShortcuts();
            });

        // Initial draw
        redrawShortcuts();

        mUI->bindingsTable->setCellWidget(i, 1, bindingWidget);
    }

    // Disable internal scrolling and fix height to content
    mUI->bindingsTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    int totalHeight = 2; // For borders
    if (mUI->bindingsTable->horizontalHeader()->isVisible()) {
        totalHeight += mUI->bindingsTable->horizontalHeader()->height();
    }

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

std::vector<InputBindingsWidget::ActionInfo> InputBindingsWidget::getActions()
    const
{
    std::vector<ActionInfo> actions;
    for (const auto& a : mMap.get().actions()) {
        actions.push_back(ActionInfo {a.id, a.name});
    }
    return actions;
}

std::vector<std::string> InputBindingsWidget::currentInputs(
    const std::string& actionId) const
{
    if (mPendingBindings.count(actionId))
        return mPendingBindings.at(actionId);

    for (const auto& a : mMap.get().actions()) {
        if (a.id == actionId) {
            return a.inputs;
        }
    }
    return {};
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
