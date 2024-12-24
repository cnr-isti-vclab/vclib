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

#ifndef TEXT_DRAWER_H
#define TEXT_DRAWER_H

#include <vclib/bgfx/text/text_view.h>
#include <vclib/render/drawers/plain_drawer.h>

namespace vcl {

template<typename DerivedRenderer>
class TextDrawer : public PlainDrawer<DerivedRenderer>
{
    using DRT = DerivedRenderer;

    TextView mTextView;

public:
    TextDrawer() : TextDrawer(1,1) {}

    TextDrawer(uint width, uint height)
    {
        // TODO: this should be done in the member function onInit()
        // however, when using glfw the onInit is called just before the
        // show method is called, making impossible to add text before
        // the window is shown
        mTextView.init(width, height);
    }

    void onResize(uint width, uint height) override final
    {
        mTextView.resize(width, height);
    }

    void onDraw(uint viewId) override final
    {
        auto fbh = DRT::D::canvasFrameBuffer(derived());
        mTextView.frame(fbh);
    }

    // text
    void enableText(bool b = true) { mTextView.enableText(b); }

    bool isTextEnabled() const { return mTextView.isTextEnabled(); }

    void setTextFont(VclFont::Enum font, uint fontSize)
    {
        mTextView.setTextFont(font, fontSize);
    }

    void setTextFont(const std::string& fontName, uint fontSize)
    {
        mTextView.setTextFont(fontName, fontSize);
    }

    void clearText()
    {
        mTextView.clearText();
    }

    void appendStaticText(
        const Point2f&     pos,
        const std::string& text,
        const Color&       color = Color::Black)
    {
        mTextView.appendStaticText(pos, text, color);
    }

    void appendTransientText(
        const Point2f&     pos,
        const std::string& text,
        const Color&       color = Color::Black)
    {
        mTextView.appendTransientText(pos, text, color);
    }

private:
    auto* derived() { return static_cast<DRT*>(this); }

    const auto* derived() const { return static_cast<const DRT*>(this); }

};

} // namespace vcl

#endif // TEXT_DRAWER_H
