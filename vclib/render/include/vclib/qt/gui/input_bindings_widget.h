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

class InputBindingsWidget : public QWidget
{
    Q_OBJECT

    std::unique_ptr<Ui::InputBindingsWidget>       mUI;
    std::reference_wrapper<AbstractInputActionMap> mMap;
    std::map<std::string, std::string>             mPendingBindings;

public:
    explicit InputBindingsWidget(
        std::reference_wrapper<AbstractInputActionMap> map,
        QWidget*                                       parent = nullptr);
    ~InputBindingsWidget() override;

    void applySettings();

private:
    void populateTable();
};

} // namespace vcl::qt

#endif // VCL_QT_INPUT_BINDINGS_WIDGET_H
