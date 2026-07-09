// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_INPUT_H
#define VCL_QT_INPUT_H

#include <vclib/render/input.h>

#include <QMouseEvent>

namespace vcl::qt {

MouseButton::Enum fromQt(Qt::MouseButton button);

KeyModifier::Enum fromQt(Qt::KeyboardModifier modifier);

Key::Enum fromQt(Qt::Key key, Qt::KeyboardModifiers modifiers = Qt::NoModifier);

KeyModifiers fromQt(Qt::KeyboardModifiers modifiers);

} // namespace vcl::qt

#endif // VCL_QT_INPUT_H
