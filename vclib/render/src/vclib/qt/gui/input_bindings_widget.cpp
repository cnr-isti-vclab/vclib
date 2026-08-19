// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/input_bindings_widget.h>

#include "ui_input_bindings_widget.h"

#include <vclib/render/input/abstract_input_action_map.h>

namespace vcl::qt {

InputBindingsWidget::InputBindingsWidget(
    std::reference_wrapper<AbstractInputActionMap> map,
    QWidget*                                       parent) :
        QWidget(parent), mUI(new Ui::InputBindingsWidget), mMap(map)
{
    mUI->setupUi(this);
}

InputBindingsWidget::~InputBindingsWidget() = default;

} // namespace vcl::qt
