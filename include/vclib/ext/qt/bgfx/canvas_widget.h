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

#include <QVBoxLayout>
#include <QWidget>

#include <vclib/ext/bgfx/canvas.h>

#include "canvas_window.h"

namespace vcl::qbgf {

#ifndef __APPLE__

class CanvasWidget : public QWidget, public vcl::bgf::Canvas
{
    using Canvas = vcl::bgf::Canvas;

public:
    explicit CanvasWidget(QWidget* parent = nullptr);

    virtual ~CanvasWidget();

    virtual void draw() override;

    virtual void onResize(unsigned int w, unsigned int h);

    void update();

protected:
    bool event(QEvent* event) override;

    void paintEvent(QPaintEvent* event) override;

    void resizeEvent(QResizeEvent* event) override;

private:
    void paint();
};

#else  // __APPLE__
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
template<typename CanvasWindow = vcl::qbgf::CanvasWindow>
class CanvasWidget : public QWidget
{
public:
    explicit CanvasWidget(QWidget* parent = nullptr) : QWidget(parent)
    {
        setGeometry(100, 100, 1024, 768);

        // Set layout
        QVBoxLayout* layout = new QVBoxLayout;

        canvasWindow = new CanvasWindow();
        layout->addWidget(QWidget::createWindowContainer(canvasWindow, this));

        setLayout(layout);
    }

    ~CanvasWidget() {}

    bgfx::ViewId viewID() const { return canvasWindow->viewID(); }

    virtual void draw() { canvasWindow->draw(); }

    void update()
    {
        canvasWindow->update();
        QWidget::update();
    }

    void keyPressEvent(QKeyEvent* event) override
    {
        // sometimes, the viewer does not automatically get key events
        canvasWindow->keyPressEvent(event);
    }

    void keyReleaseEvent(QKeyEvent* event) override
    {
        // sometimes, the viewer does not automatically get key events
        canvasWindow->keyReleaseEvent(event);
    }

protected:
    CanvasWindow* canvasWindow = nullptr;
};
#endif // __APPLE__

} // namespace vcl::qbgf

#endif // VCL_EXT_QT_BGFX_CANVAS_WIDGET_H
