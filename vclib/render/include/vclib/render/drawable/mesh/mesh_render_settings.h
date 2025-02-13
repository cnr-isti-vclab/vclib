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

#include "mesh_render_info.h"

#include <vclib/mesh/requirements.h>
#include <vclib/space/core/bit_set.h>
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
    using MRI = MeshRenderInfo;

    MeshRenderInfo mCapability; // capabilities of the mesh
    MeshRenderInfo mDrawMode; // current rendering settings

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

    bool operator==(const MeshRenderSettings&) const = default;

    bool operator!=(const MeshRenderSettings&) const = default;

    bool canBeVisible() const
    {
        return mCapability.visible();
    }

    template<MeshRenderInfo::Primitive PRIMITIVE, typename Enum>
    bool can(Enum val) const
    {
        assert(val < Enum::COUNT);
        return mCapability.settings<PRIMITIVE>()[toUnderlying(val)];
    }

    bool canPoints(MeshRenderInfo::Points p) const
    {
        return can<MRI::Primitive::POINTS>(p);
    }

    bool canSurface(MeshRenderInfo::Surface s) const
    {
        return can<MRI::Primitive::SURFACE>(s);
    }

    bool canWireframe(MeshRenderInfo::Wireframe w) const
    {
        return can<MRI::Primitive::WIREFRAME>(w);
    }

    bool canEdges(MeshRenderInfo::Edges e) const
    {
        return can<MRI::Primitive::EDGES>(e);
    }

    // rendering options getters

    MeshRenderInfo drawMode() const
    {
        return mDrawMode;
    }

    template<MeshRenderInfo::Primitive PRIMITIVE, typename Enum>
    bool is(Enum val) const
    {
        assert(val < Enum::COUNT);
        return mDrawMode.settings<PRIMITIVE>()[toUnderlying(val)];
    }

    bool isVisible() const { return mDrawMode.visible(); }

    bool isPoint(MeshRenderInfo::Points p) const
    {
        return is<MRI::Primitive::POINTS>(p);
    }

    bool isPointVisible() const
    {
        return isPoint(MRI::Points::VISIBLE);
    }

    bool isPointShadingNone() const
    {
        return isPoint(MRI::Points::SHADING_NONE);
    }

    bool isPointShadingPerVertex() const
    {
        return isPoint(MRI::Points::SHADING_VERT);
    }

    bool isPointColorPerVertex() const
    {
        return isPoint(MRI::Points::COLOR_VERTEX);
    }

    bool isPointColorPerMesh() const
    {
        return isPoint(MRI::Points::COLOR_MESH);
    }

    bool isPointColorUserDefined() const
    {
        return isPoint(MRI::Points::COLOR_USER);
    }

    float pointWidth() const { return mPointWidth; }

    vcl::Color pointUserColor() const;

    const float* pointUserColorData() const { return mPointUserColor; }

    bool isSurface(MeshRenderInfo::Surface s) const
    {
        return is<MRI::Primitive::SURFACE>(s);
    }

    bool isSurfaceVisible() const
    {
        return isSurface(MRI::Surface::VISIBLE);
    }

    bool isSurfaceShadingNone() const
    {
        return isSurface(MRI::Surface::SHADING_NONE);
    }

    bool isSurfaceShadingFlat() const
    {
        return isSurface(MRI::Surface::SHADING_FLAT);
    }

    bool isSurfaceShadingSmooth() const
    {
        return isSurface(MeshRenderInfo::Surface::SHADING_SMOOTH);
    }

    bool isSurfaceColorPerFace() const
    {
        return isSurface(MRI::Surface::COLOR_FACE);
    }

    bool isSurfaceColorPerVertex() const
    {
        return isSurface(MRI::Surface::COLOR_VERTEX);
    }

    bool isSurfaceColorPerMesh() const
    {
        return isSurface(MRI::Surface::COLOR_MESH);
    }

    bool isSurfaceColorUserDefined() const
    {
        return isSurface(MRI::Surface::COLOR_USER);
    }

    bool isSurfaceColorPerVertexTexcoords() const
    {
        return isSurface(MRI::Surface::COLOR_VERTEX_TEX);
    }

    bool isSurfaceColorPerWedgeTexcoords() const
    {
        return isSurface(MRI::Surface::COLOR_WEDGE_TEX);
    }

    vcl::Color surfaceUserColor() const;

    const uint* surfaceUserColorData() const { return &mSurfUserColor; }

    bool isWireframe(MeshRenderInfo::Wireframe w) const
    {
        return is<MRI::Primitive::WIREFRAME>(w);
    }

    bool isWireframeVisible() const
    {
        return isWireframe(MRI::Wireframe::VISIBLE);
    }

    bool isWireframeShadingNone() const
    {
        return isWireframe(MRI::Wireframe::SHADING_NONE);
    }

    bool isWireframeShadingPerVertex() const
    {
        return isWireframe(MRI::Wireframe::SHADING_VERT);
    }

    bool isWireframeColorPerVertex() const
    {
        return isWireframe(MRI::Wireframe::COLOR_VERTEX);
    }

    bool isWireframeColorPerMesh() const
    {
        return isWireframe(MRI::Wireframe::COLOR_MESH);
    }

    bool isWireframeColorUserDefined() const
    {
        return isWireframe(MRI::Wireframe::COLOR_USER);
    }

    int wireframeWidth() const { return mWrfWidth; }

    vcl::Color wireframeUserColor() const;

    const float* wireframeUserColorData() const { return mWrfUserColor; }

    bool isEdges(MeshRenderInfo::Edges e) const
    {
        return is<MRI::Primitive::EDGES>(e);
    }

    bool isEdgesVisible() const
    {
        return isEdges(MRI::Edges::VISIBLE);
    }

    bool isEdgesShadingNone() const
    {
        return isEdges(MRI::Edges::SHADING_NONE);
    }

    bool isEdgesShadingSmooth() const
    {
        return isEdges(MRI::Edges::SHADING_SMOOTH);
    }

    bool isEdgesShadingFlat() const
    {
        return isEdges(MRI::Edges::SHADING_FLAT);
    }

    bool isEdgesColorPerVertex() const
    {
        return isEdges(MRI::Edges::COLOR_VERTEX);
    }

    bool isEdgesColorPerEdge() const
    {
        return isEdges(MRI::Edges::COLOR_EDGE);
    }

    bool isEdgesColorPerMesh() const
    {
        return isEdges(MRI::Edges::COLOR_MESH);
    }

    bool isEdgesColorUserDefined() const
    {
        return isEdges(MRI::Edges::COLOR_USER);
    }

    int edgesWidth() const { return mEdgesWidth; }

    vcl::Color edgesUserColor() const;

    const uint* edgesUserColorData() const { return &mEdgesUserColor; }

    // rendering options setters

    bool setVisibility(bool b);

    template<MeshRenderInfo::Primitive PRIMITIVE, typename Enum>
    bool set(Enum val, bool b = true)
    {
        if (can<PRIMITIVE>(val)) { // if the capability allows it
            // get the range of the mutual exclusive settings for val
            auto rng = MRI::exclusiveRange<PRIMITIVE>(val);
            // if there are no mutual exclusive settings
            if (rng.first == rng.second) {
                // the setting could be true or false
                // e.g. VISIBLE
                mDrawMode.settings<PRIMITIVE>()[rng.first] = b;
            }
            else {
                // only one setting in the range can be true
                // e.g. the range SHADING_*
                for (auto i = rng.first; i <= rng.second; ++i) {
                    mDrawMode.settings<PRIMITIVE>()[i] = toUnderlying(val) == i;
                }
            }
            return true;
        }
        else {
            return false;
        }
    }

    bool setPoint(MeshRenderInfo::Points p, bool b = true)
    {
        return set<MRI::Primitive::POINTS>(p, b);
    }

    bool setPointVisibility(bool b)
    {
        return setPoint(MRI::Points::VISIBLE, b);
    }

    bool setPointShadingNone()
    {
        return setPoint(MRI::Points::SHADING_NONE);
    }

    bool setPointShadingPerVertex()
    {
        return setPoint(MRI::Points::SHADING_VERT);
    }

    bool setPointColorPerVertex()
    {
        return setPoint(MRI::Points::COLOR_VERTEX);
    }

    bool setPointColorPerMesh()
    {
        return setPoint(MRI::Points::COLOR_MESH);
    }

    bool setPointColorUserDefined()
    {
        return setPoint(MRI::Points::COLOR_USER);
    }

    bool setPointWidth(float width);

    bool setPointUserColor(float r, float g, float b, float a = 1);

    bool setPointUserColor(const vcl::Color& c);

    bool setSurface(MeshRenderInfo::Surface s, bool b = true)
    {
        return set<MRI::Primitive::SURFACE>(s, b);
    }

    bool setSurfaceVisibility(bool b)
    {
        return setSurface(MRI::Surface::VISIBLE, b);
    }

    /**
     * @brief Unsets the shading of the surface (no light).
     * Unsets automatically all the other shading options.
     */
    bool setSurfaceShadingNone()
    {
        return setSurface(MRI::Surface::SHADING_NONE);
    }

    /**
     * @brief Sets (if capability allows it) the shading of the surface flat
     * (using triangle normals). Unsets automatically all the other shading
     * options.
     */
    bool setSurfaceShadingFlat()
    {
        return setSurface(MRI::Surface::SHADING_FLAT);
    }

    /**
     * @brief Sets (if capability allows it) the shading of the surface smooth
     * (using vertex normals). Unsets automatically all the other shading
     * options.
     */
    bool setSurfaceShadingSmooth()
    {
        return setSurface(MRI::Surface::SHADING_SMOOTH);
    }

    /**
     * @brief Set (if capability allows it) the surface coloring per vertex
     * (using the vertex colors). Unsets automatically all the other surface
     * colorizations.
     */
    bool setSurfaceColorPerVertex()
    {
        return setSurface(MRI::Surface::COLOR_VERTEX);
    }

    /**
     * @brief Set (if capability allows it) the surface coloring per face (using
     * the face colors). Unsets automatically all the other surface
     * colorizations.
     */
    bool setSurfaceColorPerFace()
    {
        return setSurface(MRI::Surface::COLOR_FACE);
    }

    /**
     * @brief Set (if capability allows it) the surface coloring per mesh (using
     * the mesh color). Unsets automatically all the other surface
     * colorizations.
     */
    bool setSurfaceColorPerMesh()
    {
        return setSurface(MRI::Surface::COLOR_MESH);
    }

    /**
     * @brief Set the surface coloring by the user defined color.
     * To set the user defined color, you can use the
     * @ref setSurfaceColorUserDefined() function. Unsets automatically all the
     * other surface colorizations.
     */
    bool setSurfaceColorUserDefined()
    {
        return setSurface(MRI::Surface::COLOR_USER);
    }

    /**
     * @brief Set (if capability allows it) the surface color using the per
     * vertex texcoords. Unsets automatically all the other surface
     * colorizations.
     */
    bool setSurfaceColorPerVertexTexcoords()
    {
        return setSurface(MRI::Surface::COLOR_VERTEX_TEX);
    }

    /**
     * @brief Set (if capability allows it) the surface color using the per
     * wedge texcoords. Unsets automatically all the other surface
     * colorizations.
     */
    bool setSurfaceColorPerWedgeTexcoords()
    {
        return setSurface(MRI::Surface::COLOR_WEDGE_TEX);
    }

    bool setSurfaceUserColor(float r, float g, float b, float a = 1);

    bool setSurfaceUserColor(const vcl::Color& c);

    bool setWireframe(MeshRenderInfo::Wireframe w, bool b = true)
    {
        return set<MRI::Primitive::WIREFRAME>(w, b);
    }

    bool setWireframeVisibility(bool b)
    {
        return setWireframe(MRI::Wireframe::VISIBLE, b);
    }

    bool setWireframeShadingNone()
    {
        return setWireframe(MRI::Wireframe::SHADING_NONE);
    }

    bool setWireframeShadingPerVertex()
    {
        return setWireframe(MRI::Wireframe::SHADING_VERT);
    }

    bool setWireframeColorPerVertex()
    {
        return setWireframe(MRI::Wireframe::COLOR_VERTEX);
    }

    bool setWireframeColorPerMesh()
    {
        return setWireframe(MRI::Wireframe::COLOR_MESH);
    }

    bool setWireframeColorUserDefined()
    {
        return setWireframe(MRI::Wireframe::COLOR_USER);
    }

    bool setWireframeUserColor(float r, float g, float b, float a = 1);

    bool setWireframeUserColor(const vcl::Color& c);

    bool setWireframeWidth(int width);

    bool setEdges(MeshRenderInfo::Edges e, bool b = true)
    {
        return set<MRI::Primitive::EDGES>(e, b);
    }

    bool setEdgesVisibility(bool b)
    {
        return setEdges(MRI::Edges::VISIBLE, b);
    }

    bool setEdgesShadingNone()
    {
        return setEdges(MRI::Edges::SHADING_NONE);
    }

    bool setEdgesShadingSmooth()
    {
        return setEdges(MRI::Edges::SHADING_SMOOTH);
    }

    bool setEdgesShadingFlat()
    {
        return setEdges(MRI::Edges::SHADING_FLAT);
    }

    bool setEdgesColorPerVertex()
    {
        return setEdges(MRI::Edges::COLOR_VERTEX);
    }

    bool setEdgesColorPerEdge()
    {
        return setEdges(MRI::Edges::COLOR_EDGE);
    }

    bool setEdgesColorPerMesh()
    {
        return setEdges(MRI::Edges::COLOR_MESH);
    }

    bool setEdgesColorUserDefined()
    {
        return setEdges(MRI::Edges::COLOR_USER);
    }

    bool setEdgesUserColor(float r, float g, float b, float a = 1);

    bool setEdgesUserColor(const vcl::Color& c);

    bool setEdgesWidth(int width);

    template<MeshConcept MeshType>
    void setRenderCapabilityFrom(const MeshType& m)
    {
        mCapability.reset();

        if (m.vertexNumber() > 0) {
            mCapability.visible() = true;

            // -- Points --
            setPointsCapability(MRI::Points::VISIBLE);
            setPointsCapability(MRI::Points::SHAPE_PIXEL);
            setPointsCapability(MRI::Points::SHAPE_CIRCLE);
            setPointsCapability(MRI::Points::SHADING_NONE);
            setPointsCapability(MRI::Points::COLOR_USER);

            if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                if (vcl::isPerVertexNormalAvailable(m)) {
                    setPointsCapability(MRI::Points::SHADING_VERT);
                }
            }

            if constexpr (vcl::HasPerVertexColor<MeshType>) {
                if (vcl::isPerVertexColorAvailable(m)) {
                    setPointsCapability(MRI::Points::COLOR_VERTEX);
                }
            }

            if constexpr (vcl::HasColor<MeshType>) {
                setPointsCapability(MRI::Points::COLOR_MESH);
            }

            // -- Surface and Wireframe --
            if constexpr (vcl::HasFaces<MeshType>) {
                if (m.faceNumber() > 0) {
                    setSurfaceCapability(MRI::Surface::VISIBLE);
                    setSurfaceCapability(MRI::Surface::SHADING_NONE);
                    setSurfaceCapability(MRI::Surface::COLOR_USER);
                    setWireframeCapability(MRI::Wireframe::VISIBLE);
                    setWireframeCapability(MRI::Wireframe::SHADING_NONE);
                    setWireframeCapability(MRI::Wireframe::COLOR_USER);

                    if constexpr (vcl::HasColor<MeshType>) {
                        setSurfaceCapability(MRI::Surface::COLOR_MESH);
                        setWireframeCapability(MRI::Wireframe::COLOR_MESH);
                    }

                    if constexpr (vcl::HasPerFaceNormal<MeshType>) {
                        if (vcl::isPerFaceNormalAvailable(m)) {
                            setSurfaceCapability(MRI::Surface::SHADING_FLAT);
                        }
                    }

                    if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                        if (vcl::isPerVertexNormalAvailable(m)) {
                            setSurfaceCapability(MRI::Surface::SHADING_SMOOTH);
                            setWireframeCapability(MRI::Wireframe::SHADING_VERT);
                        }
                    }

                    if constexpr (vcl::HasPerFaceColor<MeshType>) {
                        if (vcl::isPerFaceColorAvailable(m))
                            setSurfaceCapability(MRI::Surface::COLOR_FACE);
                    }

                    if constexpr (vcl::HasPerVertexColor<MeshType>) {
                        if (vcl::isPerVertexColorAvailable(m)) {
                            setSurfaceCapability(MRI::Surface::COLOR_VERTEX);
                            setWireframeCapability(MRI::Wireframe::COLOR_VERTEX);
                        }
                    }

                    if constexpr (vcl::HasTexturePaths<MeshType>) {
                        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
                            if (vcl::isPerVertexTexCoordAvailable(m))
                                setSurfaceCapability(MRI::Surface::COLOR_VERTEX_TEX);
                        }

                        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                            if (vcl::isPerFaceWedgeTexCoordsAvailable(m))
                                setSurfaceCapability(MRI::Surface::COLOR_WEDGE_TEX);
                        }
                    }
                }
            }

            // -- Edges --
            if constexpr (vcl::HasEdges<MeshType>) {
                if (m.edgeNumber() > 0) {
                    setEdgesCapability(MRI::Edges::VISIBLE);
                    setEdgesCapability(MRI::Edges::SHADING_NONE);
                    setEdgesCapability(MRI::Edges::COLOR_USER);

                    if constexpr (vcl::HasColor<MeshType>) {
                        setEdgesCapability(MRI::Edges::COLOR_MESH);
                    }

                    if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                        if (vcl::isPerVertexNormalAvailable(m)) {
                            setEdgesCapability(MRI::Edges::SHADING_SMOOTH);
                        }
                    }

                    if constexpr (vcl::HasPerEdgeNormal<MeshType>) {
                        if (vcl::isPerEdgeNormalAvailable(m)) {
                            setEdgesCapability(MRI::Edges::SHADING_FLAT);
                        }
                    }

                    if constexpr (vcl::HasPerEdgeColor<MeshType>) {
                        if (vcl::isPerEdgeColorAvailable(m))
                            setEdgesCapability(MRI::Edges::COLOR_EDGE);
                    }

                    if constexpr (vcl::HasPerVertexColor<MeshType>) {
                        if (vcl::isPerVertexColorAvailable(m)) {
                            setEdgesCapability(MRI::Edges::COLOR_VERTEX);
                        }
                    }
                }
            }
        }

        // make sure that the previous draw mode satisfies the new capabilites
        mDrawMode &= mCapability;
    }

    void setDefaultSettingsFromCapability();

private:
    template<MeshRenderInfo::Primitive PRIMITIVE, typename Enum>
    void setCapability(Enum val, bool b = true)
    {
        assert(val < Enum::COUNT);
        mCapability.settings<PRIMITIVE>()[toUnderlying(val)] = b;
    }

    void setPointsCapability(MeshRenderInfo::Points p, bool b = true)
    {
        setCapability<MRI::Primitive::POINTS>(p, b);
    }

    void setSurfaceCapability(MeshRenderInfo::Surface s, bool b = true)
    {
        setCapability<MRI::Primitive::SURFACE>(s, b);
    }

    void setWireframeCapability(MeshRenderInfo::Wireframe w, bool b = true)
    {
        setCapability<MRI::Primitive::WIREFRAME>(w, b);
    }

    void setEdgesCapability(MeshRenderInfo::Edges e, bool b = true)
    {
        setCapability<MRI::Primitive::EDGES>(e, b);
    }

    void setDefaultPointSettingsFromCapability();

    void setDefaultSurfaceSettingsFromCapability();

    void setDefaultWireframeSettingsFromCapability();

    void setDefaultEdgeSettingsFromCapability();
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_SETTINGS_H
