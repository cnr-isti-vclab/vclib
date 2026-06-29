// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/clickable_label.h>

namespace vcl::qt {

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags) :
        QLabel(parent)
{
}

void ClickableLabel::mousePressEvent(QMouseEvent*)
{
    emit clicked();
}

} // namespace vcl::qt
