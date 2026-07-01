// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_BOX3_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_BOX3_H

#include <vclib/bgfx/primitives/lines.h>
#include <vclib/render/drawable/drawable_object.h>

#include <vclib/space/core.h>

namespace vcl {

class DrawableBox3 : public DrawableObject
{
    Box3f mBox;

    Lines mBoxLines;
    bool  mVisible = true;

public:
    DrawableBox3() = default;

    template<typename S>
    DrawableBox3(
        const Box3<S>& box,
        const Color&   color     = Color::Black,
        float          thickness = 1.0f) : mBox(box.template cast<float>())
    {
        mBoxLines.setGeneralColor(color);
        mBoxLines.setWidth(thickness);
        updateLines();
    }

    DrawableBox3(const DrawableBox3& other);

    DrawableBox3(DrawableBox3&& other) { swap(other); }

    ~DrawableBox3() = default;

    DrawableBox3& operator=(DrawableBox3 other);

    void swap(DrawableBox3& other);

    friend void swap(DrawableBox3& first, DrawableBox3& second)
    {
        first.swap(second);
    }

    template<typename S>
    void setBox(const Box3<S>& box)
    {
        mBox = box.template cast<float>();
        updateLines();
    }

    void setThickness(float thickness);

    void setColor(const Color& color);

    // DrawableObject implementation

    void draw(const DrawObjectSettings& settings) override;

    vcl::Box3d boundingBox() const override { return mBox.cast<double>(); }

    std::shared_ptr<DrawableObject> clone() const& override;

    std::shared_ptr<DrawableObject> clone() && override;

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }

private:
    void updateLines();
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_BOX3_H
