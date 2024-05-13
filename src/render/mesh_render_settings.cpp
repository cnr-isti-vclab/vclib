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

#include <vclib/render/mesh_render_settings.h>

namespace vcl {

Color MeshRenderSettings::pointCloudUserColor() const
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
        if (b)
            mDrawMode0 |= VCL_MRS_DRAW_MESH;
        else
            mDrawMode0 &= ~VCL_MRS_DRAW_MESH;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudVisibility(bool b)
{
    if (canPointCloudBeVisible()) {
        if (b)
            mDrawMode0 |= VCL_MRS_DRAW_POINTS;
        else
            mDrawMode0 &= ~VCL_MRS_DRAW_POINTS;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudShadingNone()
{
    if (canPointCloudBeVisible()) {
        mDrawMode0 |= VCL_MRS_POINTS_SHADING_NONE;
        mDrawMode0 &= ~VCL_MRS_POINTS_SHADING_VERT;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudShadingPerVertex()
{
    if (canPointCloudBeVisible()) {
        mDrawMode0 &= ~VCL_MRS_POINTS_SHADING_NONE;
        mDrawMode0 |= VCL_MRS_POINTS_SHADING_VERT;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudColorPerVertex()
{
    if (canSurfaceColorBePerVertex()) {
        mDrawMode0 |= VCL_MRS_POINTS_COLOR_VERTEX;
        mDrawMode0 &= ~VCL_MRS_POINTS_COLOR_MESH;
        mDrawMode0 &= ~VCL_MRS_POINTS_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudColorPerMesh()
{
    if (canSurfaceColorBePerMesh()) {
        mDrawMode0 &= ~VCL_MRS_POINTS_COLOR_VERTEX;
        mDrawMode0 |= VCL_MRS_POINTS_COLOR_MESH;
        mDrawMode0 &= ~VCL_MRS_POINTS_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudColorUserDefined()
{
    if (canPointCloudBeVisible()) {
        mDrawMode0 &= ~VCL_MRS_POINTS_COLOR_VERTEX;
        mDrawMode0 &= ~VCL_MRS_POINTS_COLOR_MESH;
        mDrawMode0 |= VCL_MRS_POINTS_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointWidth(float width)
{
    if (canPointCloudBeVisible()) {
        mPointWidth = width;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudUserColor(
    float r,
    float g,
    float b,
    float a)
{
    if (canPointCloudBeVisible()) {
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

bool MeshRenderSettings::setPointCloudUserColor(const Color& c)
{
    if (canPointCloudBeVisible()) {
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

bool MeshRenderSettings::setSurfaceVisibility(bool b)
{
    if (canSurfaceBeVisible()) {
        if (b)
            mDrawMode0 |= VCL_MRS_DRAW_SURF;
        else
            mDrawMode0 &= ~VCL_MRS_DRAW_SURF;
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Unsets the shading of the surface (no light).
 * Unsets automatically the smooth and flat shadings.
 */
bool MeshRenderSettings::setSurfaceShadingNone()
{
    if (canSurfaceBeVisible()) {
        mDrawMode0 |= VCL_MRS_SURF_SHADING_NONE;
        mDrawMode0 &= ~VCL_MRS_SURF_SHADING_FLAT;
        mDrawMode0 &= ~VCL_MRS_SURF_SHADING_SMOOTH;
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Sets the shading of the surface flat (using triangle normals).
 * Unsets automatically the none and smooth shading.
 */
bool MeshRenderSettings::setSurfaceShadingFlat()
{
    if (canSurfaceShadingBeFlat()) {
        mDrawMode0 |= VCL_MRS_SURF_SHADING_FLAT;
        mDrawMode0 &= ~VCL_MRS_SURF_SHADING_NONE;
        mDrawMode0 &= ~VCL_MRS_SURF_SHADING_SMOOTH;
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Sets the shading of the surface smooth (using vertex normals).
 * Unsets automatically the none and flat shading.
 */
bool MeshRenderSettings::setSurfaceShadingSmooth()
{
    if (canSurfaceShadingBeSmooth()) {
        mDrawMode0 |= VCL_MRS_SURF_SHADING_SMOOTH;
        mDrawMode0 &= ~VCL_MRS_SURF_SHADING_NONE;
        mDrawMode0 &= ~VCL_MRS_SURF_SHADING_FLAT;
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Set the surface coloring per vertex (using the vertex colors).
 * Unsets automatically the other possible surface colorizations:
 * - color per face
 * - color per mesh
 * - color user defined
 * - per vertex texture
 * - per wedge texture
 */
bool MeshRenderSettings::setSurfaceColorPerVertex()
{
    if (canSurfaceColorBePerVertex()) {
        mDrawMode0 |= VCL_MRS_SURF_COLOR_VERTEX;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_FACE;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_MESH;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_USER;
        mDrawMode0 &= ~VCL_MRS_SURF_TEX_VERTEX;
        mDrawMode0 &= ~VCL_MRS_SURF_TEX_WEDGE;
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Set the surface coloring per face (using the face colors).
 * Unsets automatically the other possible surface colorizations:
 * - color per vertex
 * - color per mesh
 * - color user defined
 * - per vertex texture
 * - per wedge texture
 */
bool MeshRenderSettings::setSurfaceColorPerFace()
{
    if (canSurfaceColorBePerFace()) {
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_VERTEX;
        mDrawMode0 |= VCL_MRS_SURF_COLOR_FACE;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_MESH;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_USER;
        mDrawMode0 &= ~VCL_MRS_SURF_TEX_VERTEX;
        mDrawMode0 &= ~VCL_MRS_SURF_TEX_WEDGE;
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Set the surface coloring per mesh (using the mesh color).
 * Unsets automatically the other possible surface colorizations:
 * - color per vertex
 * - color per face
 * - color user defined
 * - per vertex texture
 * - per wedge texture
 */
bool MeshRenderSettings::setSurfaceColorPerMesh()
{
    if (canSurfaceColorBePerMesh()) {
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_VERTEX;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_FACE;
        mDrawMode0 |= VCL_MRS_SURF_COLOR_MESH;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_USER;
        mDrawMode0 &= ~VCL_MRS_SURF_TEX_VERTEX;
        mDrawMode0 &= ~VCL_MRS_SURF_TEX_WEDGE;
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Set the surface coloring by the user defined color.
 * To set the user defined color, you can use the
 * setSurfaceColorUserDefined() function.
 *  * Unsets automatically the other possible surface colorizations:
 * - color per vertex
 * - color per face
 * - color per mesh
 * - per vertex texture
 * - per wedge texture
 */
bool MeshRenderSettings::setSurfaceColorUserDefined()
{
    if (canSurfaceBeVisible()) {
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_VERTEX;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_FACE;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_MESH;
        mDrawMode0 |= VCL_MRS_SURF_COLOR_USER;
        mDrawMode0 &= ~VCL_MRS_SURF_TEX_VERTEX;
        mDrawMode0 &= ~VCL_MRS_SURF_TEX_WEDGE;
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Set the surface texture per vertex texcoords.
 * Unsets automatically the other possible surface colorizations:
 * - color per vertex
 * - color per face
 * - color per mesh
 * - color user defined
 * - per wedge texture
 */
bool MeshRenderSettings::setSurfaceColorPerVertexTexcoords()
{
    if (canSurfaceColorBePerVertexTexcoords()) {
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_VERTEX;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_FACE;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_MESH;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_USER;
        mDrawMode0 |= VCL_MRS_SURF_TEX_VERTEX;
        mDrawMode0 &= ~VCL_MRS_SURF_TEX_WEDGE;
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Set the surface texture per wedge texcoords.
 * Unsets automatically the other possible surface colorizations:
 * - color per vertex
 * - color per face
 * - color per mesh
 * - color user defined
 * - per vertex texture
 */
bool MeshRenderSettings::setSurfaceColorPerWedgeTexcoords()
{
    if (canSurfaceColorBePerWedgeTexcoords()) {
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_VERTEX;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_FACE;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_MESH;
        mDrawMode0 &= ~VCL_MRS_SURF_COLOR_USER;
        mDrawMode0 &= ~VCL_MRS_SURF_TEX_VERTEX;
        mDrawMode0 |= VCL_MRS_SURF_TEX_WEDGE;
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

bool MeshRenderSettings::setWireframeVisibility(bool b)
{
    if (canSurfaceBeVisible()) {
        if (b)
            mDrawMode0 |= VCL_MRS_DRAW_WIREFRAME;
        else
            mDrawMode0 &= ~VCL_MRS_DRAW_WIREFRAME;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeShadingNone()
{
    if (canSurfaceBeVisible()) {
        mDrawMode0 |= VCL_MRS_WIREFRAME_SHADING_NONE;
        mDrawMode0 &= ~VCL_MRS_WIREFRAME_SHADING_VERT;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeShadingPerVertex()
{
    if (canSurfaceBeVisible()) {
        mDrawMode0 &= ~VCL_MRS_WIREFRAME_SHADING_NONE;
        mDrawMode0 |= VCL_MRS_WIREFRAME_SHADING_VERT;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeColorPerVertex()
{
    if (canWireframeColorBePerVertex()) {
        mDrawMode0 |= VCL_MRS_WIREFRAME_COLOR_VERT;
        mDrawMode0 &= ~VCL_MRS_WIREFRAME_COLOR_MESH;
        mDrawMode0 &= ~VCL_MRS_WIREFRAME_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeColorPerMesh()
{
    if (canWireframeColorBePerMesh()) {
        mDrawMode0 &= ~VCL_MRS_WIREFRAME_COLOR_VERT;
        mDrawMode0 |= VCL_MRS_WIREFRAME_COLOR_MESH;
        mDrawMode0 &= ~VCL_MRS_WIREFRAME_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeColorUserDefined()
{
    if (canSurfaceBeVisible()) {
        mDrawMode0 &= ~VCL_MRS_WIREFRAME_COLOR_VERT;
        mDrawMode0 &= ~VCL_MRS_WIREFRAME_COLOR_MESH;
        mDrawMode0 |= VCL_MRS_WIREFRAME_COLOR_USER;
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

bool MeshRenderSettings::setBoundingBoxVisibility(bool b)
{
    if (canBoundingBoxBeVisible()) {
        if (b)
            mDrawMode0 |= VCL_MRS_DRAW_BOUNDINGBOX;
        else
            mDrawMode0 &= ~VCL_MRS_DRAW_BOUNDINGBOX;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesVisibility(bool b)
{
    if (canEdgesBeVisible()) {
        if (b)
            mDrawMode1 |= VCL_MRS_DRAW_EDGES;
        else
            mDrawMode1 &= ~VCL_MRS_DRAW_EDGES;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesShadingNone()
{
    if (canEdgesBeVisible()) {
        mDrawMode1 |= VCL_MRS_EDGES_SHADING_NONE;
        mDrawMode1 &= ~VCL_MRS_EDGES_SHADING_SMOOTH;
        mDrawMode1 &= ~VCL_MRS_EDGES_SHADING_FLAT;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesShadingSmooth()
{
    if (canEdgesShadingBeSmooth()) {
        mDrawMode1 &= ~VCL_MRS_EDGES_SHADING_NONE;
        mDrawMode1 |= VCL_MRS_EDGES_SHADING_SMOOTH;
        mDrawMode1 &= ~VCL_MRS_EDGES_SHADING_FLAT;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesShadingFlat()
{
    if (canEdgesShadingBeFlat()) {
        mDrawMode1 &= ~VCL_MRS_EDGES_SHADING_NONE;
        mDrawMode1 &= ~VCL_MRS_EDGES_SHADING_SMOOTH;
        mDrawMode1 |= VCL_MRS_EDGES_SHADING_FLAT;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesColorPerVertex()
{
    if (canEdgesColorBePerVertex()) {
        mDrawMode1 |= VCL_MRS_EDGES_COLOR_VERTEX;
        mDrawMode1 &= ~VCL_MRS_EDGES_COLOR_EDGE;
        mDrawMode1 &= ~VCL_MRS_EDGES_COLOR_MESH;
        mDrawMode1 &= ~VCL_MRS_EDGES_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesColorPerEdge()
{
    if (canEdgesColorBePerEdge()) {
        mDrawMode1 &= ~VCL_MRS_EDGES_COLOR_VERTEX;
        mDrawMode1 |= VCL_MRS_EDGES_COLOR_EDGE;
        mDrawMode1 &= ~VCL_MRS_EDGES_COLOR_MESH;
        mDrawMode1 &= ~VCL_MRS_EDGES_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesColorPerMesh()
{
    if (canEdgesColorBePerMesh()) {
        mDrawMode1 &= ~VCL_MRS_EDGES_COLOR_VERTEX;
        mDrawMode1 &= ~VCL_MRS_EDGES_COLOR_EDGE;
        mDrawMode1 |= VCL_MRS_EDGES_COLOR_MESH;
        mDrawMode1 &= ~VCL_MRS_EDGES_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setEdgesColorUserDefined()
{
    if (canEdgesBeVisible()) {
        mDrawMode1 &= ~VCL_MRS_EDGES_COLOR_VERTEX;
        mDrawMode1 &= ~VCL_MRS_EDGES_COLOR_EDGE;
        mDrawMode1 &= ~VCL_MRS_EDGES_COLOR_MESH;
        mDrawMode1 |= VCL_MRS_EDGES_COLOR_USER;
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

void MeshRenderSettings::setDefaultSettingsFromCapability()
{
    mDrawMode0 = 0;

    // default settings - ignored if not available
    setWireframeColorUserDefined();

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
            else if (canSurfaceColorBePerMesh()) {
                setSurfaceColorPerMesh();
            }
            else {
                setSurfaceColorUserDefined();
            }
        }

        if (canPointCloudBeVisible()) {
            if (!canSurfaceBeVisible())
                setPointCloudVisibility(true);
            setPointCloudShadingNone();
            if (canPointCloudShadingBePerVertex()) {
                setPointCloudShadingPerVertex();
            }
            if (canPointCloudColorBePerVertex()) {
                setPointCloudColorPerVertex();
            }
            else if (canPointCloudColorBePerMesh()) {
                setPointCloudColorPerMesh();
            }
            else {
                setPointCloudColorUserDefined();
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
            else if (canEdgesColorBePerMesh()) {
                setEdgesColorPerMesh();
            }
            else {
                setEdgesColorUserDefined();
            }
        }
    }
}

} // namespace vcl
