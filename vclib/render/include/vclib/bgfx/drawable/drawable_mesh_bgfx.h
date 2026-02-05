/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_MESH_BGFX_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_MESH_BGFX_H

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/drawable_environment.h>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers.h>
#include <vclib/bgfx/drawable/uniforms/mesh_render_settings_uniforms.h>
#include <vclib/bgfx/drawers/uniforms/viewer_drawer_uniforms.h>

#include <bgfx/bgfx.h>

namespace vcl {

template<MeshConcept MeshType>
class DrawableMeshBGFX : public AbstractDrawableMesh, public MeshType
{
public:
    // TODO: to be removed after shader benchmarks
    enum class SurfaceProgramsType {
        UBER,
        SPLIT,
        UBER_WITH_STATIC_IF,
    };

private:
    using MRI = MeshRenderInfo;

    mutable DrawableMeshUniforms       mMeshUniforms;
    mutable MaterialUniforms           mMaterialUniforms;
    mutable MeshRenderSettingsUniforms mMeshRenderSettingsUniforms;

    Uniform mIdUniform = Uniform("u_meshId", bgfx::UniformType::Vec4);

    // TODO: to be removed after shader benchmarks
    SurfaceProgramsType mSurfaceProgramType = SurfaceProgramsType::UBER;

    inline static const uint N_TEXTURE_TYPES =
        toUnderlying(Material::TextureType::COUNT);

protected:
    MeshRenderBuffers<MeshType> mMRB;

public:
    DrawableMeshBGFX() = default;

    DrawableMeshBGFX(const MeshType& mesh) :
            AbstractDrawableMesh(mesh), MeshType(mesh)
    {
        updateBuffers();
    }

    DrawableMeshBGFX(MeshType&& mesh) :
            AbstractDrawableMesh(mesh), MeshType(std::move(mesh))
    {
        updateBuffers();
    }

    DrawableMeshBGFX(const DrawableMeshBGFX& drawableMesh) :
            AbstractDrawableMesh((const AbstractDrawableMesh&) drawableMesh),
            MeshType(drawableMesh)
    {
        if constexpr (HasName<MeshType>) {
            AbstractDrawableMesh::name() = drawableMesh.name();
        }
        mMeshRenderSettingsUniforms.updateSettings(mMRS);
        updateBuffers();
    }

    DrawableMeshBGFX(DrawableMeshBGFX&& drawableMesh) { swap(drawableMesh); }

    ~DrawableMeshBGFX() = default;

    DrawableMeshBGFX& operator=(DrawableMeshBGFX drawableMesh)
    {
        swap(drawableMesh);
        return *this;
    }

    void swap(DrawableMeshBGFX& other)
    {
        using std::swap;
        AbstractDrawableMesh::swap(other);
        MeshType::swap(other);
        swap(mMeshUniforms, other.mMeshUniforms);
        swap(mMaterialUniforms, other.mMaterialUniforms);
        swap(mMeshRenderSettingsUniforms, other.mMeshRenderSettingsUniforms);
        swap(mIdUniform, other.mIdUniform);
        swap(mSurfaceProgramType, other.mSurfaceProgramType);
        swap(mMRB, other.mMRB);
    }

    friend void swap(DrawableMeshBGFX& a, DrawableMeshBGFX& b) { a.swap(b); }

    using AbstractDrawableMesh::boundingBox;

    // TODO: to be removed after shader benchmarks
    void setSurfaceProgramType(SurfaceProgramsType type)
    {
        if (type != mSurfaceProgramType) {
            std::cerr << "Program Type changed: ";
            switch (type) {
            case SurfaceProgramsType::UBER: std::cerr << "UBER\n"; break;
            case SurfaceProgramsType::SPLIT: std::cerr << "SPLITTED\n"; break;
            case SurfaceProgramsType::UBER_WITH_STATIC_IF:
                std::cerr << "UBER_WITH_STATIC_IF\n";
                break;
            }
            mSurfaceProgramType = type;
        }
    }

    // AbstractDrawableMesh implementation

    void updateBuffers(
        MRI::BuffersBitSet buffersToUpdate = MRI::BUFFERS_ALL) override
    {
        if constexpr (HasName<MeshType>) {
            AbstractDrawableMesh::name() = MeshType::name();
        }

        AbstractDrawableMesh::computeBoundingBox(
            static_cast<const MeshType&>(*this));

        mMRB.update(*this, buffersToUpdate);
        mMRS.setRenderCapabilityFrom(*this);
        setRenderSettings(mMRS);
    }

    void setRenderSettings(const MeshRenderSettings& rs) override
    {
        AbstractDrawableMesh::setRenderSettings(rs);
        mMeshRenderSettingsUniforms.updateSettings(rs);
        mMRB.updateEdgeSettings(rs);
        mMRB.updateWireframeSettings(rs);
    }

    uint vertexNumber() const override { return MeshType::vertexNumber(); }

    uint faceNumber() const override
    {
        if constexpr (HasFaces<MeshType>)
            return MeshType::faceNumber();
        else
            return 0;
    }

    uint edgeNumber() const override
    {
        if constexpr (HasEdges<MeshType>)
            return MeshType::edgeNumber();
        else
            return 0;
    }

    vcl::Matrix44d transformMatrix() const override
    {
        if constexpr (HasTransformMatrix<MeshType>) {
            return MeshType::transformMatrix().template cast<double>();
        }
        else {
            return vcl::Matrix44d::Identity();
        }
    }

    View<MatIt> materials() const override
    {
        if constexpr (HasMaterials<MeshType>) {
            return MeshType::materials();
        }
        else {
            return View<MatIt>();
        }
    }

    const Image& textureImage(const std::string& path) const override
    {
        if constexpr (HasMaterials<MeshType>) {
            return MeshType::textureImage(path);
        }
        else {
            return AbstractDrawableMesh::textureImage(path);
        }
    }

    // DrawableObject implementation

    void init() override {}

    void draw(const DrawObjectSettings& settings) const override
    {
        using enum VertFragProgram;

        ProgramManager& pm = Context::instance().programManager();

        uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                         BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL;

        vcl::Matrix44f model = vcl::Matrix44f::Identity();

        if constexpr (HasTransformMatrix<MeshType>) {
            model = MeshType::transformMatrix().template cast<float>();
        }

        mMeshUniforms.update(*this);

        if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
            const PBRViewerSettings&    pbrSettings = settings.pbrSettings;
            const DrawableEnvironment*  env         = settings.environment;

            bool iblEnabled = pbrSettings.imageBasedLighting &&
                              env != nullptr && env->canDraw();

            for (uint i = 0; i < mMRB.triangleChunksNumber(); ++i) {
                uint64_t surfaceState = state;
                uint64_t materialState =
                    updateAndBindMaterialUniforms(i, iblEnabled);

                mMeshUniforms.updateFirstChunkIndex(
                    mMRB.triangleChunk(i).startIndex);

                bindUniforms();

                // Bind textures before vertex buffers!!
                mMRB.bindTextures(mMRS, i, *this);
                if (pbrSettings.pbrMode && iblEnabled) {
                    using enum DrawableEnvironment::TextureType;
                    env->bindTexture(BRDF_LUT, VCL_MRB_TEXTURE5);
                    env->bindTexture(IRRADIANCE, VCL_MRB_CUBEMAP0);
                    env->bindTexture(SPECULAR, VCL_MRB_CUBEMAP1);
                }
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(mMRS, i);

                if (pbrSettings.pbrMode) {
                    surfaceState |= materialState;
                }

                bgfx::setState(surfaceState);
                bgfx::setTransform(model.data());

                if (pbrSettings.pbrMode) {
                    bgfx::submit(
                        settings.viewId,
                        pm.getProgram<DRAWABLE_MESH_SURFACE_UBER_PBR>());
                }
                else {
                    bgfx::submit(settings.viewId, surfaceProgramSelector());
                }
            }
        }

        if (mMRS.isWireframe(MRI::Wireframe::VISIBLE)) {
            bgfx::setTransform(model.data());

            mMRB.drawWireframeLines(settings.viewId);
        }

        if (mMRS.isEdges(MRI::Edges::VISIBLE)) {
            bgfx::setTransform(model.data());

            mMRB.drawEdgeLines(settings.viewId);
        }

        if (mMRS.isPoints(MRI::Points::VISIBLE)) {
            if (!Context::instance().supportsCompute()) {
                // 1 px vertices
                mMRB.bindVertexBuffers(mMRS);
                bindUniforms();

                bgfx::setState(state | BGFX_STATE_PT_POINTS);
                bgfx::setTransform(model.data());

                bgfx::submit(
                    settings.viewId, pm.getProgram<DRAWABLE_MESH_POINTS>());
            }
            else {
                // generate splats (quads) lazy
                mMRB.computeQuadVertexBuffers(*this, settings.viewId);

                // render splats
                mMRB.bindVertexQuadBuffer();
                bindUniforms();

                bgfx::setState(state);
                bgfx::setTransform(model.data());

                bgfx::submit(
                    settings.viewId,
                    pm.getProgram<DRAWABLE_MESH_POINTS_INSTANCE>());
            }
        }
    }

    void drawId(const DrawObjectSettings& settings) const override
    {
        using enum VertFragProgram;

        ProgramManager& pm = Context::instance().programManager();

        uint64_t state =
            0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
            BGFX_STATE_DEPTH_TEST_LEQUAL |
            BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ZERO);
        // write alpha as is

        vcl::Matrix44f model = vcl::Matrix44f::Identity();

        if constexpr (HasTransformMatrix<MeshType>) {
            model = MeshType::transformMatrix().template cast<float>();
        }

        const std::array<float, 4> idFloat = {
            std::bit_cast<float>(settings.objectId), 0.0f, 0.0f, 0.0f};

        if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
            mMRB.bindVertexBuffers(mMRS);
            mMRB.bindIndexBuffers(mMRS);
            mIdUniform.bind(&idFloat);
            mMeshUniforms.updateFirstChunkIndex(0);

            bgfx::setState(state);
            bgfx::setTransform(model.data());

            bgfx::submit(
                settings.viewId, pm.getProgram<DRAWABLE_MESH_SURFACE_ID>());
        }

        // if (mMRS.isWireframe(MRI::Wireframe::VISIBLE)) {
        //     mMRB.bindVertexBuffers(mMRS);
        //     mMRB.bindIndexBuffers(mMRS, MRI::Buffers::WIREFRAME);
        //     mIdUniform.bind(&idFloat);

        //     bgfx::setState(state | BGFX_STATE_PT_LINES);
        //     bgfx::setTransform(model.data());

        //     bgfx::submit(viewId,
        //     pm.getProgram<DRAWABLE_MESH_WIREFRAME_ID>());
        // }

        // if (mMRS.isEdges(MRI::Edges::VISIBLE)) {
        //     mMRB.bindVertexBuffers(mMRS);
        //     mMRB.bindIndexBuffers(mMRS, MRI::Buffers::EDGES);
        //     mIdUniform.bind(&idFloat);

        //     bgfx::setState(state | BGFX_STATE_PT_LINES);
        //     bgfx::setTransform(model.data());

        //     bgfx::submit(viewId, pm.getProgram<DRAWABLE_MESH_EDGES_ID>());
        // }

        if (mMRS.isPoints(MRI::Points::VISIBLE)) {
            if (!Context::instance().supportsCompute()) {
                // 1 px vertices
                mMRB.bindVertexBuffers(mMRS);
                mIdUniform.bind(&idFloat);

                bgfx::setState(state | BGFX_STATE_PT_POINTS);
                bgfx::setTransform(model.data());

                bgfx::submit(
                    settings.viewId, pm.getProgram<DRAWABLE_MESH_POINTS_ID>());
            }
            else {
                // generate splats (quads) lazy
                mMRB.computeQuadVertexBuffers(*this, settings.viewId);

                // render splats
                mMRB.bindVertexQuadBuffer();
                bindUniforms();
                mIdUniform.bind(&idFloat);

                bgfx::setState(state);
                bgfx::setTransform(model.data());

                bgfx::submit(
                    settings.viewId,
                    pm.getProgram<DRAWABLE_MESH_POINTS_INSTANCE_ID>());
            }
        }
    }

    std::shared_ptr<DrawableObject> clone() const& override
    {
        return std::make_shared<DrawableMeshBGFX>(*this);
    }

    std::shared_ptr<DrawableObject> clone() && override
    {
        return std::make_shared<DrawableMeshBGFX>(std::move(*this));
    }

    void setVisibility(bool vis) override
    {
        AbstractDrawableMesh::setVisibility(vis);
        mMeshRenderSettingsUniforms.updateSettings(mMRS);
    }

    std::string& name() override { return MeshType::name(); }

    const std::string& name() const override { return MeshType::name(); }

protected:
    void bindUniforms() const
    {
        mMeshUniforms.bind();
        mMeshRenderSettingsUniforms.bind();
    }

    /**
     * @brief Sets and binds the material uniforms for the given triangle chunk,
     * and returns the render state associated to the material that must be set
     * for the draw call.
     *
     * @param chunkNumber
     * @return the render state associated to the material
     */
    uint64_t updateAndBindMaterialUniforms(
        uint chunkNumber,
        bool imageBasedLighting) const
    {
        static const Material DEFAULT_MATERIAL;

        uint64_t state = BGFX_STATE_NONE;

        std::array<bool, N_TEXTURE_TYPES> textureAvailable = {false};

        if constexpr (!HasMaterials<MeshType>) {
            // fallback to default material
            mMaterialUniforms.update(
                DEFAULT_MATERIAL,
                isPerVertexColorAvailable(*this),
                textureAvailable,
                isPerVertexTangentAvailable(*this),
                imageBasedLighting);
        }
        else {
            using enum Material::AlphaMode;

            uint materialId = mMRB.materialIndex(mMRS, chunkNumber);

            if (materialId == UINT_NULL) {
                // fallback to default material
                mMaterialUniforms.update(
                    DEFAULT_MATERIAL,
                    isPerVertexColorAvailable(*this),
                    textureAvailable,
                    isPerVertexTangentAvailable(*this),
                    imageBasedLighting);
            }
            else {
                textureAvailable =
                    mMRB.textureAvailableArray(*this, materialId);

                mMaterialUniforms.update(
                    MeshType::material(materialId),
                    isPerVertexColorAvailable(*this),
                    textureAvailable,
                    isPerVertexTangentAvailable(*this),
                    imageBasedLighting);

                // set the state according to the material
                if (!MeshType::material(materialId).doubleSided()) {
                    // backface culling
                    state |= BGFX_STATE_CULL_CW;
                }
                if (MeshType::material(materialId).alphaMode() == ALPHA_BLEND) {
                    state |= BGFX_STATE_BLEND_ALPHA;
                }
            }
        }

        mMaterialUniforms.bind();
        return state;
    }

    // TODO: change this function implementation after shader benchmarks
    bgfx::ProgramHandle surfaceProgramSelector() const
    {
        using enum VertFragProgram;

        ProgramManager& pm = Context::instance().programManager();

        uint mul = 0;
        uint off = 0;

        {
            using enum MeshRenderInfo::Surface;
            if (mMRS.isSurface(SHADING_FLAT)) {
                mul = 1;
            }
            if (mMRS.isSurface(SHADING_SMOOTH)) {
                mul = 2;
            }
            if (mMRS.isSurface(COLOR_MESH)) {
                off = 1;
            }
            if (mMRS.isSurface(COLOR_FACE)) {
                off = 2;
            }
            if (mMRS.isSurface(COLOR_USER)) {
                off = 3;
            }
            if (mMRS.isSurface(COLOR_VERTEX_TEX)) {
                off = 4;
            }
            if (mMRS.isSurface(COLOR_WEDGE_TEX)) {
                off = 5;
            }
        }

        VertFragProgram p = static_cast<VertFragProgram>(6 * mul + off);

        if (mSurfaceProgramType == SurfaceProgramsType::SPLIT) {
            static const std::array<bgfx::ProgramHandle, 18>
                surfaceProgramHandles = {
                    pm.getProgram<DRAWABLE_MESH_SURFACE_NONE_COLOR_VERTEX>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_NONE_COLOR_MESH>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_NONE_COLOR_FACE>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_NONE_COLOR_USER>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_NONE_TEX_VERTEX>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_NONE_TEX_WEDGE>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_FLAT_COLOR_VERTEX>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_FLAT_COLOR_MESH>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_FLAT_COLOR_FACE>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_FLAT_COLOR_USER>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_FLAT_TEX_VERTEX>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_FLAT_TEX_WEDGE>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_VERTEX>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_MESH>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_FACE>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_USER>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_SMOOTH_TEX_VERTEX>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_SMOOTH_TEX_WEDGE>()};

            return surfaceProgramHandles[toUnderlying(p)];
        }

        if (mSurfaceProgramType == SurfaceProgramsType::UBER_WITH_STATIC_IF) {
            static const std::array<bgfx::ProgramHandle, 18>
                surfaceProgramHandles = {
                    pm.getProgram<DRAWABLE_MESH_SURFACE_NONE_COLOR_VERTEX_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_NONE_COLOR_MESH_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_NONE_COLOR_FACE_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_NONE_COLOR_USER_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_NONE_TEX_VERTEX_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_NONE_TEX_WEDGE_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_FLAT_COLOR_VERTEX_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_FLAT_COLOR_MESH_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_FLAT_COLOR_FACE_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_FLAT_COLOR_USER_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_FLAT_TEX_VERTEX_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_FLAT_TEX_WEDGE_SI>(),
                    pm.getProgram<
                        DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_VERTEX_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_MESH_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_FACE_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_USER_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_SMOOTH_TEX_VERTEX_SI>(),
                    pm.getProgram<DRAWABLE_MESH_SURFACE_SMOOTH_TEX_WEDGE_SI>()};

            return surfaceProgramHandles[toUnderlying(p)];
        }

        return pm.getProgram<DRAWABLE_MESH_SURFACE_UBER>();
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_MESH_BGFX_H
