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

#ifndef RENDERER_H
#define RENDERER_H

#include "concepts/canvas.h"
#include "concepts/window_manager.h"

namespace vcl {

template<
    template<typename> typename WindowManagerT,
    template<typename> typename CanvasT>
class Renderer :
        public WindowManagerT<Renderer<WindowManagerT, CanvasT>>,
        private CanvasT<Renderer<WindowManagerT, CanvasT>>
{
    friend class CanvasT<Renderer<WindowManagerT, CanvasT>>;
    friend class WindowManagerT<Renderer<WindowManagerT, CanvasT>>;

    using WindowManagerType = WindowManagerT<Renderer<WindowManagerT, CanvasT>>;
    using CanvasType = CanvasT<Renderer<WindowManagerT, CanvasT>>;

    static_assert(
        WindowManagerConcept<WindowManagerType>,
        "The second template parameter type of the Renderer class must be a "
        "class that satisfies the WindowManagerConcept.");

    static_assert(
        CanvasConcept<CanvasType>,
        "The first template parameter type of the Renderer class must be a "
        "class that satisfies the CanvasConcept.");

public:
    Renderer() :
            WindowManagerType(), CanvasType(
                                     (void*) WindowManagerType::winId(),
                                     WindowManagerType::width(),
                                     WindowManagerType::height(),
                                     WindowManagerType::displayId())
    {
    }

    Renderer(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768) :
            WindowManagerType(windowTitle, width, height),
            CanvasType(
                (void*) WindowManagerType::winId(),
                width,
                height,
                WindowManagerType::displayId())
    {
    }

private:
    /***** Member functions called by CanvasType *****/

    /**
     * @brief The CanvasType solecits a new frame, and it asks the window
     * manager to update the window.
     */
    void cnvUpdate()
    {
        // solecit new frame (by calling an update member function of the
        // WindowManagerType).
        WindowManagerType::update();
    }

    /**
     * @brief The CanvasType is ready to draw, and asks the Renderer to call
     * the draw function for every Drawer object.
     */
    void cnvDraw()
    {
        // TODO: call the draw function for every Drawer object.
    }

    void cnvDrawContent()
    {
        // TODO: call the drawContent function for every Drawer object.
    }

    /***** Member functions called by WindowManagerType *****/

    /**
     * @brief The WindowManagerType calls this member function when the window
     * is resized, telling the new width and height. The Renderer propagates
     * the resize event to the CanvasType and to each Drawer object.
     *
     * @param width
     * @param height
     */
    void wmResize(uint width, uint height)
    {
        CanvasType::resize(width, height);
        // call the resize member function of each Drawer object
    }
};

} // namespace vcl

#endif // RENDERER_H
