/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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
    enum {
        DRAW_MESH = 1 << 0, // mesh visibility

        // points
        DRAW_POINTS              = 1 << 1, // point visibility
        DRAW_POINTS_PIXEL        = 1 << 2, // draw points as pixels
        DRAW_POINTS_CIRCLE       = 1 << 3, // draw points as circles
        DRAW_POINTS_COLOR_VERTEX = 1 << 4, // use vertex colors for points
        DRAW_POINTS_COLOR_MESH   = 1 << 5, // use mesh color for points
        DRAW_POINTS_COLOR_USER   = 1 << 6, // use user color for points

        // surface
        DRAW_SURF              = 1 << 10, // surface visibility
        DRAW_SURF_FLAT         = 1 << 11, // flat shading
        DRAW_SURF_SMOOTH       = 1 << 12, // smooth shading
        DRAW_SURF_COLOR_FACE   = 1 << 13, // use face color for surface
        DRAW_SURF_COLOR_VERTEX = 1 << 14, // use vertex color for surface
        DRAW_SURF_COLOR_MESH   = 1 << 15, // use mesh color for surface
        DRAW_SURF_COLOR_USER   = 1 << 16, // use user color for surface
        DRAW_SURF_TEX_VERTEX   = 1 << 17, // use per vertex texcoords
        DRAW_SURF_TEX_WEDGE    = 1 << 18, // use per wedge texcoords

        // wireframe
        DRAW_WIREFRAME            = 1 << 20, // draw wireframe
        DRAW_WIREFRAME_COLOR_USER = 1 << 21,
        DRAW_WIREFRAME_COLOR_MESH = 1 << 22,

        // bounding box
        DRAW_BOUNDINGBOX = 1 << 25
    };

    // draw integers controlled using enum
    int dModeCapability = 0;
    int dMode           = 0;

    int   pWidth        = 3;
    float pUserColor[4] = {1, 1, 0, 1};
    float sUserColor[4] = {0.8f, 0.8f, 0.8f, 1.f};
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

    bool canBeVisible() const { return dModeCapability & DRAW_MESH; }

    bool canPointCloudBeVisible() const
    {
        return dModeCapability & DRAW_POINTS;
    }

    bool canPointCloudBeColoredPerVertex() const
    {
        return dModeCapability & DRAW_POINTS_COLOR_VERTEX;
    }

    bool canPointCloudBeColoredPerMesh() const
    {
        return dModeCapability & DRAW_POINTS_COLOR_MESH;
    }

    bool canSurfaceBeVisible() const { return dModeCapability & DRAW_SURF; }

    bool canSurfaceBeSmooth() const
    {
        return dModeCapability & DRAW_SURF_SMOOTH;
    }

    bool canSurfaceBeColoredPerFace() const
    {
        return dModeCapability & DRAW_SURF_COLOR_FACE;
    }

    bool canSurfaceBeColoredPerVertex() const
    {
        return dModeCapability & DRAW_SURF_COLOR_VERTEX;
    }

    bool canSurfaceBeColoredPerMesh() const
    {
        return dModeCapability & DRAW_SURF_COLOR_MESH;
    }

    bool canSurfaceBeColoredPerVertexTexcoords() const
    {
        return dModeCapability & DRAW_SURF_TEX_VERTEX;
    }

    bool canSurfaceBeColoredPerWedgeTexcoords() const
    {
        return dModeCapability & DRAW_SURF_TEX_WEDGE;
    }

    bool canWireframeBeColoredPerMesh() const
    {
        return dModeCapability & DRAW_WIREFRAME_COLOR_MESH;
    }

    bool canBoundingBoxBeVisible() const
    {
        return dModeCapability & DRAW_BOUNDINGBOX;
    }

    // rendering options getters

    int drawMode() const { return dMode; }

    int drawModeCapability() const { return dModeCapability; }

    bool isVisible() const { return dMode & DRAW_MESH; }

    bool isPointCloudVisible() const { return dMode & DRAW_POINTS; }

    bool isPointCloudColorPerVertex() const
    {
        return dMode & DRAW_POINTS_COLOR_VERTEX;
    }

    bool isPointCloudColorPerMesh() const
    {
        return dMode & DRAW_POINTS_COLOR_MESH;
    }

    bool isPointCloudColorUserDefined() const
    {
        return dMode & DRAW_POINTS_COLOR_USER;
    }

    int pointWidth() const { return pWidth; }

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

    bool isSurfaceVisible() const { return dMode & DRAW_SURF; }

    bool isSurfaceShadingFlat() const { return dMode & DRAW_SURF_FLAT; }

    bool isSurfaceShadingSmooth() const { return dMode & DRAW_SURF_SMOOTH; }

    bool isSurfaceColorPerFace() const
    {
        return dMode & DRAW_SURF_COLOR_FACE;
    }

    bool isSurfaceColorPerVertex() const
    {
        return dMode & DRAW_SURF_COLOR_VERTEX;
    }

    bool isSurfaceColorPerMesh() const
    {
        return dMode & DRAW_SURF_COLOR_MESH;
    }

    bool isSurfaceColorUserDefined() const
    {
        return dMode & DRAW_SURF_COLOR_USER;
    }

    bool isSurfaceColorPerVertexTexcoords() const
    {
        return dMode & DRAW_SURF_TEX_VERTEX;
    }

    bool isSurfaceColorPerWedgeTexcoords() const
    {
        return dMode & DRAW_SURF_TEX_WEDGE;
    }

    vcl::Color surfaceUserColor() const
    {
        vcl::Color c;
        c.setRedF(sUserColor[0]);
        c.setGreenF(sUserColor[1]);
        c.setBlueF(sUserColor[2]);
        c.setAlphaF(sUserColor[3]);
        return c;
    }

    const float* surfaceUserColorData() const { return sUserColor; }

    bool isWireframeVisible() const { return dMode & DRAW_WIREFRAME; }

    bool isWireframeColorPerMesh() const
    {
        return dMode & DRAW_WIREFRAME_COLOR_MESH;
    }

    bool isWireframeColorUserDefined() const
    {
        return dMode & DRAW_WIREFRAME_COLOR_USER;
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

    bool isBboxEnabled() const { return dMode & DRAW_BOUNDINGBOX; }

    // rendering options setters

    bool setVisibility(bool b)
    {
        if (canBeVisible()) {
            if (b)
                dMode |= DRAW_MESH;
            else
                dMode &= ~DRAW_MESH;
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
                dMode |= DRAW_POINTS;
            else
                dMode &= ~DRAW_POINTS;
            return true;
        }
        else {
            return false;
        }
    }

    bool setPointCloudColorPerVertex()
    {
        if (canSurfaceBeColoredPerVertex()) {
            dMode |= DRAW_POINTS_COLOR_VERTEX;
            dMode &= ~DRAW_POINTS_COLOR_MESH;
            dMode &= ~DRAW_POINTS_COLOR_USER;
            return true;
        }
        else {
            return false;
        }
    }

    bool setPointCloudColorPerMesh()
    {
        if (canSurfaceBeColoredPerMesh()) {
            dMode &= ~DRAW_POINTS_COLOR_VERTEX;
            dMode |= DRAW_POINTS_COLOR_MESH;
            dMode &= ~DRAW_POINTS_COLOR_USER;
            return true;
        }
        else {
            return false;
        }
    }

    bool setPointCloudColorUserDefined()
    {
        if (canPointCloudBeVisible()) {
            dMode &= ~DRAW_POINTS_COLOR_VERTEX;
            dMode &= ~DRAW_POINTS_COLOR_MESH;
            dMode |= DRAW_POINTS_COLOR_USER;
            return true;
        }
        else {
            return false;
        }
    }

    bool setPointWidth(int width)
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
                dMode |= DRAW_SURF;
            else
                dMode &= ~DRAW_SURF;
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * @brief Sets the visibility of the surface flat (using triangle normals).
     * Unsets automatically the smooth shading.
     */
    bool setSurfaceShadingFlat()
    {
        if (canSurfaceBeVisible()) {
            dMode |= DRAW_SURF_FLAT;
            dMode &= ~DRAW_SURF_SMOOTH;
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * @brief Sets the visibility of the surface smooth (using vertex normals).
     * Unsets automatically the flat shading.
     */
    bool setSurfaceShadingSmooth()
    {
        if (canSurfaceBeSmooth()) {
            dMode |= DRAW_SURF_SMOOTH;
            dMode &= ~DRAW_SURF_FLAT;
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
        if (canSurfaceBeColoredPerVertex()) {
            dMode |= DRAW_SURF_COLOR_VERTEX;
            dMode &= ~DRAW_SURF_COLOR_FACE;
            dMode &= ~DRAW_SURF_COLOR_MESH;
            dMode &= ~DRAW_SURF_COLOR_USER;
            dMode &= ~DRAW_SURF_TEX_VERTEX;
            dMode &= ~DRAW_SURF_TEX_WEDGE;
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
        if (canSurfaceBeColoredPerFace()) {
            dMode &= ~DRAW_SURF_COLOR_VERTEX;
            dMode |= DRAW_SURF_COLOR_FACE;
            dMode &= ~DRAW_SURF_COLOR_MESH;
            dMode &= ~DRAW_SURF_COLOR_USER;
            dMode &= ~DRAW_SURF_TEX_VERTEX;
            dMode &= ~DRAW_SURF_TEX_WEDGE;
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
        if (canSurfaceBeColoredPerMesh()) {
            dMode &= ~DRAW_SURF_COLOR_VERTEX;
            dMode &= ~DRAW_SURF_COLOR_FACE;
            dMode |= DRAW_SURF_COLOR_MESH;
            dMode &= ~DRAW_SURF_COLOR_USER;
            dMode &= ~DRAW_SURF_TEX_VERTEX;
            dMode &= ~DRAW_SURF_TEX_WEDGE;
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
            dMode &= ~DRAW_SURF_COLOR_VERTEX;
            dMode &= ~DRAW_SURF_COLOR_FACE;
            dMode &= ~DRAW_SURF_COLOR_MESH;
            dMode |= DRAW_SURF_COLOR_USER;
            dMode &= ~DRAW_SURF_TEX_VERTEX;
            dMode &= ~DRAW_SURF_TEX_WEDGE;
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
        if (canSurfaceBeColoredPerVertexTexcoords()) {
            dMode &= ~DRAW_SURF_COLOR_VERTEX;
            dMode &= ~DRAW_SURF_COLOR_FACE;
            dMode &= ~DRAW_SURF_COLOR_MESH;
            dMode &= ~DRAW_SURF_COLOR_USER;
            dMode |= DRAW_SURF_TEX_VERTEX;
            dMode &= ~DRAW_SURF_TEX_WEDGE;
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
        if (canSurfaceBeColoredPerWedgeTexcoords()) {
            dMode &= ~DRAW_SURF_COLOR_VERTEX;
            dMode &= ~DRAW_SURF_COLOR_FACE;
            dMode &= ~DRAW_SURF_COLOR_MESH;
            dMode &= ~DRAW_SURF_COLOR_USER;
            dMode &= ~DRAW_SURF_TEX_VERTEX;
            dMode |= DRAW_SURF_TEX_WEDGE;
            return true;
        }
        else {
            return false;
        }
    }

    bool setSurfaceUserColor(float r, float g, float b, float a = 1)
    {
        if (canSurfaceBeVisible()) {
            sUserColor[0] = r;
            sUserColor[1] = g;
            sUserColor[2] = b;
            sUserColor[3] = a;
            return true;
        }
        else {
            return false;
        }
    }

    bool setSurfaceUserColor(const vcl::Color& c)
    {
        if (canSurfaceBeVisible()) {
            sUserColor[0] = c.redF();
            sUserColor[1] = c.greenF();
            sUserColor[2] = c.blueF();
            sUserColor[3] = c.alphaF();
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
                dMode |= DRAW_WIREFRAME;
            else
                dMode &= ~DRAW_WIREFRAME;
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
        if (canWireframeBeColoredPerMesh()) {
            dMode |= DRAW_WIREFRAME_COLOR_MESH;
            dMode &= ~DRAW_WIREFRAME_COLOR_USER;
            return true;
        }
        else {
            return false;
        }
    }

    bool setWireframeColorUserDefined()
    {
        if (canSurfaceBeVisible()) {
            dMode &= ~DRAW_WIREFRAME_COLOR_MESH;
            dMode |= DRAW_WIREFRAME_COLOR_USER;
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
                dMode |= DRAW_BOUNDINGBOX;
            else
                dMode &= ~DRAW_BOUNDINGBOX;
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
            dModeCapability |= DRAW_MESH;

            // -- Points --
            dModeCapability |= DRAW_POINTS;
            dModeCapability |= DRAW_POINTS_PIXEL;
            dModeCapability |= DRAW_POINTS_CIRCLE;
            dModeCapability |= DRAW_POINTS_COLOR_USER;

            if constexpr (vcl::HasPerVertexColor<MeshType>) {
                if (vcl::isPerVertexColorAvailable(m)) {
                    dModeCapability |= DRAW_POINTS_COLOR_VERTEX;
                }
            }

            if constexpr (vcl::HasColor<MeshType>) {
                dModeCapability |= DRAW_POINTS_COLOR_MESH;
            }

            // -- Surface and Wireframe --
            if constexpr (vcl::HasFaces<MeshType>) {
                if (m.faceNumber() > 0) {
                    dModeCapability |= DRAW_SURF;
                    dModeCapability |= DRAW_SURF_FLAT;
                    dModeCapability |= DRAW_SURF_COLOR_USER;
                    dModeCapability |= DRAW_WIREFRAME;
                    dModeCapability |= DRAW_WIREFRAME_COLOR_USER;

                    if constexpr (vcl::HasColor<MeshType>) {
                        dModeCapability |= DRAW_SURF_COLOR_MESH;
                        dModeCapability |= DRAW_WIREFRAME_COLOR_MESH;
                    }

                    if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                        if (vcl::isPerVertexNormalAvailable(m))
                            dModeCapability |= DRAW_SURF_SMOOTH;
                    }

                    if constexpr (vcl::HasPerFaceColor<MeshType>) {
                        if (vcl::isPerFaceColorAvailable(m))
                            dModeCapability |= DRAW_SURF_COLOR_FACE;
                    }

                    if constexpr (vcl::HasPerVertexColor<MeshType>) {
                        if (vcl::isPerVertexColorAvailable(m)) {
                            dModeCapability |= DRAW_SURF_COLOR_VERTEX;
                        }
                    }

                    if constexpr (vcl::HasTexturePaths<MeshType>) {
                        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
                            if (vcl::isPerVertexTexCoordAvailable(m))
                                dModeCapability |= DRAW_SURF_TEX_VERTEX;
                        }

                        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                            if (vcl::isPerFaceWedgeTexCoordsAvailable(m))
                                dModeCapability |= DRAW_SURF_TEX_WEDGE;
                        }
                    }
                }
            }

            // -- Bounding Box --
            if constexpr (vcl::HasBoundingBox<MeshType>) {
                dModeCapability |= DRAW_BOUNDINGBOX;
            }
        }

        // make sure that the previous draw mode satisfies the new capabilites
        dMode &= dModeCapability;
    }

    void setDefaultSettingsFromCapability()
    {
        dMode = 0;

        // default settings - ignored if not available
        setPointCloudColorUserDefined();
        setSurfaceColorUserDefined();
        setWireframeColorUserDefined();

        if (canBeVisible()) {
            setVisibility(true);
            if (canSurfaceBeVisible()) {
                setSurfaceVisibility(true);
                if (canSurfaceBeSmooth()) {
                    setSurfaceShadingSmooth();
                }
                else {
                    setSurfaceShadingFlat();
                }
                if (canSurfaceBeColoredPerVertex()) {
                    setSurfaceColorPerVertex();
                }
                else if (canSurfaceBeColoredPerFace()) {
                    setSurfaceColorPerFace();
                }
                else if (canSurfaceBeColoredPerWedgeTexcoords()) {
                    setSurfaceColorPerWedgeTexcoords();
                }
                else if (canSurfaceBeColoredPerVertexTexcoords()) {
                    setSurfaceColorPerVertexTexcoords();
                }
                else if (canSurfaceBeColoredPerMesh()) {
                    setSurfaceColorPerMesh();
                }
            }
            else {
                if (canPointCloudBeVisible()) {
                    setPointCloudVisibility(true);
                    if (canPointCloudBeColoredPerVertex()) {
                        setPointCloudColorPerVertex();
                    }
                    else if (canPointCloudBeColoredPerMesh()) {
                        setPointCloudColorPerMesh();
                    }
                }
            }
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_MESH_RENDER_SETTINGS_H
