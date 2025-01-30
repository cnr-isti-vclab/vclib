/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef IMGUI_DISABLE
#include <vclib/qt_imgui/imgui_impl_qt.h>

#include <QApplication>
#include <QClipboard>
#include <QEvent>
#include <QKeyEvent>

#include <chrono>

using Clock     = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

// forward declaration
class ImGuiQtEventFilter;

// data stored within ImGui::GetIO().BackendPlatformUserData
struct ImGui_ImplQt_Data
{
    QWidget*            Widget      = nullptr;
    ImGuiQtEventFilter* EventFilter = nullptr;
    TimePoint           LastTime    = TimePoint(Clock::duration::zero());
};

static ImGui_ImplQt_Data* ImGui_ImplQt_GetBackendData()
{
    return ImGui::GetCurrentContext() ?
               (ImGui_ImplQt_Data*) ImGui::GetIO().BackendPlatformUserData :
               nullptr;
}

#ifndef QT_NO_CURSOR
static constexpr Qt::CursorShape QtMouseCursors[ImGuiMouseCursor_COUNT] = {
    Qt::ArrowCursor,        // ImGuiMouseCursor_Arrow
    Qt::IBeamCursor,        // ImGuiMouseCursor_TextInput
    Qt::SizeAllCursor,      // ImGuiMouseCursor_ResizeAll
    Qt::SizeVerCursor,      // ImGuiMouseCursor_ResizeNS
    Qt::SizeHorCursor,      // ImGuiMouseCursor_ResizeEW
    Qt::SizeBDiagCursor,    // ImGuiMouseCursor_ResizeNESW
    Qt::SizeFDiagCursor,    // ImGuiMouseCursor_ResizeNWSE
    Qt::PointingHandCursor, // ImGuiMouseCursor_Hand
    Qt::ForbiddenCursor,    // ImGuiMouseCursor_NotAllowed
};
#endif

static int ImGui_ImplQt_MouseButtonToImGuiButton(int button)
{
    switch (button) {
    case Qt::LeftButton: return ImGuiMouseButton_Left;
    case Qt::RightButton: return ImGuiMouseButton_Right;
    case Qt::MiddleButton: return ImGuiMouseButton_Middle;
    default: break;
    }
    return ImGuiMouseButton_COUNT;
}

static ImGuiKey ImGui_ImplQt_KeyToImGuiKey(int keycode)
{
    switch (keycode) {
    case Qt::Key_Tab: return ImGuiKey_Tab;
    case Qt::Key_Left: return ImGuiKey_LeftArrow;
    case Qt::Key_Right: return ImGuiKey_RightArrow;
    case Qt::Key_Up: return ImGuiKey_UpArrow;
    case Qt::Key_Down: return ImGuiKey_DownArrow;
    case Qt::Key_PageUp: return ImGuiKey_PageUp;
    case Qt::Key_PageDown: return ImGuiKey_PageDown;
    case Qt::Key_Home: return ImGuiKey_Home;
    case Qt::Key_End: return ImGuiKey_End;
    case Qt::Key_Insert: return ImGuiKey_Insert;
    case Qt::Key_Delete: return ImGuiKey_Delete;
    case Qt::Key_Backspace: return ImGuiKey_Backspace;
    case Qt::Key_Space: return ImGuiKey_Space;
    case Qt::Key_Enter: return ImGuiKey_Enter;
    case Qt::Key_Escape: return ImGuiKey_Escape;
    case Qt::Key_Control: return ImGuiKey_LeftCtrl;
    case Qt::Key_Shift: return ImGuiKey_LeftShift;
    case Qt::Key_Alt: return ImGuiKey_LeftAlt;
    case Qt::Key_Meta: return ImGuiKey_LeftSuper;
    // TODO: for right side keys we should use native scan codes
    case Qt::Key_Menu: return ImGuiKey_Menu;
    case Qt::Key_0: return ImGuiKey_0;
    case Qt::Key_1: return ImGuiKey_1;
    case Qt::Key_2: return ImGuiKey_2;
    case Qt::Key_3: return ImGuiKey_3;
    case Qt::Key_4: return ImGuiKey_4;
    case Qt::Key_5: return ImGuiKey_5;
    case Qt::Key_6: return ImGuiKey_6;
    case Qt::Key_7: return ImGuiKey_7;
    case Qt::Key_8: return ImGuiKey_8;
    case Qt::Key_9: return ImGuiKey_9;
    case Qt::Key_A: return ImGuiKey_A;
    case Qt::Key_B: return ImGuiKey_B;
    case Qt::Key_C: return ImGuiKey_C;
    case Qt::Key_D: return ImGuiKey_D;
    case Qt::Key_E: return ImGuiKey_E;
    case Qt::Key_F: return ImGuiKey_F;
    case Qt::Key_G: return ImGuiKey_G;
    case Qt::Key_H: return ImGuiKey_H;
    case Qt::Key_I: return ImGuiKey_I;
    case Qt::Key_J: return ImGuiKey_J;
    case Qt::Key_K: return ImGuiKey_K;
    case Qt::Key_L: return ImGuiKey_L;
    case Qt::Key_M: return ImGuiKey_M;
    case Qt::Key_N: return ImGuiKey_N;
    case Qt::Key_O: return ImGuiKey_O;
    case Qt::Key_P: return ImGuiKey_P;
    case Qt::Key_Q: return ImGuiKey_Q;
    case Qt::Key_R: return ImGuiKey_R;
    case Qt::Key_S: return ImGuiKey_S;
    case Qt::Key_T: return ImGuiKey_T;
    case Qt::Key_U: return ImGuiKey_U;
    case Qt::Key_V: return ImGuiKey_V;
    case Qt::Key_W: return ImGuiKey_W;
    case Qt::Key_X: return ImGuiKey_X;
    case Qt::Key_Y: return ImGuiKey_Y;
    case Qt::Key_Z: return ImGuiKey_Z;
    case Qt::Key_F1: return ImGuiKey_F1;
    case Qt::Key_F2: return ImGuiKey_F2;
    case Qt::Key_F3: return ImGuiKey_F3;
    case Qt::Key_F4: return ImGuiKey_F4;
    case Qt::Key_F5: return ImGuiKey_F5;
    case Qt::Key_F6: return ImGuiKey_F6;
    case Qt::Key_F7: return ImGuiKey_F7;
    case Qt::Key_F8: return ImGuiKey_F8;
    case Qt::Key_F9: return ImGuiKey_F9;
    case Qt::Key_F10: return ImGuiKey_F10;
    case Qt::Key_F11: return ImGuiKey_F11;
    case Qt::Key_F12: return ImGuiKey_F12;
    case Qt::Key_F13: return ImGuiKey_F13;
    case Qt::Key_F14: return ImGuiKey_F14;
    case Qt::Key_F15: return ImGuiKey_F15;
    case Qt::Key_F16: return ImGuiKey_F16;
    case Qt::Key_F17: return ImGuiKey_F17;
    case Qt::Key_F18: return ImGuiKey_F18;
    case Qt::Key_F19: return ImGuiKey_F19;
    case Qt::Key_F20: return ImGuiKey_F20;
    case Qt::Key_F21: return ImGuiKey_F21;
    case Qt::Key_F22: return ImGuiKey_F22;
    case Qt::Key_F23: return ImGuiKey_F23;
    case Qt::Key_F24: return ImGuiKey_F24;
    case Qt::Key_Apostrophe: return ImGuiKey_Apostrophe;
    case Qt::Key_Comma: return ImGuiKey_Comma;
    case Qt::Key_Minus: return ImGuiKey_Minus;
    case Qt::Key_Period: return ImGuiKey_Period;
    case Qt::Key_Slash: return ImGuiKey_Slash;
    case Qt::Key_Semicolon: return ImGuiKey_Semicolon;
    case Qt::Key_Equal: return ImGuiKey_Equal;
    case Qt::Key_BracketLeft: return ImGuiKey_LeftBracket;
    case Qt::Key_Backslash: return ImGuiKey_Backslash;
    case Qt::Key_BracketRight: return ImGuiKey_RightBracket;
    case Qt::Key_QuoteLeft: return ImGuiKey_GraveAccent;
    case Qt::Key_CapsLock: return ImGuiKey_CapsLock;
    case Qt::Key_ScrollLock: return ImGuiKey_ScrollLock;
    case Qt::Key_NumLock: return ImGuiKey_NumLock;
    case Qt::Key_Print: return ImGuiKey_PrintScreen;
    case Qt::Key_Pause: return ImGuiKey_Pause;
    // Keypad keys not supported yet
    case Qt::Key_Back: return ImGuiKey_AppBack;
    case Qt::Key_Forward: return ImGuiKey_AppForward;
    // Gamepad keys not supported yet
    default: break;
    }
    return ImGuiKey_None;
}

static void ImGui_ImplQt_UpdateKeyboardModifiers(Qt::KeyboardModifiers mods)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(ImGuiKey_ModCtrl, mods.testFlag(Qt::ControlModifier));
    io.AddKeyEvent(ImGuiKey_ModShift, mods.testFlag(Qt::ShiftModifier));
    io.AddKeyEvent(ImGuiKey_ModAlt, mods.testFlag(Qt::AltModifier));
    io.AddKeyEvent(ImGuiKey_ModSuper, mods.testFlag(Qt::MetaModifier));
}

static void ImGui_ImplQt_UpdateFocusEvent(bool focused)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddFocusEvent(focused);
}

static void ImGui_ImplQt_UpdateMouseData()
{
    ImGui_ImplQt_Data* bd = ImGui_ImplQt_GetBackendData();
    ImGuiIO&           io = ImGui::GetIO();

    // (those braces are here to reduce diff with multi-viewports support in
    // 'docking' branch)
    {
        const auto* widget     = bd->Widget;
        const bool  is_focused = widget->hasFocus();

        if (is_focused) {
            // (Optional) Set OS mouse position from Dear ImGui if requested
            // (rarely used, only when io.ConfigNavMoveSetMousePos is enabled by
            // user)
            if (io.WantSetMousePos)
                QCursor::setPos(widget->mapToGlobal(
                    QPoint((int) io.MousePos.x, (int) io.MousePos.y)));
        }
    }
}

static void ImGui_ImplQt_UpdateMouseCursor()
{
#ifndef QT_NO_CURSOR
    ImGuiIO&           io = ImGui::GetIO();
    ImGui_ImplQt_Data* bd = ImGui_ImplQt_GetBackendData();

    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    // (those braces are here to reduce diff with multi-viewports support in
    // 'docking' branch)
    {
        auto* widget = bd->Widget;
        if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor) {
            // hide mouse cursor if imgui is drawing it or if it wants no cursor
            widget->setCursor(Qt::BlankCursor);
        }
        else {
            // show requested mouse cursor
            widget->setCursor(QtMouseCursors[imgui_cursor]);
        }
    }
#endif
}

class ImGuiQtEventFilter : public QObject
{
    using QObject::QObject;

protected:
    bool eventFilter(QObject* obj, QEvent* event) override
    {
        static const bool BLOCK_EVENTS = false;

        ImGui_ImplQt_Data* bd = ImGui_ImplQt_GetBackendData();
        if (bd == nullptr)
            return false;
        if (obj == bd->Widget) {
            ImGuiIO& io = ImGui::GetIO();
            switch (event->type()) {
            case QEvent::KeyPress:
            case QEvent::KeyRelease: {
                QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
                const bool press    = (event->type() == QEvent::KeyPress);
                ImGui_ImplQt_UpdateKeyboardModifiers(keyEvent->modifiers());
                io.AddKeyEvent(
                    ImGui_ImplQt_KeyToImGuiKey(keyEvent->key()), press);
                if (press && !keyEvent->text().isEmpty()) {
                    io.AddInputCharactersUTF8(
                        keyEvent->text().toUtf8().constData());
                }
                return BLOCK_EVENTS;
            }
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonRelease: {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                const auto   button     = mouseEvent->button();
                const bool press = (event->type() == QEvent::MouseButtonPress);
                io.AddMouseButtonEvent(
                    ImGui_ImplQt_MouseButtonToImGuiButton(button), press);
                return BLOCK_EVENTS;
            }
            case QEvent::MouseMove: {
                QMouseEvent*  mouseEvent = static_cast<QMouseEvent*>(event);
                const QPointF pos        = mouseEvent->position();
                io.AddMousePosEvent(pos.x(), pos.y());
                return BLOCK_EVENTS;
            }
            case QEvent::Wheel: {
                QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
                QPointF      wheel(
                    wheelEvent->angleDelta().x() / 120.0f,
                    wheelEvent->angleDelta().y() / 120.0f);
                io.AddMouseWheelEvent(wheel.x(), wheel.y());
                return BLOCK_EVENTS;
            }
            case QEvent::FocusIn:
            case QEvent::FocusOut: {
                ImGui_ImplQt_UpdateFocusEvent(event->type() == QEvent::FocusIn);
                return BLOCK_EVENTS;
            }
            case QEvent::Enter:
            case QEvent::Leave: {
                ImGui_ImplQt_UpdateMouseData();
                return BLOCK_EVENTS;
            }
            default: break;
            }
        }
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
};

QByteArray gClipboardText;

bool ImGui_ImplQt_Init(QWidget* widget)
{
    // check if Qt Application is available
    if (QCoreApplication::instance() == nullptr)
        return false;

    ImGuiIO& io = ImGui::GetIO();

// Setup backend capabilities flags
#ifndef QT_NO_CURSOR
    io.BackendFlags |=
        ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor()
                                           // values (optional)
    io.BackendFlags |=
        ImGuiBackendFlags_HasSetMousePos; // We can honor io.WantSetMousePos
                                          // requests (optional, rarely used)
#endif
    io.BackendPlatformName = "imgui_impl_qt";

    // set clipboard functions
    ImGuiPlatformIO& platform_io            = ImGui::GetPlatformIO();
    platform_io.Platform_SetClipboardTextFn = [](ImGuiContext*,
                                                 const char* text) {
        auto* clipboard = QGuiApplication::clipboard();
        if (clipboard != nullptr)
            clipboard->setText(QString::fromUtf8(text));
    };
    platform_io.Platform_GetClipboardTextFn = [](ImGuiContext*) -> const char* {
        auto* clipboard = QGuiApplication::clipboard();
        if (clipboard != nullptr) {
            gClipboardText = clipboard->text().toUtf8();
            return (const char*) gClipboardText.constData();
        }
        return nullptr;
    };

    // Setup backend data
    ImGui_ImplQt_Data* bd = ImGui_ImplQt_GetBackendData();
    if (bd == nullptr) {
        bd                         = IM_NEW(ImGui_ImplQt_Data)();
        io.BackendPlatformUserData = (void*) bd;
    }
    // store widget and install event filter
    bd->Widget      = widget; // TODO: handle multiple widgets
    bd->EventFilter = new ImGuiQtEventFilter();
    widget->setMouseTracking(true);
    widget->installEventFilter(bd->EventFilter);

    // Set platform dependent data in viewport
    ImGuiViewport* main_viewport  = ImGui::GetMainViewport();
    main_viewport->PlatformHandle = (void*) bd->Widget;
    // PlatformHandleRaw probably not needed

    return true;
}

void ImGui_ImplQt_Shutdown()
{
    ImGui_ImplQt_Data* bd = ImGui_ImplQt_GetBackendData();
    if (bd == nullptr)
        return;
    if (bd->Widget != nullptr) {
        bd->Widget->removeEventFilter(bd->EventFilter);
        delete bd->EventFilter;
    }
    IM_DELETE(bd);
    ImGui::GetIO().BackendPlatformUserData = nullptr;
}

void ImGui_ImplQt_NewFrame()
{
    ImGuiIO&           io = ImGui::GetIO();
    ImGui_ImplQt_Data* bd = ImGui_ImplQt_GetBackendData();
    IM_ASSERT(
        bd != nullptr &&
        "Platform backend not initialized! Did you call ImGui_ImplQt_Init()?");

    // update display
    const auto size   = bd->Widget->size();
    io.DisplaySize    = ImVec2(float(size.width()), float(size.height()));
    const float scale = bd->Widget->devicePixelRatio();
    if (size.width() > 0 && size.height() > 0)
        io.DisplayFramebufferScale = ImVec2(scale, scale);

    // update time
    const auto now      = Clock::now();
    if (bd->LastTime == TimePoint(Clock::duration::zero()))
    {
        bd->LastTime = now - std::chrono::milliseconds(16);
    }
    const auto duration = now - bd->LastTime;
    io.DeltaTime        = std::chrono::duration<float>(duration).count();
    bd->LastTime        = now;

    // update mouse and cursors
    ImGui_ImplQt_UpdateMouseData();
    ImGui_ImplQt_UpdateMouseCursor();

    // TODO: gamepads?
}

#endif // #ifndef IMGUI_DISABLE
