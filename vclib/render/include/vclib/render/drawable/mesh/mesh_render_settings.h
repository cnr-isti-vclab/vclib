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
 * settings of a Mesh. This class stores the rendering status of a Mesh, the
 * rendering capability of a Mesh and a series of other data like user colors
 * used for the mesh primitives.
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
 * order to initialize the render capabilites. Render settings won't be possible
 * to set if this class is not first initialized using a vcl::Mesh, trough the
 * constructor or the `setRenderCapabilityFrom()` member function.
 *
 * Render capabilities can be get by calling the member functions `can*()` for
 * each primitive, and providing the according enum value for the option to
 * query.
 *
 * The memeber functions `is*()` allow to get the current render status for
 * each primitive, providing the according enum value for the option to query.
 *
 * The render status can be modified using the `set*()` member functions for
 * each primitive. They return a boolean indicating if the operation has been
 * performed (if the capabilities allows it, the operation will be always
 * performed).
 *
 * The `set*()` functions are guaranteed to keep the state of the draw mode
 * consistent. This means that if the user sets an option that belongs to a
 * range (e.g. SHADING_*), the other options of the range will be automatically
 * set to false.
 */
class MeshRenderSettings
{
    using MRI = MeshRenderInfo;

    MeshRenderInfo mCapability; // capabilities of the mesh
    MeshRenderInfo mDrawMode;   // current rendering settings

    float mPointWidth        = 3;
    float mPointUserColor[4] = {1, 1, 0, 1}; // TODO: change to uint?
    uint  mSurfUserColor     = 0xFF808080;   // abgr
    int   mWrfWidth          = 1;
    float mWrfUserColor[4]   = {0, 0, 0, 1}; // TODO: change to uint?
    int   mEdgesWidth        = 1;
    uint  mEdgesUserColor    = 0xFF000000; // abgr

public:
    /**
     * @brief Construct a new MeshRenderSettings object with capabilities set to
     * false.
     *
     * An object created with this constructor will not be able to set any
     * rendering option until the `setRenderCapabilityFrom()` member function is
     * called.
     */
    MeshRenderSettings() = default;

    /**
     * @brief Construct a new MeshRenderSettings object from a Mesh.
     *
     * This constructor initializes the render capabilities from the given Mesh
     * object. The render settings are then set to the default values.
     *
     * @tparam MeshType: the type of the Mesh object.
     * @param m: the Mesh object to initialize the render capabilities.
     */
    template<MeshConcept MeshType>
    MeshRenderSettings(const MeshType& m)
    {
        setRenderCapabilityFrom(m);
        setDefaultSettingsFromCapability();
    }

    /**
     * @brief Returns the current draw mode as a MeshRenderInfo object.
     * @return the current draw mode as a MeshRenderInfo object.
     */
    MeshRenderInfo drawMode() const { return mDrawMode; }

    bool operator==(const MeshRenderSettings&) const = default;

    bool operator!=(const MeshRenderSettings&) const = default;

    // rendering option capabilities of the mesh

    /**
     * @brief Returns whether the mesh can be visible.
     * @return `true` if the mesh can be visible, `false` otherwise.
     */
    bool canBeVisible() const { return mCapability.visible(); }

    /**
     * @brief Returns the capability of a given option for the given primitive.
     *
     * The primitive is a template parameter that specifies the primitive to
     * query. The Enum parameter specifies the option to query.
     *
     * @tparam PRIMTIVE: the primitive for which to get the capability.
     *
     * @param[in] val: the option to query.
     * @return `true` if the option can be set, `false` otherwise.
     */
    template<MeshRenderInfo::Primitive PRIMITIVE, typename Enum>
    bool can(Enum val) const
    {
        assert(val < Enum::COUNT);
        return mCapability.settings<PRIMITIVE>()[toUnderlying(val)];
    }

    /**
     * @brief Returns the capability of a given option for the points primitive.
     *
     * @param[in] p: the points option to query.
     * @return `true` if the option can be set, `false` otherwise.
     */
    bool canPoints(MeshRenderInfo::Points p) const
    {
        return can<MRI::Primitive::POINTS>(p);
    }

    /**
     * @brief Returns the capability of a given option for the surface
     * primitive.
     *
     * @param[in] s: the surface option to query.
     * @return `true` if the option can be set, `false` otherwise.
     */
    bool canSurface(MeshRenderInfo::Surface s) const
    {
        return can<MRI::Primitive::SURFACE>(s);
    }

    /**
     * @brief Returns the capability of a given option for the wireframe
     * primitive.
     *
     * @param[in] w: the wireframe option to query.
     * @return `true` if the option can be set, `false` otherwise.
     */
    bool canWireframe(MeshRenderInfo::Wireframe w) const
    {
        return can<MRI::Primitive::WIREFRAME>(w);
    }

    /**
     * @brief Returns the capability of a given option for the edges primitive.
     *
     * @param[in] e: the edges option to query.
     * @return `true` if the option can be set, `false` otherwise.
     */
    bool canEdges(MeshRenderInfo::Edges e) const
    {
        return can<MRI::Primitive::EDGES>(e);
    }

    // rendering option getters

    /**
     * @brief Returns whether the mesh is visible.
     * @return `true` if the mesh is visible, `false` otherwise.
     */
    bool isVisible() const { return mDrawMode.visible(); }

    /**
     * @brief Returns whether the given option for the given primitive is set.
     *
     * The primitive is a template parameter that specifies the primitive to
     * query. The Enum parameter specifies the option to query.
     *
     * @tparam PRIMTIVE: the primitive for which to get the option.
     *
     * @param[in] val: the option to query.
     * @return `true` if the option is set, `false` otherwise.
     */
    template<MeshRenderInfo::Primitive PRIMITIVE, typename Enum>
    bool is(Enum val) const
    {
        assert(val < Enum::COUNT);
        return mDrawMode.settings<PRIMITIVE>()[toUnderlying(val)];
    }

    /**
     * @brief Returns whether the given points option is set.
     *
     * @param[in] p: the points option to query.
     * @return `true` if the option is set, `false` otherwise.
     */
    bool isPoints(MeshRenderInfo::Points p) const
    {
        return is<MRI::Primitive::POINTS>(p);
    }

    float pointWidth() const { return mPointWidth; }

    vcl::Color pointUserColor() const
    {
        vcl::Color c;
        c.setRedF(mPointUserColor[0]);
        c.setGreenF(mPointUserColor[1]);
        c.setBlueF(mPointUserColor[2]);
        c.setAlphaF(mPointUserColor[3]);
        return c;
    }

    const float* pointUserColorData() const { return mPointUserColor; }

    /**
     * @brief Returns whether the given surface option is set.
     *
     * @param[in] s: the surface option to query.
     * @return `true` if the option is set, `false` otherwise.
     */
    bool isSurface(MeshRenderInfo::Surface s) const
    {
        return is<MRI::Primitive::SURFACE>(s);
    }

    vcl::Color surfaceUserColor() const
    {
        vcl::Color c;
        c.setAbgr(mSurfUserColor);
        return c;
    }

    const uint* surfaceUserColorData() const { return &mSurfUserColor; }

    /**
     * @brief Returns whether the given wireframe option is set.
     *
     * @param[in] w: the wireframe option to query.
     * @return `true` if the option is set, `false` otherwise.
     */
    bool isWireframe(MeshRenderInfo::Wireframe w) const
    {
        return is<MRI::Primitive::WIREFRAME>(w);
    }

    int wireframeWidth() const { return mWrfWidth; }

    vcl::Color wireframeUserColor() const
    {
        vcl::Color c;
        c.setRedF(mWrfUserColor[0]);
        c.setGreenF(mWrfUserColor[1]);
        c.setBlueF(mWrfUserColor[2]);
        c.setAlphaF(mWrfUserColor[3]);
        return c;
    }

    const float* wireframeUserColorData() const { return mWrfUserColor; }

    /**
     * @brief Returns whether the given edges option is set.
     *
     * @param[in] e: the edges option to query.
     * @return `true` if the option is set, `false` otherwise.
     */
    bool isEdges(MeshRenderInfo::Edges e) const
    {
        return is<MRI::Primitive::EDGES>(e);
    }

    int edgesWidth() const { return mEdgesWidth; }

    vcl::Color edgesUserColor() const
    {
        vcl::Color c;
        c.setAbgr(mEdgesUserColor);
        return c;
    }

    const uint* edgesUserColorData() const { return &mEdgesUserColor; }

    // rendering option setters

    /**
     * @brief Sets the visibility of the mesh.
     *
     * The visibility status can be set only if the capability allows it.
     *
     * @param[in] b: the visibility status to set.
     * @return `true` if the visibility status has been set, `false` otherwise.
     */
    bool setVisibility(bool b)
    {
        if (canBeVisible()) {
            mDrawMode.visible() = b;
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * @brief Sets given the shading option of the given primitive.
     *
     * The primitive is a template parameter that specifies the primitive to
     * query. The Enum parameter specifies the option to query.
     *
     * If the capability does not allow to set the given option, the function
     * does nothing and it returns `false`. Otherwise, it returns `true`.
     *
     * If the option does not have a range (e.g. VISIBLE), the @param b
     * parameter will be used to set the option to the primitive.
     *
     * If the option has a range (e.g. the range SHADING_*), the @param b is not
     * considered. The given option is set to `true`, and all the other options
     * of the range are automatically set to `false`.
     *
     * @tparam PRIMTIVE: the primitive for which to set the option.
     *
     * @param[in] val: the primitive option to set
     * @param[in] b: boolean value to set to the option (if it is not a range).
     * @return `true` if the option has been set, false otherwise.
     */
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

    /**
     * @brief Sets the given shading option of the points.
     *
     * If the capability does not allow to set the given option, the function
     * does nothing and it returns `false`. Otherwise, it returns `true`.
     *
     * If the option does not have a range (e.g. VISIBLE), the @param b
     * parameter will be used to set the option to the points.
     *
     * If the option has a range (e.g. the range SHADING_*), the @param b is not
     * considered. The given option is set to `true`, and all the other options
     * of the range are automatically set to `false`.
     *
     * @param[in] p: the points option to set
     * @param[in] b: boolean value to set to the option (if it is not a range).
     * @return `true` if the option has been set, false otherwise.
     */
    bool setPoints(MeshRenderInfo::Points p, bool b = true)
    {
        return set<MRI::Primitive::POINTS>(p, b);
    }

    bool setPointsWidth(float width)
    {
        if (canPoints(MRI::Points::VISIBLE)) {
            mPointWidth = width;
            return true;
        }
        else {
            return false;
        }
    }

    bool setPointsUserColor(float r, float g, float b, float a = 1)
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

    bool setPointsUserColor(const vcl::Color& c)
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

    /**
     * @brief Sets the given shading option of the surface.
     *
     * If the capability does not allow to set the given option, the function
     * does nothing and it returns `false`. Otherwise, it returns `true`.
     *
     * If the option does not have a range (e.g. VISIBLE), the @param b
     * parameter will be used to set the option to the surface.
     *
     * If the option has a range (e.g. the range SHADING_*), the @param b is not
     * considered. The given option is set to `true`, and all the other options
     * of the range are automatically set to `false`.
     *
     * @param[in] s: the surface option to set
     * @param[in] b: boolean value to set to the option (if it is not a range).
     * @return `true` if the option has been set, false otherwise.
     */
    bool setSurface(MeshRenderInfo::Surface s, bool b = true)
    {
        return set<MRI::Primitive::SURFACE>(s, b);
    }

    bool setSurfaceUserColor(float r, float g, float b, float a = 1)
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

    bool setSurfaceUserColor(const vcl::Color& c)
    {
        if (canSurface(MRI::Surface::VISIBLE)) {
            mSurfUserColor = c.abgr();
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * @brief Sets the given shading option of the wireframe.
     *
     * If the capability does not allow to set the given option, the function
     * does nothing and it returns `false`. Otherwise, it returns `true`.
     *
     * If the option does not have a range (e.g. VISIBLE), the @param b
     * parameter will be used to set the option to the wireframe.
     *
     * If the option has a range (e.g. the range SHADING_*), the @param b is not
     * considered. The given option is set to `true`, and all the other options
     * of the range are automatically set to `false`.
     *
     * @param[in] w: the wireframe option to set
     * @param[in] b: boolean value to set to the option (if it is not a range).
     * @return `true` if the option has been set, false otherwise.
     */
    bool setWireframe(MeshRenderInfo::Wireframe w, bool b = true)
    {
        return set<MRI::Primitive::WIREFRAME>(w, b);
    }

    bool setWireframeUserColor(float r, float g, float b, float a = 1)
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

    bool setWireframeUserColor(const vcl::Color& c)
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

    bool setWireframeWidth(int width)
    {
        if (canWireframe(MRI::Wireframe::VISIBLE)) {
            mWrfWidth = width;
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * @brief Sets the given shading option of the edges.
     *
     * If the capability does not allow to set the given option, the function
     * does nothing and it returns `false`. Otherwise, it returns `true`.
     *
     * If the option does not have a range (e.g. VISIBLE), the @param b
     * parameter will be used to set the option to the edges.
     *
     * If the option has a range (e.g. the range SHADING_*), the @param b is not
     * considered. The given option is set to `true`, and all the other options
     * of the range are automatically set to `false`.
     *
     * @param[in] e: the edges option to set
     * @param[in] b: boolean value to set to the option (if it is not a range).
     * @return `true` if the option has been set, false otherwise.
     */
    bool setEdges(MeshRenderInfo::Edges e, bool b = true)
    {
        return set<MRI::Primitive::EDGES>(e, b);
    }

    bool setEdgesUserColor(float r, float g, float b, float a = 1)
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

    bool setEdgesUserColor(const vcl::Color& c)
    {
        if (canEdges(MRI::Edges::VISIBLE)) {
            mEdgesUserColor = c.abgr();
            return true;
        }
        else {
            return false;
        }
    }

    bool setEdgesWidth(int width)
    {
        if (canEdges(MRI::Edges::VISIBLE)) {
            mEdgesWidth = width;
            return true;
        }
        else {
            return false;
        }
    }

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
            setPointsCapability(MRI::Points::SHAPE_SPHERE);
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
                            setWireframeCapability(
                                MRI::Wireframe::SHADING_VERT);
                        }
                    }

                    if constexpr (vcl::HasPerFaceColor<MeshType>) {
                        if (vcl::isPerFaceColorAvailable(m))
                            setSurfaceCapability(MRI::Surface::COLOR_FACE);
                    }

                    if constexpr (vcl::HasPerVertexColor<MeshType>) {
                        if (vcl::isPerVertexColorAvailable(m)) {
                            setSurfaceCapability(MRI::Surface::COLOR_VERTEX);
                            setWireframeCapability(
                                MRI::Wireframe::COLOR_VERTEX);
                        }
                    }

                    if constexpr (vcl::HasTexturePaths<MeshType>) {
                        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
                            if (vcl::isPerVertexTexCoordAvailable(m) &&
                                m.textureNumber() > 0)
                                setSurfaceCapability(
                                    MRI::Surface::COLOR_VERTEX_TEX);
                        }

                        if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                            if (vcl::isPerFaceWedgeTexCoordsAvailable(m) &&
                                m.textureNumber() > 0)
                                setSurfaceCapability(
                                    MRI::Surface::COLOR_WEDGE_TEX);
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

    void setDefaultSettingsFromCapability()
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

    void setDefaultPointSettingsFromCapability()
    {
        using enum MRI::Points;

        mDrawMode.points().reset();

        if (canPoints(VISIBLE)) {
            if (!canSurface(MRI::Surface::VISIBLE))
                setPoints(VISIBLE, true);
            setPoints(SHADING_NONE);
            setPoints(SHAPE_PIXEL);
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

    void setDefaultSurfaceSettingsFromCapability()
    {
        using enum MRI::Surface;

        mDrawMode.surface().reset();

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
            if (canSurface(COLOR_WEDGE_TEX)) {
                setSurface(COLOR_WEDGE_TEX);
            }
            else if (canSurface(COLOR_VERTEX_TEX)) {
                setSurface(COLOR_VERTEX_TEX);
            }
            else if (canSurface(COLOR_VERTEX)) {
                setSurface(COLOR_VERTEX);
            }
            else if (canSurface(COLOR_FACE)) {
                setSurface(COLOR_FACE);
            }
            // jump mesh color on purpose: it is always available on the mesh,
            // but rarely used and it is likely to be set to 0 (black)
            else {
                setSurface(COLOR_USER);
            }
        }
    }

    void setDefaultWireframeSettingsFromCapability()
    {
        using enum MRI::Wireframe;

        mDrawMode.wireframe().reset();

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

    void setDefaultEdgeSettingsFromCapability()
    {
        using enum MRI::Edges;

        mDrawMode.edges().reset();

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
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_SETTINGS_H
