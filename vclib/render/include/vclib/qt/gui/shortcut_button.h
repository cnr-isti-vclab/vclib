// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_SHORTCUT_BUTTON_H
#define VCL_QT_SHORTCUT_BUTTON_H

#include <vclib/render/input/abstract_input_action_map.h>

#include <QPushButton>
#include <QTimer>

#include <functional>
#include <string>

namespace vcl::qt {

/**
 * @brief A push button that, when clicked, listens for the next key or mouse
 * input and reports it as a string via the onInputCaptured callback.
 *
 * Used by InputBindingsWidget to let the user interactively (re)assign a
 * shortcut to an action. The kind of input it listens for (key, mouse button
 * or scroll axis) is restricted by \p mExpectedType, so that e.g. a button
 * editing a mouse binding ignores keyboard events.
 */
class ShortcutButton : public QPushButton
{
    AbstractInputActionMap::InputType mExpectedType;
    bool                              mListening = false;
    QString                           mOriginalText;
    // used to distinguish a single click from the first click of a double
    // click when capturing mouse bindings
    QTimer*                           mDoubleClickTimer = nullptr;
    Qt::MouseButton                   mPendingButton;
    Qt::KeyboardModifiers             mPendingModifiers;

public:
    std::function<void(const std::string&)> onInputCaptured;

    explicit ShortcutButton(
        AbstractInputActionMap::InputType expectedType,
        const QString&                    text,
        QWidget*                          parent = nullptr);

    void startListening();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
};

} // namespace vcl::qt

#endif // VCL_QT_SHORTCUT_BUTTON_H
