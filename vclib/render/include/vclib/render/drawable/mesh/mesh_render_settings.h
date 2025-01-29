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

#ifndef VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_SETTINGS_H
#define VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_SETTINGS_H

#include "mesh_render_settings_macros.h"

#include <vclib/mesh/requirements.h>
#include <vclib/space/core/color.h>

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

    // draw mode 0: general visibility, points, surface and wireframe
    uint mDrawModeCapability0 = 0;
    uint mDrawMode0           = 0;

    // draw mode 1: edges, ... (to be implemented)
    uint mDrawModeCapability1 = 0;
    uint mDrawMode1           = 0;

    float mPointWidth        = 3;
    float mPointUserColor[4] = {1, 1, 0, 1}; // todo: change to uint
    uint  mSurfUserColor     = 0xFF808080;   // abgr
    int   mWrfWidth          = 1;
    float mWrfUserColor[4]   = {0, 0, 0, 1}; // todo: change to uint
    int   mEdgesWidth        = 1;
    uint  mEdgesUserColor    = 0xFF000000; // abgr

public:
    MeshRenderSettings() = default;

    template<MeshConcept MeshType>
    MeshRenderSettings(const MeshType& m)
    {
        setRenderCapabilityFrom(m);
        setDefaultSettingsFromCapability();
    }

    // rendering options capability of the mesh

    bool canBeVisible() const
    {
        return mDrawModeCapability0 & VCL_MRS_DRAW_MESH;
    }

    bool canPointBeVisible() const
    {
        return mDrawModeCapability0 & VCL_MRS_DRAW_POINTS;
    }

    bool canPointShadingBePerVertex() const
    {
        return mDrawModeCapability0 & VCL_MRS_POINTS_SHADING_VERT;
    }

    bool canPointColorBePerVertex() const
    {
        return mDrawModeCapability0 & VCL_MRS_POINTS_COLOR_VERTEX;
    }

    bool canPointColorBePerMesh() const
    {
        return mDrawModeCapability0 & VCL_MRS_POINTS_COLOR_MESH;
    }

    bool canSurfaceBeVisible() const
    {
        return mDrawModeCapability0 & VCL_MRS_DRAW_SURF;
    }

    bool canSurfaceShadingBeFlat() const
    {
        return mDrawModeCapability0 & VCL_MRS_SURF_SHADING_FLAT;
    }

    bool canSurfaceShadingBeSmooth() const
    {
        return mDrawModeCapability0 & VCL_MRS_SURF_SHADING_SMOOTH;
    }

    bool canSurfaceColorBePerFace() const
    {
        return mDrawModeCapability0 & VCL_MRS_SURF_COLOR_FACE;
    }

    bool canSurfaceColorBePerVertex() const
    {
        return mDrawModeCapability0 & VCL_MRS_SURF_COLOR_VERTEX;
    }

    bool canSurfaceColorBePerMesh() const
    {
        return mDrawModeCapability0 & VCL_MRS_SURF_COLOR_MESH;
    }

    bool canSurfaceColorBePerVertexTexcoords() const
    {
        return mDrawModeCapability0 & VCL_MRS_SURF_TEX_VERTEX;
    }

    bool canSurfaceColorBePerWedgeTexcoords() const
    {
        return mDrawModeCapability0 & VCL_MRS_SURF_TEX_WEDGE;
    }

    bool canWireframeShadingBePerVertex() const
    {
        return mDrawModeCapability0 & VCL_MRS_WIREFRAME_SHADING_VERT;
    }

    bool canWireframeColorBePerVertex() const
    {
        return mDrawModeCapability0 & VCL_MRS_WIREFRAME_COLOR_VERT;
    }

    bool canWireframeColorBePerMesh() const
    {
        return mDrawModeCapability0 & VCL_MRS_WIREFRAME_COLOR_MESH;
    }

    bool canBoundingBoxBeVisible() const
    {
        return mDrawModeCapability0 & VCL_MRS_DRAW_BOUNDINGBOX;
    }

    bool canEdgesBeVisible() const
    {
        return mDrawModeCapability1 & VCL_MRS_DRAW_EDGES;
    }

    bool canEdgesShadingBeSmooth() const
    {
        return mDrawModeCapability1 & VCL_MRS_EDGES_SHADING_SMOOTH;
    }

    bool canEdgesShadingBeFlat() const
    {
        return mDrawModeCapability1 & VCL_MRS_EDGES_SHADING_FLAT;
    }

    bool canEdgesColorBePerVertex() const
    {
        return mDrawModeCapability1 & VCL_MRS_EDGES_COLOR_VERTEX;
    }

    bool canEdgesColorBePerEdge() const
    {
        return mDrawModeCapability1 & VCL_MRS_EDGES_COLOR_EDGE;
    }

    bool canEdgesColorBePerMesh() const
    {
        return mDrawModeCapability1 & VCL_MRS_EDGES_COLOR_MESH;
    }

    // rendering options getters

    uint drawMode0() const { return mDrawMode0; }

    uint drawMode1() const { return mDrawMode1; }

    uint drawModeCapability0() const { return mDrawModeCapability0; }

    uint drawModeCapability1() const { return mDrawModeCapability1; }

    bool isVisible() const { return mDrawMode0 & VCL_MRS_DRAW_MESH; }

    bool isPointVisible() const { return mDrawMode0 & VCL_MRS_DRAW_POINTS; }

    bool isPointShadingNone() const
    {
        return mDrawMode0 & VCL_MRS_POINTS_SHADING_NONE;
    }

    bool isPointShadingPerVertex() const
    {
        return mDrawMode0 & VCL_MRS_POINTS_SHADING_VERT;
    }

    bool isPointColorPerVertex() const
    {
        return mDrawMode0 & VCL_MRS_POINTS_COLOR_VERTEX;
    }

    bool isPointColorPerMesh() const
    {
        return mDrawMode0 & VCL_MRS_POINTS_COLOR_MESH;
    }

    bool isPointColorUserDefined() const
    {
        return mDrawMode0 & VCL_MRS_POINTS_COLOR_USER;
    }

    float pointWidth() const { return mPointWidth; }

    vcl::Color pointUserColor() const;

    const float* pointUserColorData() const { return mPointUserColor; }

    bool isSurfaceVisible() const { return mDrawMode0 & VCL_MRS_DRAW_SURF; }

    bool isSurfaceShadingNone() const
    {
        return mDrawMode0 & VCL_MRS_SURF_SHADING_NONE;
    }

    bool isSurfaceShadingFlat() const
    {
        return mDrawMode0 & VCL_MRS_SURF_SHADING_FLAT;
    }

    bool isSurfaceShadingSmooth() const
    {
        return mDrawMode0 & VCL_MRS_SURF_SHADING_SMOOTH;
    }

    bool isSurfaceColorPerFace() const
    {
        return mDrawMode0 & VCL_MRS_SURF_COLOR_FACE;
    }

    bool isSurfaceColorPerVertex() const
    {
        return mDrawMode0 & VCL_MRS_SURF_COLOR_VERTEX;
    }

    bool isSurfaceColorPerMesh() const
    {
        return mDrawMode0 & VCL_MRS_SURF_COLOR_MESH;
    }

    bool isSurfaceColorUserDefined() const
    {
        return mDrawMode0 & VCL_MRS_SURF_COLOR_USER;
    }

    bool isSurfaceColorPerVertexTexcoords() const
    {
        return mDrawMode0 & VCL_MRS_SURF_TEX_VERTEX;
    }

    bool isSurfaceColorPerWedgeTexcoords() const
    {
        return mDrawMode0 & VCL_MRS_SURF_TEX_WEDGE;
    }

    vcl::Color surfaceUserColor() const;

    const uint* surfaceUserColorData() const { return &mSurfUserColor; }

    bool isWireframeVisible() const
    {
        return mDrawMode0 & VCL_MRS_DRAW_WIREFRAME;
    }

    bool isWireframeShadingNone() const
    {
        return mDrawMode0 & VCL_MRS_WIREFRAME_SHADING_NONE;
    }

    bool isWireframeShadingPerVertex() const
    {
        return mDrawMode0 & VCL_MRS_WIREFRAME_SHADING_VERT;
    }

    bool isWireframeColorPerVertex() const
    {
        return mDrawMode0 & VCL_MRS_WIREFRAME_COLOR_VERT;
    }

    bool isWireframeColorPerMesh() const
    {
        return mDrawMode0 & VCL_MRS_WIREFRAME_COLOR_MESH;
    }

    bool isWireframeColorUserDefined() const
    {
        return mDrawMode0 & VCL_MRS_WIREFRAME_COLOR_USER;
    }

    int wireframeWidth() const { return mWrfWidth; }

    vcl::Color wireframeUserColor() const;

    const float* wireframeUserColorData() const { return mWrfUserColor; }

    bool isBboxEnabled() const { return mDrawMode0 & VCL_MRS_DRAW_BOUNDINGBOX; }

    bool isEdgesVisible() const { return mDrawMode1 & VCL_MRS_DRAW_EDGES; }

    bool isEdgesShadingNone() const
    {
        return mDrawMode1 & VCL_MRS_EDGES_SHADING_NONE;
    }

    bool isEdgesShadingSmooth() const
    {
        return mDrawMode1 & VCL_MRS_EDGES_SHADING_SMOOTH;
    }

    bool isEdgesShadingFlat() const
    {
        return mDrawMode1 & VCL_MRS_EDGES_SHADING_FLAT;
    }

    bool isEdgesColorPerVertex() const
    {
        return mDrawMode1 & VCL_MRS_EDGES_COLOR_VERTEX;
    }

    bool isEdgesColorPerEdge() const
    {
        return mDrawMode1 & VCL_MRS_EDGES_COLOR_EDGE;
    }

    bool isEdgesColorPerMesh() const
    {
        return mDrawMode1 & VCL_MRS_EDGES_COLOR_MESH;
    }

    bool isEdgesColorUserDefined() const
    {
        return mDrawMode1 & VCL_MRS_EDGES_COLOR_USER;
    }

    int edgesWidth() const { return mEdgesWidth; }

    vcl::Color edgesUserColor() const;

    const uint* edgesUserColorData() const { return &mEdgesUserColor; }

    // rendering options setters

    bool setVisibility(bool b);

    bool setPointVisibility(bool b);

    bool setPointShadingNone();

    bool setPointShadingPerVertex();

    bool setPointColorPerVertex();

    bool setPointColorPerMesh();

    bool setPointColorUserDefined();

    bool setPointWidth(float width);

    bool setPointUserColor(float r, float g, float b, float a = 1);

    bool setPointUserColor(const vcl::Color& c);

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

    bool setEdgesVisibility(bool b);

    bool setEdgesShadingNone();

    bool setEdgesShadingSmooth();

    bool setEdgesShadingFlat();

    bool setEdgesColorPerVertex();

    bool setEdgesColorPerEdge();

    bool setEdgesColorPerMesh();

    bool setEdgesColorUserDefined();

    bool setEdgesWidth(int width);

    bool setEdgesUserColor(float r, float g, float b, float a = 1);

    bool setEdgesUserColor(const vcl::Color& c);

    template<MeshConcept MeshType>
    void setRenderCapabilityFrom(const MeshType& m)
    {
        mDrawModeCapability0 = 0;

        if (m.vertexNumber() > 0) {
            mDrawModeCapability0 |= VCL_MRS_DRAW_MESH;

            // -- Points --
            mDrawModeCapability0 |= VCL_MRS_DRAW_POINTS;
            mDrawModeCapability0 |= VCL_MRS_POINTS_SHADING_NONE;
            mDrawModeCapability0 |= VCL_MRS_POINTS_PIXEL;
            mDrawModeCapability0 |= VCL_MRS_POINTS_CIRCLE;
            mDrawModeCapability0 |= VCL_MRS_POINTS_COLOR_USER;

            if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                if (vcl::isPerVertexNormalAvailable(m)) {
                    mDrawModeCapability0 |= VCL_MRS_POINTS_SHADING_VERT;
                }
            }

            if constexpr (vcl::HasPerVertexColor<MeshType>) {
                if (vcl::isPerVertexColorAvailable(m)) {
                    mDrawModeCapability0 |= VCL_MRS_POINTS_COLOR_VERTEX;
                }
            }

            if constexpr (vcl::HasColor<MeshType>) {
                mDrawModeCapability0 |= VCL_MRS_POINTS_COLOR_MESH;
            }

            // -- Surface and Wireframe --
            if constexpr (vcl::HasFaces<MeshType>) {
                if (m.faceNumber() > 0) {
                    mDrawModeCapability0 |= VCL_MRS_DRAW_SURF;
                    mDrawModeCapability0 |= VCL_MRS_SURF_SHADING_NONE;
                    mDrawModeCapability0 |= VCL_MRS_SURF_COLOR_USER;
                    mDrawModeCapability0 |= VCL_MRS_DRAW_WIREFRAME;
                    mDrawModeCapability0 |= VCL_MRS_WIREFRAME_SHADING_NONE;
                    mDrawModeCapability0 |= VCL_MRS_WIREFRAME_COLOR_USER;

                    if constexpr (vcl::HasColor<MeshType>) {
                        mDrawModeCapability0 |= VCL_MRS_SURF_COLOR_MESH;
                        mDrawModeCapability0 |= VCL_MRS_WIREFRAME_COLOR_MESH;
                    }

                    if constexpr (vcl::HasPerFaceNormal<MeshType>) {
                        if (vcl::isPerFaceNormalAvailable(m)) {
                            mDrawModeCapability0 |= VCL_MRS_SURF_SHADING_FLAT;
                        }
                    }

                    if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                        if (vcl::isPerVertexNormalAvailable(m)) {
                            mDrawModeCapability0 |= VCL_MRS_SURF_SHADING_SMOOTH;
                            mDrawModeCapability0 |=
                                VCL_MRS_WIREFRAME_SHADING_VERT;
                        }
                    }

                    if constexpr (vcl::HasPerFaceColor<MeshType>) {
                        if (vcl::isPerFaceColorAvailable(m))
                            mDrawModeCapability0 |= VCL_MRS_SURF_COLOR_FACE;
                    }

                    if constexpr (vcl::HasPerVertexColor<MeshType>) {
                        if (vcl::isPerVertexColorAvailable(m)) {
                            mDrawModeCapability0 |= VCL_MRS_SURF_COLOR_VERTEX;
                            mDrawModeCapability0 |=
                                VCL_MRS_WIREFRAME_COLOR_VERT;
                        }
                    }

                    if constexpr (vcl::HasTexturePaths<MeshType>) {
                        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
                            if (vcl::isPerVertexTexCoordAvailable(m))
                                mDrawModeCapability0 |= VCL_MRS_SURF_TEX_VERTEX;
                        }

                        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                            if (vcl::isPerFaceWedgeTexCoordsAvailable(m))
                                mDrawModeCapability0 |= VCL_MRS_SURF_TEX_WEDGE;
                        }
                    }
                }
            }

            // -- Bounding Box --
            if constexpr (vcl::HasBoundingBox<MeshType>) {
                mDrawModeCapability0 |= VCL_MRS_DRAW_BOUNDINGBOX;
            }

            // -- Edges --
            if constexpr (vcl::HasEdges<MeshType>) {
                if (m.edgeNumber() > 0) {
                    mDrawModeCapability1 |= VCL_MRS_DRAW_EDGES;
                    mDrawModeCapability1 |= VCL_MRS_EDGES_SHADING_NONE;
                    mDrawModeCapability1 |= VCL_MRS_EDGES_COLOR_USER;

                    if constexpr (vcl::HasColor<MeshType>) {
                        mDrawModeCapability1 |= VCL_MRS_EDGES_COLOR_MESH;
                    }

                    if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                        if (vcl::isPerVertexNormalAvailable(m)) {
                            mDrawModeCapability1 |=
                                VCL_MRS_EDGES_SHADING_SMOOTH;
                        }
                    }

                    if constexpr (vcl::HasPerEdgeNormal<MeshType>) {
                        if (vcl::isPerEdgeNormalAvailable(m)) {
                            mDrawModeCapability1 |= VCL_MRS_EDGES_SHADING_FLAT;
                        }
                    }

                    if constexpr (vcl::HasPerEdgeColor<MeshType>) {
                        if (vcl::isPerEdgeColorAvailable(m))
                            mDrawModeCapability1 |= VCL_MRS_EDGES_COLOR_EDGE;
                    }

                    if constexpr (vcl::HasPerVertexColor<MeshType>) {
                        if (vcl::isPerVertexColorAvailable(m)) {
                            mDrawModeCapability1 |= VCL_MRS_EDGES_COLOR_VERTEX;
                        }
                    }
                }
            }
        }

        // make sure that the previous draw mode satisfies the new capabilites
        mDrawMode0 &= mDrawModeCapability0;
        mDrawMode1 &= mDrawModeCapability1;
    }

    void setDefaultSettingsFromCapability();
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_SETTINGS_H
