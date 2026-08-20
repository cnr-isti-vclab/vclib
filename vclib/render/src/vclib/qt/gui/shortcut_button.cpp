// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/shortcut_button.h>

#include <vclib/qt/input.h>

#include <QFocusEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

namespace vcl::qt {

ShortcutButton::ShortcutButton(
    AbstractInputActionMap::InputType expectedType,
    const QString&                    text,
    QWidget* parent) : QPushButton(text, parent), mExpectedType(expectedType)
{
    setFocusPolicy(Qt::StrongFocus);
    setStyleSheet("text-align: left; padding: 2px 5px;");
    setCursor(Qt::PointingHandCursor);
}

void ShortcutButton::startListening()
{
    if (mListening)
        return;
    mListening    = true;
    mOriginalText = text();
    setText("Press any key...");
    setFocus();
}

void ShortcutButton::keyPressEvent(QKeyEvent* event)
{
    if (!mListening) {
        QPushButton::keyPressEvent(event);
        return;
    }

    if (event->key() == Qt::Key_Escape) {
        mListening = false;
        setText(mOriginalText);
        clearFocus();
        return;
    }

    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        mListening = false;
        setText("None");
        if (onInputCaptured)
            onInputCaptured("");
        clearFocus();
        return;
    }

    if (mExpectedType != AbstractInputActionMap::InputType::KEY) {
        return;
    }

    if (event->key() == Qt::Key_Shift || event->key() == Qt::Key_Control ||
        event->key() == Qt::Key_Alt || event->key() == Qt::Key_Meta ||
        event->key() == Qt::Key_unknown) {
        return;
    }

    vcl::Key::Enum    vclKey   = vcl::qt::fromQt((Qt::Key) event->key());
    vcl::KeyModifiers vclMods  = vcl::qt::fromQt(event->modifiers());
    std::string       inputStr = vcl::toString(std::make_pair(vclKey, vclMods));

    mListening = false;
    setText(QString::fromStdString(inputStr));
    if (onInputCaptured)
        onInputCaptured(inputStr);
    clearFocus();
}

void ShortcutButton::mousePressEvent(QMouseEvent* event)
{
    if (!mListening) {
        QPushButton::mousePressEvent(event);
        return;
    }

    if (mExpectedType != AbstractInputActionMap::InputType::MOUSE_BUTTON) {
        return;
    }

    vcl::MouseButton::Enum vclBtn  = vcl::qt::fromQt(event->button());
    vcl::KeyModifiers      vclMods = vcl::qt::fromQt(event->modifiers());
    std::string inputStr = vcl::toString(std::make_pair(vclBtn, vclMods));

    mListening = false;
    setText(QString::fromStdString(inputStr));
    if (onInputCaptured)
        onInputCaptured(inputStr);
    clearFocus();
}

void ShortcutButton::wheelEvent(QWheelEvent* event)
{
    if (!mListening) {
        QPushButton::wheelEvent(event);
        return;
    }

    if (mExpectedType != AbstractInputActionMap::InputType::SCROLL_AXIS) {
        return;
    }

    vcl::ScrollAxis::Enum axis    = (std::abs(event->angleDelta().x()) >
                                     std::abs(event->angleDelta().y())) ?
                                        vcl::ScrollAxis::HORIZONTAL :
                                        vcl::ScrollAxis::VERTICAL;
    vcl::KeyModifiers     vclMods = vcl::qt::fromQt(event->modifiers());
    std::string inputStr = vcl::toString(std::make_pair(axis, vclMods));

    mListening = false;
    setText(QString::fromStdString(inputStr));
    if (onInputCaptured)
        onInputCaptured(inputStr);
    clearFocus();
}

void ShortcutButton::focusOutEvent(QFocusEvent* event)
{
    if (mListening) {
        mListening = false;
        setText(mOriginalText);
    }
    QPushButton::focusOutEvent(event);
}

} // namespace vcl::qt
