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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <vclib/render/mesh_render_settings.h>

namespace vcl {

Color MeshRenderSettings::pointCloudUserColor() const
{
    vcl::Color c;
    c.setRedF(pUserColor[0]);
    c.setGreenF(pUserColor[1]);
    c.setBlueF(pUserColor[2]);
    c.setAlphaF(pUserColor[3]);
    return c;
}

Color MeshRenderSettings::surfaceUserColor() const
{
    vcl::Color c;
    c.setAbgr(sUserColor);
    return c;
}

Color MeshRenderSettings::wireframeUserColor() const
{
    vcl::Color c;
    c.setRedF(wUserColor[0]);
    c.setGreenF(wUserColor[1]);
    c.setBlueF(wUserColor[2]);
    c.setAlphaF(wUserColor[3]);
    return c;
}

bool MeshRenderSettings::setVisibility(bool b)
{
    if (canBeVisible()) {
        if (b)
            dMode |= VCL_MRS_DRAW_MESH;
        else
            dMode &= ~VCL_MRS_DRAW_MESH;
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
            dMode |= VCL_MRS_DRAW_POINTS;
        else
            dMode &= ~VCL_MRS_DRAW_POINTS;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudShadingNone()
{
    if (canPointCloudBeVisible()) {
        dMode |= VCL_MRS_POINTS_SHADING_NONE;
        dMode &= ~VCL_MRS_POINTS_SHADING_VERT;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudShadingPerVertex()
{
    if (canPointCloudBeVisible()) {
        dMode &= ~VCL_MRS_POINTS_SHADING_NONE;
        dMode |= VCL_MRS_POINTS_SHADING_VERT;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudColorPerVertex()
{
    if (canSurfaceColorBePerVertex()) {
        dMode |= VCL_MRS_POINTS_COLOR_VERTEX;
        dMode &= ~VCL_MRS_POINTS_COLOR_MESH;
        dMode &= ~VCL_MRS_POINTS_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudColorPerMesh()
{
    if (canSurfaceColorBePerMesh()) {
        dMode &= ~VCL_MRS_POINTS_COLOR_VERTEX;
        dMode |= VCL_MRS_POINTS_COLOR_MESH;
        dMode &= ~VCL_MRS_POINTS_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudColorUserDefined()
{
    if (canPointCloudBeVisible()) {
        dMode &= ~VCL_MRS_POINTS_COLOR_VERTEX;
        dMode &= ~VCL_MRS_POINTS_COLOR_MESH;
        dMode |= VCL_MRS_POINTS_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointWidth(float width)
{
    if (canPointCloudBeVisible()) {
        pWidth = width;
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
        pUserColor[0] = r;
        pUserColor[1] = g;
        pUserColor[2] = b;
        pUserColor[3] = a;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setPointCloudUserColor(const Color& c)
{
    if (canPointCloudBeVisible()) {
        pUserColor[0] = c.redF();
        pUserColor[1] = c.greenF();
        pUserColor[2] = c.blueF();
        pUserColor[3] = c.alphaF();
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
            dMode |= VCL_MRS_DRAW_SURF;
        else
            dMode &= ~VCL_MRS_DRAW_SURF;
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
        dMode |= VCL_MRS_SURF_SHADING_NONE;
        dMode &= ~VCL_MRS_SURF_SHADING_FLAT;
        dMode &= ~VCL_MRS_SURF_SHADING_SMOOTH;
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
        dMode |= VCL_MRS_SURF_SHADING_FLAT;
        dMode &= ~VCL_MRS_SURF_SHADING_NONE;
        dMode &= ~VCL_MRS_SURF_SHADING_SMOOTH;
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
        dMode |= VCL_MRS_SURF_SHADING_SMOOTH;
        dMode &= ~VCL_MRS_SURF_SHADING_NONE;
        dMode &= ~VCL_MRS_SURF_SHADING_FLAT;
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
        dMode |= VCL_MRS_SURF_COLOR_VERTEX;
        dMode &= ~VCL_MRS_SURF_COLOR_FACE;
        dMode &= ~VCL_MRS_SURF_COLOR_MESH;
        dMode &= ~VCL_MRS_SURF_COLOR_USER;
        dMode &= ~VCL_MRS_SURF_TEX_VERTEX;
        dMode &= ~VCL_MRS_SURF_TEX_WEDGE;
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
        dMode &= ~VCL_MRS_SURF_COLOR_VERTEX;
        dMode |= VCL_MRS_SURF_COLOR_FACE;
        dMode &= ~VCL_MRS_SURF_COLOR_MESH;
        dMode &= ~VCL_MRS_SURF_COLOR_USER;
        dMode &= ~VCL_MRS_SURF_TEX_VERTEX;
        dMode &= ~VCL_MRS_SURF_TEX_WEDGE;
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
        dMode &= ~VCL_MRS_SURF_COLOR_VERTEX;
        dMode &= ~VCL_MRS_SURF_COLOR_FACE;
        dMode |= VCL_MRS_SURF_COLOR_MESH;
        dMode &= ~VCL_MRS_SURF_COLOR_USER;
        dMode &= ~VCL_MRS_SURF_TEX_VERTEX;
        dMode &= ~VCL_MRS_SURF_TEX_WEDGE;
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
        dMode &= ~VCL_MRS_SURF_COLOR_VERTEX;
        dMode &= ~VCL_MRS_SURF_COLOR_FACE;
        dMode &= ~VCL_MRS_SURF_COLOR_MESH;
        dMode |= VCL_MRS_SURF_COLOR_USER;
        dMode &= ~VCL_MRS_SURF_TEX_VERTEX;
        dMode &= ~VCL_MRS_SURF_TEX_WEDGE;
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
        dMode &= ~VCL_MRS_SURF_COLOR_VERTEX;
        dMode &= ~VCL_MRS_SURF_COLOR_FACE;
        dMode &= ~VCL_MRS_SURF_COLOR_MESH;
        dMode &= ~VCL_MRS_SURF_COLOR_USER;
        dMode |= VCL_MRS_SURF_TEX_VERTEX;
        dMode &= ~VCL_MRS_SURF_TEX_WEDGE;
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
        dMode &= ~VCL_MRS_SURF_COLOR_VERTEX;
        dMode &= ~VCL_MRS_SURF_COLOR_FACE;
        dMode &= ~VCL_MRS_SURF_COLOR_MESH;
        dMode &= ~VCL_MRS_SURF_COLOR_USER;
        dMode &= ~VCL_MRS_SURF_TEX_VERTEX;
        dMode |= VCL_MRS_SURF_TEX_WEDGE;
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
        sUserColor = c.abgr();
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setSurfaceUserColor(const Color& c)
{
    if (canSurfaceBeVisible()) {
        sUserColor = c.abgr();
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
            dMode |= VCL_MRS_DRAW_WIREFRAME;
        else
            dMode &= ~VCL_MRS_DRAW_WIREFRAME;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeShadingNone()
{
    if (canSurfaceBeVisible()) {
        dMode |= VCL_MRS_WIREFRAME_SHADING_NONE;
        dMode &= ~VCL_MRS_WIREFRAME_SHADING_VERT;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeShadingPerVertex()
{
    if (canSurfaceBeVisible()) {
        dMode &= ~VCL_MRS_WIREFRAME_SHADING_NONE;
        dMode |= VCL_MRS_WIREFRAME_SHADING_VERT;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeColorPerVertex()
{
    if (canWireframeColorBePerVertex()) {
        dMode |= VCL_MRS_WIREFRAME_COLOR_VERT;
        dMode &= ~VCL_MRS_WIREFRAME_COLOR_MESH;
        dMode &= ~VCL_MRS_WIREFRAME_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeColorPerMesh()
{
    if (canWireframeColorBePerMesh()) {
        dMode &= ~VCL_MRS_WIREFRAME_COLOR_VERT;
        dMode |= VCL_MRS_WIREFRAME_COLOR_MESH;
        dMode &= ~VCL_MRS_WIREFRAME_COLOR_USER;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeColorUserDefined()
{
    if (canSurfaceBeVisible()) {
        dMode &= ~VCL_MRS_WIREFRAME_COLOR_VERT;
        dMode &= ~VCL_MRS_WIREFRAME_COLOR_MESH;
        dMode |= VCL_MRS_WIREFRAME_COLOR_USER;
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
        wUserColor[0] = r;
        wUserColor[1] = g;
        wUserColor[2] = b;
        wUserColor[3] = a;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeWidth(int width)
{
    if (canSurfaceBeVisible()) {
        wWidth = width;
        return true;
    }
    else {
        return false;
    }
}

bool MeshRenderSettings::setWireframeUserColor(const Color& c)
{
    if (canSurfaceBeVisible()) {
        wUserColor[0] = c.redF();
        wUserColor[1] = c.greenF();
        wUserColor[2] = c.blueF();
        wUserColor[3] = c.alphaF();
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
            dMode |= VCL_MRS_DRAW_BOUNDINGBOX;
        else
            dMode &= ~VCL_MRS_DRAW_BOUNDINGBOX;
        return true;
    }
    else {
        return false;
    }
}

void MeshRenderSettings::setDefaultSettingsFromCapability()
{
    dMode = 0;

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
    }
}

} // namespace vcl
