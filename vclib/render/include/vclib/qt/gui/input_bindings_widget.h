// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_INPUT_BINDINGS_WIDGET_H
#define VCL_QT_INPUT_BINDINGS_WIDGET_H

#include <QWidget>

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace vcl {
class AbstractInputActionMap;
} // namespace vcl

namespace vcl::qt {

namespace Ui {
class InputBindingsWidget;
} // namespace Ui

/**
 * @brief A widget listing all the actions of a single AbstractInputActionMap,
 * each with a ShortcutButton to view and reassign its current binding.
 *
 * Edits made by the user are buffered in mPendingBindings and are only
 * written back to the underlying action map when applySettings() is called,
 * so that closing the settings dialog without applying discards the changes.
 */
class InputBindingsWidget : public QWidget
{
    Q_OBJECT

    std::unique_ptr<Ui::InputBindingsWidget>       mUI;
    std::reference_wrapper<AbstractInputActionMap> mMap;
    // actionId -> pending input strings, not yet applied to mMap
    std::map<std::string, std::vector<std::string>> mPendingBindings;

public:
    struct ActionInfo
    {
        std::string id;
        std::string name;
    };

    explicit InputBindingsWidget(
        std::reference_wrapper<AbstractInputActionMap> map,
        QWidget*                                       parent = nullptr);
    ~InputBindingsWidget() override;

    void applySettings();

    // Conflict resolution interface
    int         inputType() const;
    std::string mapName() const;

    std::vector<ActionInfo> getActions() const;

    std::vector<std::string> currentInputs(const std::string& actionId) const;

    void setConflict(
        const std::string& actionId,
        bool               hasConflict,
        const QString&     tooltip = "");

    void clearAllConflicts();

signals:
    void bindingsChanged();

private:
    void populateTable();
};

} // namespace vcl::qt

#endif // VCL_QT_INPUT_BINDINGS_WIDGET_H
