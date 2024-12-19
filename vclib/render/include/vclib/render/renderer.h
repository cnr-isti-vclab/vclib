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

namespace vcl {

namespace detail {

template<template<typename> typename CanvasT>
class Renderer : private CanvasT<Renderer<CanvasT>>
{
    using CanvasType = CanvasT<Renderer<CanvasT>>;

    static_assert(
        CanvasConcept<CanvasType>,
        "The first template parameter type of the Renderer class must be a "
        "class that satisfies the CanvasConcept.");

public:
    /*
     * All the following member functions are public because must be called
     * from the Base classes (e.g. the CanvasType).
     * These member functions will be then hidden from the user by adding the
     * private inheritance from this class outside the detail namespace.
     */

    void update()
    {
        // TODO: Implement update
        // solecit new frame (by calling an update member function of the
        // WindowManagerType).
    }

    void draw()
    {
        // TODO: call the draw function for every Drawer object.
    }

    void drawContent()
    {
        // TODO: call the drawContent function for every Drawer object.
    }
};

} // namespace detail

template<template<typename> typename CanvasT>
class Renderer : private detail::Renderer<CanvasT>
{
    using Base = detail::Renderer<CanvasT>;
};

} // namespace vcl

#endif // RENDERER_H
