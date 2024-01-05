/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_QT_BGFX_CANVAS_WIDGET_H
#define VCL_EXT_QT_BGFX_CANVAS_WIDGET_H

#include <bgfx/bgfx.h>

#include <QVBoxLayout>
#include <QWidget>

namespace vcl::qbgf {

/**
 * @brief The CanvasWidget class allows to use bgfx in a QWidget.
 *
 * Normally, this class would be a native QWidget that allows to render using
 * the bgfx library. However, QWidgets and bgfx, at the time of writing (2024),
 * are not compatible on all platforms (MacOS).
 *
 * This class is a workaround that uses a CanvasWindow to render the bgfx
 * content. The CanvasWindow is then embedded in a QWidget.
 *
 * To create a QWidget that embeds a class that draws using bgfx:
 * - Create a class that inherits from CanvasWindow (e.g. MyViewerWindow);
 * - Use this class by instantiating a CanvasWidget with your CanvasWindow
 * class (e.g. CanvasWidget<MyViewerWindow>).
 *
 * @tparam CanvasWindow the type of the CanvasWindow to use.
 */
template<typename CanvasWindow>
class CanvasWidget : public QWidget
{
public:
    explicit CanvasWidget(QWidget* parent = nullptr) : QWidget(parent)
    {
        setGeometry(100, 100, 1024, 768);

        // Set layout
        QVBoxLayout* layout = new QVBoxLayout;

#ifdef Q_OS_LINUX
        const auto rt = bgfx::RendererType::Count;
        // const auto rt = bgfx::RendererType::OpenGL;
#else
        const auto rt = bgfx::RendererType::Count; // default
#endif

        canvasWindow = new CanvasWindow(rt);
        layout->addWidget(QWidget::createWindowContainer(canvasWindow, this));

        setLayout(layout);
    }

    ~CanvasWidget() {}

    CanvasWindow* window() { return canvasWindow; }

    const CanvasWindow* window() const { return canvasWindow; }

    virtual void draw() { canvasWindow->draw(); }

    void keyPressEvent(QKeyEvent* event) override
    {
        // sometimes, the viewer does not automatically gets key events
        canvasWindow->keyPressEvent(event);
    }

    void keyReleaseEvent(QKeyEvent* event) override
    {
        // sometimes, the viewer does not automatically gets key events
        canvasWindow->keyReleaseEvent(event);
    }

protected:
    CanvasWindow* canvasWindow = nullptr;
};

} // namespace vcl::qbgf

#endif // VCL_EXT_QT_BGFX_CANVAS_WIDGET_H
