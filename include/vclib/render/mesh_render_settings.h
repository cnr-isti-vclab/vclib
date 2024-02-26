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
    uint  sUserColor    = 0xFF808080; // abgr
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

    bool canWireframeShadingBePerVertex() const
    {
        return dModeCapability & VCL_MRS_WIREFRAME_SHADING_VERT;
    }

    bool canWireframeColorBePerVertex() const
    {
        return dModeCapability & VCL_MRS_WIREFRAME_COLOR_VERT;
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

    vcl::Color pointCloudUserColor() const;

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

    vcl::Color surfaceUserColor() const;

    const uint* surfaceUserColorData() const { return &sUserColor; }

    bool isWireframeVisible() const { return dMode & VCL_MRS_DRAW_WIREFRAME; }

    bool isWireframeShadingNone() const
    {
        return dMode & VCL_MRS_WIREFRAME_SHADING_NONE;
    }

    bool isWireframeShadingPerVertex() const
    {
        return dMode & VCL_MRS_WIREFRAME_SHADING_VERT;
    }

    bool isWireframeColorPerVertex() const
    {
        return dMode & VCL_MRS_WIREFRAME_COLOR_VERT;
    }

    bool isWireframeColorPerMesh() const
    {
        return dMode & VCL_MRS_WIREFRAME_COLOR_MESH;
    }

    bool isWireframeColorUserDefined() const
    {
        return dMode & VCL_MRS_WIREFRAME_COLOR_USER;
    }

    int wireframeWidth() const { return wWidth; }

    vcl::Color wireframeUserColor() const;

    const float* wireframeUserColorData() const { return wUserColor; }

    bool isBboxEnabled() const { return dMode & VCL_MRS_DRAW_BOUNDINGBOX; }

    // rendering options setters

    bool setVisibility(bool b);

    bool setPointCloudVisibility(bool b);

    bool setPointCloudShadingNone();

    bool setPointCloudShadingPerVertex();

    bool setPointCloudColorPerVertex();

    bool setPointCloudColorPerMesh();

    bool setPointCloudColorUserDefined();

    bool setPointWidth(float width);

    bool setPointCloudUserColor(float r, float g, float b, float a = 1);

    bool setPointCloudUserColor(const vcl::Color& c);

    bool setSurfaceVisibility(bool b);

    bool setSurfaceShadingNone();

    bool setSurfaceShadingFlat();

    bool setSurfaceShadingSmooth();

    bool setSurfaceColorPerVertex();

    bool setSurfaceColorPerFace();

    bool setSurfaceColorPerMesh();

    bool setSurfaceColorUserDefined();

    bool setSurfaceColorPerVertexTexcoords();

    bool setSurfaceColorPerWedgeTexcoords();

    bool setSurfaceUserColor(float r, float g, float b, float a = 1);

    bool setSurfaceUserColor(const vcl::Color& c);

    bool setWireframeVisibility(bool b);

    bool setWireframeShadingNone();

    bool setWireframeShadingPerVertex();

    bool setWireframeColorPerVertex();

    bool setWireframeColorPerMesh();

    bool setWireframeColorUserDefined();

    bool setWireframeUserColor(float r, float g, float b, float a = 1);

    bool setWireframeWidth(int width);

    bool setWireframeUserColor(const vcl::Color& c);

    bool setBoundingBoxVisibility(bool b);

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
                    dModeCapability |= VCL_MRS_WIREFRAME_SHADING_NONE;
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
                        if (vcl::isPerVertexNormalAvailable(m)) {
                            dModeCapability |= VCL_MRS_SURF_SHADING_SMOOTH;
                            dModeCapability |= VCL_MRS_WIREFRAME_SHADING_VERT;
                        }
                    }

                    if constexpr (vcl::HasPerFaceColor<MeshType>) {
                        if (vcl::isPerFaceColorAvailable(m))
                            dModeCapability |= VCL_MRS_SURF_COLOR_FACE;
                    }

                    if constexpr (vcl::HasPerVertexColor<MeshType>) {
                        if (vcl::isPerVertexColorAvailable(m)) {
                            dModeCapability |= VCL_MRS_SURF_COLOR_VERTEX;
                            dModeCapability |= VCL_MRS_WIREFRAME_COLOR_VERT;
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

    void setDefaultSettingsFromCapability();
};

} // namespace vcl

#endif // VCL_RENDER_MESH_RENDER_SETTINGS_H
