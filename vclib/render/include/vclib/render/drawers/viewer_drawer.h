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

#ifndef VCL_RENDER_DRAWERS_VIEWER_DRAWER_H
#define VCL_RENDER_DRAWERS_VIEWER_DRAWER_H

#include <vclib/render/config.h>

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/drawers/viewer_drawer.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include <vclib/opengl2/drawers/viewer_drawer.h>
#endif

namespace vcl {

#ifdef VCLIB_RENDER_BACKEND_BGFX
template<typename DerivedRenderer>
using ViewerDrawer = ViewerDrawerBGFX<DerivedRenderer>;
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
template<typename DerivedRenderer>
using ViewerDrawer = ViewerDrawerOpenGL2<DerivedRenderer>;
#endif

} // namespace vcl

#endif // VCL_RENDER_DRAWERS_VIEWER_DRAWER_H
