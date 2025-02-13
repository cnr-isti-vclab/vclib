/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include <vclib/render/drawable/mesh/mesh_render_settings.h>

namespace vcl {

Color MeshRenderSettings::pointUserColor() const
{
    vcl::Color c;
    c.setRedF(mPointUserColor[0]);
    c.setGreenF(mPointUserColor[1]);
    c.setBlueF(mPointUserColor[2]);
    c.setAlphaF(mPointUserColor[3]);
    return c;
}

Color MeshRenderSettings::surfaceUserColor() const
{
    vcl::Color c;
    c.setAbgr(mSurfUserColor);
    return c;
}

Color MeshRenderSettings::wireframeUserColor() const
{
    vcl::Color c;
    c.setRedF(mWrfUserColor[0]);
    c.setGreenF(mWrfUserColor[1]);
    c.setBlueF(mWrfUserColor[2]);
    c.setAlphaF(mWrfUserColor[3]);
    return c;
}

Color MeshRenderSettings::edgesUserColor() const
{
    vcl::Color c;
    c.setAbgr(mEdgesUserColor);
    return c;
}

bool MeshRenderSettings::setVisibility(bool b)
{
    if (canBeVisible()) {
        mDrawMode.visible() = b;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPoint(MeshRenderInfo::Points p, bool b)
{
    if (canPoint(p)) { // if the capability allows it
        // get the range of the mutual exclusive settings for p
        auto rng = MRI::pointsExclusiveRange(p);
        // if there are no mutual exclusive settings
        if (rng.first == rng.second) {
            // the setting could be true or false
            // e.g. VISIBLE
            mDrawMode.points()[rng.first] = b;
        }
        else {
            // only one setting in the range can be true
            // e.g. the range SHAPE_*
            for (auto i = rng.first; i <= rng.second; ++i) {
                mDrawMode.points()[i] = toUnderlying(p) == i;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointWidth(float width)
{
    if (canPointBeVisible()) {
        mPointWidth = width;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointUserColor(float r, float g, float b, float a)
{
    if (canPointBeVisible()) {
        mPointUserColor[0] = r;
        mPointUserColor[1] = g;
        mPointUserColor[2] = b;
        mPointUserColor[3] = a;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointUserColor(const Color& c)
{
    if (canPointBeVisible()) {
        mPointUserColor[0] = c.redF();
        mPointUserColor[1] = c.greenF();
        mPointUserColor[2] = c.blueF();
        mPointUserColor[3] = c.alphaF();
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setSurface(MeshRenderInfo::Surface s, bool b)
{
    if (canSurface(s)) { // if the capability allows it
        // get the range of the mutual exclusive settings for s
        auto rng = MRI::surfaceExclusiveRange(s);
        // if there are no mutual exclusive settings
        if (rng.first == rng.second) {
            // the setting could be true or false
            // e.g. VISIBLE
            mDrawMode.surface()[rng.first] = b;
        }
        else {
            // only one setting in the range can be true
            // e.g. the range COLOR_*
            for (auto i = rng.first; i <= rng.second; ++i) {
                mDrawMode.surface()[i] = toUnderlying(s) == i;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setSurfaceUserColor(float r, float g, float b, float a)
{
    if (canSurfaceBeVisible()) {
        vcl::Color c;
        c.setRedF(r);
        c.setGreenF(g);
        c.setBlueF(b);
        c.setAlphaF(a);
        mSurfUserColor = c.abgr();
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setSurfaceUserColor(const Color& c)
{
    if (canSurfaceBeVisible()) {
        mSurfUserColor = c.abgr();
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframe(MeshRenderInfo::Wireframe w, bool b)
{
    if (canWireframe(w)) { // if the capability allows it
        // get the range of the mutual exclusive settings for w
        auto rng = MRI::wireframeExclusiveRange(w);
        // if there are no mutual exclusive settings
        if (rng.first == rng.second) {
            // the setting could be true or false
            // e.g. VISIBLE
            mDrawMode.wireframe()[rng.first] = b;
        }
        else {
            // only one setting in the range can be true
            // e.g. the range COLOR_*
            for (auto i = rng.first; i <= rng.second; ++i) {
                mDrawMode.wireframe()[i] = toUnderlying(w) == i;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeUserColor(
    float r,
    float g,
    float b,
    float a)
{
    if (canSurfaceBeVisible()) {
        mWrfUserColor[0] = r;
        mWrfUserColor[1] = g;
        mWrfUserColor[2] = b;
        mWrfUserColor[3] = a;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeUserColor(const Color& c)
{
    if (canSurfaceBeVisible()) {
        mWrfUserColor[0] = c.redF();
        mWrfUserColor[1] = c.greenF();
        mWrfUserColor[2] = c.blueF();
        mWrfUserColor[3] = c.alphaF();
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeWidth(int width)
{
    if (canSurfaceBeVisible()) {
        mWrfWidth = width;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdges(MeshRenderInfo::Edges e, bool b)
{
    if (canEdges(e)) { // if the capability allows it
        // get the range of the mutual exclusive settings for e
        auto rng = MRI::edgesExclusiveRange(e);
        // if there are no mutual exclusive settings
        if (rng.first == rng.second) {
            // the setting could be true or false
            // e.g. VISIBLE
            mDrawMode.edges()[rng.first] = b;
        }
        else {
            // only one setting in the range can be true
            // e.g. the range COLOR_*
            for (auto i = rng.first; i <= rng.second; ++i) {
                mDrawMode.edges()[i] = toUnderlying(e) == i;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesUserColor(float r, float g, float b, float a)
{
    if (canEdgesBeVisible()) {
        vcl::Color c;
        c.setRedF(r);
        c.setGreenF(g);
        c.setBlueF(b);
        c.setAlphaF(a);
        mEdgesUserColor = c.abgr();
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesUserColor(const Color& c)
{
    if (canEdgesBeVisible()) {
        mEdgesUserColor = c.abgr();
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesWidth(int width)
{
    if (canEdgesBeVisible()) {
        mEdgesWidth = width;
        return true;
    }
    else {
        return false;
    }
}

void MeshRenderSettings::setDefaultSettingsFromCapability()
{
    mDrawMode.reset();

    if (canBeVisible()) {
        setVisibility(true);
        if (canSurfaceBeVisible()) {
            setSurfaceVisibility(true);
            // shading
            if (canSurfaceShadingBeSmooth()) {
                setSurfaceShadingSmooth();
            }
            else if (canSurfaceShadingBeFlat()) {
                setSurfaceShadingFlat();
            }
            else {
                setSurfaceShadingNone();
            }
            // color
            if (canSurfaceColorBePerVertex()) {
                setSurfaceColorPerVertex();
            }
            else if (canSurfaceColorBePerFace()) {
                setSurfaceColorPerFace();
            }
            else if (canSurfaceColorBePerWedgeTexcoords()) {
                setSurfaceColorPerWedgeTexcoords();
            }
            else if (canSurfaceColorBePerVertexTexcoords()) {
                setSurfaceColorPerVertexTexcoords();
            }
            else {
                setSurfaceColorUserDefined();
            }
            // wireframe shading
            if (canWireframeShadingBePerVertex()) {
                setWireframeShadingPerVertex();
            }
            else {
                setWireframeShadingNone();
            }
            // wireframe color (defaults to user defined)
            setWireframeColorUserDefined();
        }

        if (canPointBeVisible()) {
            if (!canSurfaceBeVisible())
                setPointVisibility(true);
            setPointShadingNone();
            if (canPointShadingBePerVertex()) {
                setPointShadingPerVertex();
            }
            if (canPointColorBePerVertex()) {
                setPointColorPerVertex();
            }
            else {
                setPointColorUserDefined();
            }
        }

        if (canEdgesBeVisible()) {
            setEdgesVisibility(true);

            if (canEdgesShadingBeSmooth()) {
                setEdgesShadingSmooth();
            }
            else if (canEdgesShadingBeFlat()) {
                setEdgesShadingFlat();
            }
            else {
                setEdgesShadingNone();
            }

            if (canEdgesColorBePerVertex()) {
                setEdgesColorPerVertex();
            }
            else if (canEdgesColorBePerEdge()) {
                setEdgesColorPerEdge();
            }
            else {
                setEdgesColorUserDefined();
            }
        }
    }
}

} // namespace vcl
