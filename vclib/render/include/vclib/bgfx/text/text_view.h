// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_TEXT_TEXT_VIEW_H
#define VCL_BGFX_TEXT_TEXT_VIEW_H

#include "../context.h"
#include "text_manager.h"

namespace vcl {

class TextView
{
    TextManager  mTextManager;
    bgfx::ViewId mView = BGFX_INVALID_VIEW;
    float        mTextViewMatrix[16];
    float        mTextProjMatrix[16];
    uint         mWidth  = 0;
    uint         mHeight = 0;

public:
    TextView();
    ~TextView();

    void init(uint width, uint height);

    void enableText(bool b = true);
    bool isTextEnabled() const;

    void setTextFont(VclFont::Enum font, uint fontSize);
    void setTextFont(const std::string& fontName, uint fontSize);

    void clearText();

    void appendStaticText(
        const vcl::Point2f& pos,
        const std::string&  text,
        const vcl::Color&   color = vcl::Color::Black);

    void appendTransientText(
        const vcl::Point2f& pos,
        const std::string&  text,
        const vcl::Color&   color = vcl::Color::Black);

    void frame(bgfx::FrameBufferHandle fbh);

    void resize(uint width, uint height);

private:
    void updateProjMatrix();
};

} // namespace vcl

#endif // VCL_BGFX_TEXT_TEXT_VIEW_H
