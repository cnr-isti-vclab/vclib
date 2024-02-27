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

#include <vclib/ext/bgfx/canvas.h>

#include <vclib/gui/native_window_handle.h>

#include <vector>
#include <fstream>

namespace vcl::bgf {

Canvas::Canvas()
{
}

Canvas::Canvas(void* winID, uint width, uint height)
{
    init(winID, width, height);
}

Canvas::~Canvas()
{
    if (bgfx::isValid(fbh))
        bgfx::destroy(fbh);
    Context::releaseViewId(view);

    // text

    m_fontManager->destroyFont(m_visitor10);
    m_fontManager->destroyTtf(m_visitorTtf);

    m_textBufferManager->destroyTextBuffer(m_transientText);

    delete m_fontManager;
    delete m_textBufferManager;
}

void Canvas::init(void* winId, uint width, uint height)
{
    this->winID = winId;

    view = Context::requestViewId();

    fbh = bgfx::createFrameBuffer(winId, width, height);

    bgfx::setViewFrameBuffer(view, fbh);
    bgfx::setViewClear(
        view, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 1.0f, 0);
    bgfx::setViewRect(view, 0, 0, width, height);
    bgfx::touch(view);

    // text
    m_fontManager = new FontManager(512);
    m_textBufferManager = new TextBufferManager(m_fontManager);

    m_visitorTtf = loadTtf(*m_fontManager, "assets/fonts/droidsans.ttf");
    m_visitor10 = m_fontManager->createFontByPixelSize(m_visitorTtf, 0, 10);

    m_fontManager->preloadGlyph(m_visitor10, L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ. \n");

    m_transientText = m_textBufferManager->createTextBuffer(
        FONT_TYPE_ALPHA, BufferType::Transient);

    // end text
}

void Canvas::screenShot(const std::string& filename, uint width, uint height)
{
    if (width == 0 || height == 0) {
        draw();
        bgfx::requestScreenShot(fbh, filename.c_str());
        bgfx::frame();
    }
    else {
        void* d;
        void* w = vcl::createWindow("", width, height, d, true);

        // setup view and frame buffer
        bgfx::ViewId            v   = Context::requestViewId();
        bgfx::FrameBufferHandle fbh = bgfx::createFrameBuffer(w, width, height);
        bgfx::setViewFrameBuffer(v, fbh);
        bgfx::setViewClear(
            v, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xffffffff, 1.0f, 0);
        bgfx::setViewRect(v, 0, 0, width, height);
        bgfx::touch(v);

        // replace the current view with the new one
        bgfx::ViewId tmpView = view;
        view                 = v;
        draw();
        bgfx::requestScreenShot(fbh, filename.c_str());
        bgfx::frame();

        // restore the previous view and release the resources
        view = tmpView;
        bgfx::destroy(fbh);
        Context::releaseViewId(v);
        vcl::closeWindow(w, d);
    }
}

void Canvas::frame()
{
    bgfx::setViewFrameBuffer(view, fbh);
    bgfx::touch(view);
    draw();

    m_textBufferManager->clearTextBuffer(m_transientText);

    // text will be black
    m_textBufferManager->setTextColor(m_transientText, 0x000000ff);

    m_textBufferManager->setPenPosition(m_transientText, 10.f, 10.0f);
    m_textBufferManager->appendText(m_transientText, m_visitor10, "Transient\n");
    m_textBufferManager->appendText(m_transientText, m_visitor10, "text buffer\n");

    m_textBufferManager->submitTextBuffer(m_transientText, view);

    bgfx::frame();
}

void Canvas::resize(uint width, uint height)
{
    if (bgfx::isValid(fbh))
        bgfx::destroy(fbh);

    fbh = bgfx::createFrameBuffer(winID, width, height);
    bgfx::setViewFrameBuffer(view, fbh);
    bgfx::setViewRect(view, 0, 0, width, height);
    bgfx::touch(view);
}


TrueTypeHandle Canvas::loadTtf(FontManager& fontManager, const char* filePath)
{
    std::vector<uint8_t> data;

    // create ifstream from filePath
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    // get the size of the file
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the file and place it into the vector data
    data.resize(size);
    file.read((char*)data.data(), size);

    TrueTypeHandle handle = fontManager.createTtf(data.data(), size);
    return handle;
}

} // namespace vcl::bgf
