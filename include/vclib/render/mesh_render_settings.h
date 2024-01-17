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

#ifndef VCL_RENDER_MESH_RENDER_SETTINGS_H
#define VCL_RENDER_MESH_RENDER_SETTINGS_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/color.h>

#include "mesh_render_settings_macros.h"

namespace vcl {

/**
 * @brief The MeshRenderSettings class allows an easy management of render
 * settings of a Mesh. This class stores the rendering status of a Mesh and the
 * rendering capability of a Mesh.
 *
 * Render capabilities store what can be actually rendered of a vcl::Mesh (e.g.
 * it will be possible to render the surface of the mesh if the Mesh has Faces,
 * or it will be possible to render per Face colors only if the Mesh has
 * **enabled** per face colors).
 *
 * The render status of this class is **guaranteed to be consistent** to the
 * render capabilities.
 *
 * An instance of this class must be initialized with a vcl::Mesh at first, in
 * order to initialize the Render capabilites. Render settings won't be possible
 * to set if this class is not first initialized using a vcl::Mesh, trough the
 * constructor or the `setRenderCapabilityFrom()` member function.
 *
 * Render capabilities can be get by calling the member functions `can*()` of
 * this class.
 *
 * The memeber functions `is*()` allow to get the current render status.
 *
 * The render status can be modified using the `set*()` member functions, that
 * return a boolean indicating if the operation has been performed (if the
 * capabilities allows it, the operation will be always performed).
 */
class MeshRenderSettings
{
    // draw integers controlled using macros (same macros used also in shaders)
    uint dModeCapability = 0;
    uint dMode           = 0;

    float pWidth        = 3;
    float pUserColor[4] = {1, 1, 0, 1};
    uint  sUserColor = 0xFF808080; // abgr
    int   wWidth        = 1;
    float wUserColor[4] = {0, 0, 0, 1};

public:
    MeshRenderSettings() = default;

    template<MeshConcept MeshType>
    MeshRenderSettings(const MeshType& m)
    {
        setRenderCapabilityFrom(m);
        setDefaultSettingsFromCapability();
    }

    // rendering options capability of the mesh

    bool canBeVisible() const { return dModeCapability & VCL_MRS_DRAW_MESH; }

    bool canPointCloudBeVisible() const
    {
        return dModeCapability & VCL_MRS_DRAW_POINTS;
    }

    bool canPointCloudShadingBePerVertex() const
    {
        return dModeCapability & VCL_MRS_POINTS_SHADING_VERT;
    }

    bool canPointCloudColorBePerVertex() const
    {
        return dModeCapability & VCL_MRS_POINTS_COLOR_VERTEX;
    }

    bool canPointCloudColorBePerMesh() const
    {
        return dModeCapability & VCL_MRS_POINTS_COLOR_MESH;
    }

    bool canSurfaceBeVisible() const
    {
        return dModeCapability & VCL_MRS_DRAW_SURF;
    }

    bool canSurfaceShadingBeFlat() const
    {
        return dModeCapability & VCL_MRS_SURF_SHADING_FLAT;
    }

    bool canSurfaceShadingBeSmooth() const
    {
        return dModeCapability & VCL_MRS_SURF_SHADING_SMOOTH;
    }

    bool canSurfaceColorBePerFace() const
    {
        return dModeCapability & VCL_MRS_SURF_COLOR_FACE;
    }

    bool canSurfaceColorBePerVertex() const
    {
        return dModeCapability & VCL_MRS_SURF_COLOR_VERTEX;
    }

    bool canSurfaceColorBePerMesh() const
    {
        return dModeCapability & VCL_MRS_SURF_COLOR_MESH;
    }

    bool canSurfaceColorBePerVertexTexcoords() const
    {
        return dModeCapability & VCL_MRS_SURF_TEX_VERTEX;
    }

    bool canSurfaceColorBePerWedgeTexcoords() const
    {
        return dModeCapability & VCL_MRS_SURF_TEX_WEDGE;
    }

    bool canWireframeColorBePerMesh() const
    {
        return dModeCapability & VCL_MRS_WIREFRAME_COLOR_MESH;
    }

    bool canBoundingBoxBeVisible() const
    {
        return dModeCapability & VCL_MRS_DRAW_BOUNDINGBOX;
    }

    // rendering options getters

    uint drawMode() const { return dMode; }

    uint drawModeCapability() const { return dModeCapability; }

    bool isVisible() const { return dMode & VCL_MRS_DRAW_MESH; }

    bool isPointCloudVisible() const { return dMode & VCL_MRS_DRAW_POINTS; }

    bool isPointCloudShadingNone() const
    {
        return dMode & VCL_MRS_POINTS_SHADING_NONE;
    }

    bool isPointCloudShadingPerVertex() const
    {
        return dMode & VCL_MRS_POINTS_SHADING_VERT;
    }

    bool isPointCloudColorPerVertex() const
    {
        return dMode & VCL_MRS_POINTS_COLOR_VERTEX;
    }

    bool isPointCloudColorPerMesh() const
    {
        return dMode & VCL_MRS_POINTS_COLOR_MESH;
    }

    bool isPointCloudColorUserDefined() const
    {
        return dMode & VCL_MRS_POINTS_COLOR_USER;
    }

    float pointWidth() const { return pWidth; }

    vcl::Color pointCloudUserColor() const
    {
        vcl::Color c;
        c.setRedF(pUserColor[0]);
        c.setGreenF(pUserColor[1]);
        c.setBlueF(pUserColor[2]);
        c.setAlphaF(pUserColor[3]);
        return c;
    }

    const float* pointCloudUserColorData() const { return pUserColor; }

    bool isSurfaceVisible() const { return dMode & VCL_MRS_DRAW_SURF; }

    bool isSurfaceShadingNone() const
    {
        return dMode & VCL_MRS_SURF_SHADING_NONE;
    }

    bool isSurfaceShadingFlat() const
    {
        return dMode & VCL_MRS_SURF_SHADING_FLAT;
    }

    bool isSurfaceShadingSmooth() const
    {
        return dMode & VCL_MRS_SURF_SHADING_SMOOTH;
    }

    bool isSurfaceColorPerFace() const
    {
        return dMode & VCL_MRS_SURF_COLOR_FACE;
    }

    bool isSurfaceColorPerVertex() const
    {
        return dMode & VCL_MRS_SURF_COLOR_VERTEX;
    }

    bool isSurfaceColorPerMesh() const
    {
        return dMode & VCL_MRS_SURF_COLOR_MESH;
    }

    bool isSurfaceColorUserDefined() const
    {
        return dMode & VCL_MRS_SURF_COLOR_USER;
    }

    bool isSurfaceColorPerVertexTexcoords() const
    {
        return dMode & VCL_MRS_SURF_TEX_VERTEX;
    }

    bool isSurfaceColorPerWedgeTexcoords() const
    {
        return dMode & VCL_MRS_SURF_TEX_WEDGE;
    }

    vcl::Color surfaceUserColor() const
    {
        vcl::Color c;
        c.setAbgr(sUserColor);
        return c;
    }

    const uint* surfaceUserColorData() const { return &sUserColor; }

    bool isWireframeVisible() const { return dMode & VCL_MRS_DRAW_WIREFRAME; }

    bool isWireframeColorPerMesh() const
    {
        return dMode & VCL_MRS_WIREFRAME_COLOR_MESH;
    }

    bool isWireframeColorUserDefined() const
    {
        return dMode & VCL_MRS_WIREFRAME_COLOR_USER;
    }

    int wireframeWidth() const { return wWidth; }

    vcl::Color wireframeUserColor() const
    {
        vcl::Color c;
        c.setRedF(wUserColor[0]);
        c.setGreenF(wUserColor[1]);
        c.setBlueF(wUserColor[2]);
        c.setAlphaF(wUserColor[3]);
        return c;
    }

    const float* wireframeUserColorData() const { return wUserColor; }

    bool isBboxEnabled() const { return dMode & VCL_MRS_DRAW_BOUNDINGBOX; }

    // rendering options setters

    bool setVisibility(bool b)
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

    bool setPointCloudVisibility(bool b)
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

    bool setPointCloudShadingNone()
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

    bool setPointCloudShadingPerVertex()
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

    bool setPointCloudColorPerVertex()
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

    bool setPointCloudColorPerMesh()
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

    bool setPointCloudColorUserDefined()
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

    bool setPointWidth(float width)
    {
        if (canPointCloudBeVisible()) {
            pWidth = width;
            return true;
        }
        else {
            return false;
        }
    }

    bool setPointCloudUserColor(float r, float g, float b, float a = 1)
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

    bool setPointCloudUserColor(const vcl::Color& c)
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

    bool setSurfaceVisibility(bool b)
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
    bool setSurfaceShadingNone()
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
    bool setSurfaceShadingFlat()
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
    bool setSurfaceShadingSmooth()
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
    bool setSurfaceColorPerVertex()
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
    bool setSurfaceColorPerFace()
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
    bool setSurfaceColorPerMesh()
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
     *
     * Unsets automatically the other possible surface colorizations:
     * - color per vertex
     * - color per face
     * - color per mesh
     * - per vertex texture
     * - per wedge texture
     */
    bool setSurfaceColorUserDefined()
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
    bool setSurfaceColorPerVertexTexcoords()
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
    bool setSurfaceColorPerWedgeTexcoords()
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

    bool setSurfaceUserColor(float r, float g, float b, float a = 1)
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

    bool setSurfaceUserColor(const vcl::Color& c)
    {
        if (canSurfaceBeVisible()) {
            sUserColor = c.abgr();
            return true;
        }
        else {
            return false;
        }
    }

    bool setWireframeVisibility(bool b)
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

    bool setWireframeWidth(int width)
    {
        if (canSurfaceBeVisible()) {
            wWidth = width;
            return true;
        }
        else {
            return false;
        }
    }

    bool setWireframeColorPerMesh()
    {
        if (canWireframeColorBePerMesh()) {
            dMode |= VCL_MRS_WIREFRAME_COLOR_MESH;
            dMode &= ~VCL_MRS_WIREFRAME_COLOR_USER;
            return true;
        }
        else {
            return false;
        }
    }

    bool setWireframeColorUserDefined()
    {
        if (canSurfaceBeVisible()) {
            dMode &= ~VCL_MRS_WIREFRAME_COLOR_MESH;
            dMode |= VCL_MRS_WIREFRAME_COLOR_USER;
            return true;
        }
        else {
            return false;
        }
    }

    bool setWireframeUserColor(float r, float g, float b, float a = 1)
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

    bool setWireframeUserColor(const vcl::Color& c)
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

    bool setBoundingBoxVisibility(bool b)
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

    template<MeshConcept MeshType>
    void setRenderCapabilityFrom(const MeshType& m)
    {
        dModeCapability = 0;

        if (m.vertexNumber() > 0) {
            dModeCapability |= VCL_MRS_DRAW_MESH;

            // -- Points --
            dModeCapability |= VCL_MRS_DRAW_POINTS;
            dModeCapability |= VCL_MRS_POINTS_SHADING_NONE;
            dModeCapability |= VCL_MRS_POINTS_PIXEL;
            dModeCapability |= VCL_MRS_POINTS_CIRCLE;
            dModeCapability |= VCL_MRS_POINTS_COLOR_USER;

            if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                if (vcl::isPerVertexNormalAvailable(m)) {
                    dModeCapability |= VCL_MRS_POINTS_SHADING_VERT;
                }
            }

            if constexpr (vcl::HasPerVertexColor<MeshType>) {
                if (vcl::isPerVertexColorAvailable(m)) {
                    dModeCapability |= VCL_MRS_POINTS_COLOR_VERTEX;
                }
            }

            if constexpr (vcl::HasColor<MeshType>) {
                dModeCapability |= VCL_MRS_POINTS_COLOR_MESH;
            }

            // -- Surface and Wireframe --
            if constexpr (vcl::HasFaces<MeshType>) {
                if (m.faceNumber() > 0) {
                    dModeCapability |= VCL_MRS_DRAW_SURF;
                    dModeCapability |= VCL_MRS_SURF_SHADING_NONE;
                    dModeCapability |= VCL_MRS_SURF_COLOR_USER;
                    dModeCapability |= VCL_MRS_DRAW_WIREFRAME;
                    dModeCapability |= VCL_MRS_WIREFRAME_COLOR_USER;

                    if constexpr (vcl::HasColor<MeshType>) {
                        dModeCapability |= VCL_MRS_SURF_COLOR_MESH;
                        dModeCapability |= VCL_MRS_WIREFRAME_COLOR_MESH;
                    }

                    if constexpr (vcl::HasPerFaceNormal<MeshType>) {
                        if (vcl::isPerFaceNormalAvailable(m)) {
                            dModeCapability |= VCL_MRS_SURF_SHADING_FLAT;
                        }
                    }

                    if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                        if (vcl::isPerVertexNormalAvailable(m))
                            dModeCapability |= VCL_MRS_SURF_SHADING_SMOOTH;
                    }

                    if constexpr (vcl::HasPerFaceColor<MeshType>) {
                        if (vcl::isPerFaceColorAvailable(m))
                            dModeCapability |= VCL_MRS_SURF_COLOR_FACE;
                    }

                    if constexpr (vcl::HasPerVertexColor<MeshType>) {
                        if (vcl::isPerVertexColorAvailable(m)) {
                            dModeCapability |= VCL_MRS_SURF_COLOR_VERTEX;
                        }
                    }

                    if constexpr (vcl::HasTexturePaths<MeshType>) {
                        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
                            if (vcl::isPerVertexTexCoordAvailable(m))
                                dModeCapability |= VCL_MRS_SURF_TEX_VERTEX;
                        }

                        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                            if (vcl::isPerFaceWedgeTexCoordsAvailable(m))
                                dModeCapability |= VCL_MRS_SURF_TEX_WEDGE;
                        }
                    }
                }
            }

            // -- Bounding Box --
            if constexpr (vcl::HasBoundingBox<MeshType>) {
                dModeCapability |= VCL_MRS_DRAW_BOUNDINGBOX;
            }
        }

        // make sure that the previous draw mode satisfies the new capabilites
        dMode &= dModeCapability;
    }

    void setDefaultSettingsFromCapability()
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
            else {
                if (canPointCloudBeVisible()) {
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
    }
};

} // namespace vcl

#endif // VCL_RENDER_MESH_RENDER_SETTINGS_H
