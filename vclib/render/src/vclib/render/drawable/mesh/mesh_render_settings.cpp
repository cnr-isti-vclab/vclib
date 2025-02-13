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

bool MeshRenderSettings::setPointsWidth(float width)
{
    if (canPoints(MRI::Points::VISIBLE)) {
        mPointWidth = width;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointsUserColor(float r, float g, float b, float a)
{
    if (canPoints(MRI::Points::VISIBLE)) {
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

bool MeshRenderSettings::setPointsUserColor(const Color& c)
{
    if (canPoints(MRI::Points::VISIBLE)) {
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

bool MeshRenderSettings::setSurfaceUserColor(float r, float g, float b, float a)
{
    if (canSurface(MRI::Surface::VISIBLE)) {
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
    if (canSurface(MRI::Surface::VISIBLE)) {
        mSurfUserColor = c.abgr();
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
    if (canWireframe(MRI::Wireframe::VISIBLE)) {
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
    if (canWireframe(MRI::Wireframe::VISIBLE)) {
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
    if (canWireframe(MRI::Wireframe::VISIBLE)) {
        mWrfWidth = width;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesUserColor(float r, float g, float b, float a)
{
    if (canEdges(MRI::Edges::VISIBLE)) {
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
    if (canEdges(MRI::Edges::VISIBLE)) {
        mEdgesUserColor = c.abgr();
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesWidth(int width)
{
    if (canEdges(MRI::Edges::VISIBLE)) {
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

        setDefaultSurfaceSettingsFromCapability();
        setDefaultWireframeSettingsFromCapability();
        setDefaultPointSettingsFromCapability();
        setDefaultEdgeSettingsFromCapability();
    }
}

void MeshRenderSettings::setDefaultPointSettingsFromCapability()
{
    using enum MRI::Points;

    if (canPoints(VISIBLE)) {
        if (!canSurface(MRI::Surface::VISIBLE))
            setPoints(VISIBLE, true);
        setPoints(SHADING_NONE);
        if (canPoints(SHADING_VERT)) {
            setPoints(SHADING_VERT);
        }
        if (canPoints(COLOR_VERTEX)) {
            setPoints(COLOR_VERTEX);
        }
        else {
            setPoints(COLOR_USER);
        }
    }
}

void MeshRenderSettings::setDefaultSurfaceSettingsFromCapability()
{
    using enum MRI::Surface;

    if (canSurface(VISIBLE)) {
        setSurface(VISIBLE, true);
        // shading
        if (canSurface(SHADING_SMOOTH)) {
            setSurface(SHADING_SMOOTH);
        }
        else if (canSurface(SHADING_FLAT)) {
            setSurface(SHADING_FLAT);
        }
        else {
            setSurface(SHADING_NONE);
        }
        // color
        if (canSurface(COLOR_VERTEX)) {
            setSurface(COLOR_VERTEX);
        }
        else if (canSurface(COLOR_FACE)) {
            setSurface(COLOR_FACE);
        }
        else if (canSurface(COLOR_WEDGE_TEX)) {
            setSurface(COLOR_WEDGE_TEX);
        }
        else if (canSurface(COLOR_VERTEX_TEX)) {
            setSurface(COLOR_VERTEX_TEX);
        }
        else if (canSurface(COLOR_MESH)) {
            setSurface(COLOR_MESH);
        }
        else {
            setSurface(COLOR_USER);
        }
    }
}

void MeshRenderSettings::setDefaultWireframeSettingsFromCapability()
{
    using enum MRI::Wireframe;

    if (canWireframe(VISIBLE)) {
        if (canWireframe(SHADING_VERT)) {
            setWireframe(SHADING_VERT);
        }
        else {
            setWireframe(SHADING_NONE);
        }
        // wireframe color (defaults to user defined)
        setWireframe(COLOR_USER);
    }
}

void MeshRenderSettings::setDefaultEdgeSettingsFromCapability()
{
    using enum MRI::Edges;

    if (canEdges(VISIBLE)) {
        setEdges(VISIBLE, true);

        if (canEdges(SHADING_SMOOTH)) {
            setEdges(SHADING_SMOOTH);
        }
        else if (canEdges(SHADING_FLAT)) {
            setEdges(SHADING_FLAT);
        }
        else {
            setEdges(SHADING_NONE);
        }

        if (canEdges(COLOR_VERTEX)) {
            setEdges(COLOR_VERTEX);
        }
        else if (canEdges(COLOR_EDGE)) {
            setEdges(COLOR_EDGE);
        }
        else {
            setEdges(COLOR_USER);
        }
    }
}

} // namespace vcl
