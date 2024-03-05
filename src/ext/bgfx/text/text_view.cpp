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

#include <vclib/ext/bgfx/text/text_view.h>

#include <vclib/ext/bgfx/context.h>

#include <bx/math.h>

namespace vcl::bgf {

TextView::TextView()
{
}

TextView::~TextView()
{
    if (isTextEnabled())
        Context::releaseViewId(view);
}

void TextView::enableText(bool b)
{
    if (isViewValid(view)) {
        if (!b) {
            Context::releaseViewId(view);
            view = BGFX_INVALID_HANDLE;
        }
    }
    else {
        if (b) {
            view = Context::requestViewId();
            updateProjMatrix();
            bgfx::setViewRect(view, 0, 0, w, h);
            bgfx::touch(view);
        }
    }
}

bool TextView::isTextEnabled() const
{
    return isViewValid(view);
}

void TextView::init(uint width, uint height)
{
    w = width;
    h = height;

    const bx::Vec3 at  = { 0.0f, 0.0f,  0.0f };
    const bx::Vec3 eye = { 0.0f, 0.0f, -1.0f };

    bx::mtxLookAt(textViewMatrix, eye, at);

    textManager.init();
    textManager.loadFont("assets/fonts/droidsans.ttf", "DroidSans");
    textManager.setCurrentFont("DroidSans", 20);

    textManager.appendStaticText({10, height - 50}, "Static Text");
}

void TextView::frame(bgfx::FrameBufferHandle fbh)
{
    static uint cnt = 0;
    if (isTextEnabled()) {
        bgfx::setViewFrameBuffer(view, fbh);
        bgfx::touch(view);

        bgfx::setViewTransform(view, textViewMatrix, textProjMatrix);

        std::string s = "Frame " + std::to_string(cnt++);
        textManager.appendTransientText({10, 10}, s + "\nTransient\nHello World\n");
        textManager.submit(view);
    }
}

void TextView::resize(uint width, uint height)
{
    w = width;
    h = height;

    if (isTextEnabled()) {
        updateProjMatrix();
        bgfx::setViewRect(view, 0, 0, w, h);
        bgfx::touch(view);
    }
}

void TextView::updateProjMatrix()
{
    const bgfx::Caps* caps = bgfx::getCaps();
    bx::mtxOrtho(
        textProjMatrix,
        0.0f,
        float(w),
        float(h),
        0.0f,
        0.0f,
        100.0f,
        0.0f,
        caps->homogeneousDepth);
}

} // namespace vcl::bgf
