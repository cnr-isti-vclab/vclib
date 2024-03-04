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

#ifndef VCL_EXT_BGFX_TEXT_TEXT_VIEW_H
#define VCL_EXT_BGFX_TEXT_TEXT_VIEW_H

#include "text_manager.h"

namespace vcl::bgf {

class TextView
{
    TextManager textManager;
    bgfx::ViewId view;
    float textViewMatrix[16];
    float textProjMatrix[16];

public:
    TextView();
    ~TextView();

protected:
    void init(uint width, uint height);

    void frame(bgfx::FrameBufferHandle fbh);

    void resize(uint width, uint height);
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_TEXT_TEXT_VIEW_H
