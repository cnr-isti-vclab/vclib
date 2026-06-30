// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_DIRECTIONAL_LIGHT_BGFX_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_DIRECTIONAL_LIGHT_BGFX_H

#include <vclib/render/drawable/drawable_object.h>
#include <vclib/render/viewer/lights/directional_light.h>
#include <vclib/space/core/matrix.h>

#include <vclib/bgfx/buffers/vertex_buffer.h>
#include <vclib/bgfx/context.h>

#include <bgfx/bgfx.h>

namespace vcl {

/**
 * @brief The DrawableDirectionalLightBGFX class is a drawable object that
 * renders a directional light using the BGFX backend.
 *
 * It draws a set of lines representing the light rays.
 */
class DrawableDirectionalLightBGFX : public DrawableObject
{
    bool mVisible = false;

    Matrix44f mTransform = vcl::Matrix44f::Identity();

    std::vector<float> mVertices; // vertices of the drawn lines
    vcl::Color         mColor = vcl::Color::Yellow; // color of the lines

    VertexBuffer mVertexPosBuffer;

public:
    DrawableDirectionalLightBGFX();

    DrawableDirectionalLightBGFX(const DrawableDirectionalLightBGFX& other);

    DrawableDirectionalLightBGFX(DrawableDirectionalLightBGFX&& other);

    ~DrawableDirectionalLightBGFX();

    DrawableDirectionalLightBGFX& operator=(DrawableDirectionalLightBGFX other);

    void swap(DrawableDirectionalLightBGFX& other);

    friend void swap(
        DrawableDirectionalLightBGFX& a,
        DrawableDirectionalLightBGFX& b)
    {
        a.swap(b);
    }

    void updateRotation(const vcl::Matrix44f& rot);

    const vcl::Color& linesColor() const { return mColor; }

    void setLinesColor(const vcl::Color& c);

    // DrawableObject interface

    void draw(const DrawObjectSettings& settings) override;

    Box3d boundingBox() const override;

    std::shared_ptr<DrawableObject> clone() const& override;

    std::shared_ptr<DrawableObject> clone() && override;

    bool isVisible() const override { return mVisible; }

    void setVisibility(bool vis) override { mVisible = vis; }

private:
    void createVertexBuffer();
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_DIRECTIONAL_LIGHT_BGFX_H
