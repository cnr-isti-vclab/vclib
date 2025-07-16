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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_MESH_979_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_MESH_979_H

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>

#include <vclib/bgfx/context.h>
#include "mesh_render_buffers_979.h"
#include <vclib/bgfx/drawable/uniforms/mesh_render_settings_uniforms.h>

#include <bgfx/bgfx.h>

namespace vcl {

template<MeshConcept MeshType>
class DrawableMeshBGFX979 : public AbstractDrawableMesh, public MeshType
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

    Box3d mBoundingBox;

    mutable MeshRenderSettingsUniforms mMeshRenderSettingsUniforms;

    Uniform mIdUniform = Uniform("u_meshId", bgfx::UniformType::Vec4);

    // TODO: to be removed after shader benchmarks
    SurfaceProgramsType mSurfaceProgramType = SurfaceProgramsType::UBER;

protected:
    MeshRenderBuffers979<MeshType> mMRB;

public:
    DrawableMeshBGFX979() = default;

    DrawableMeshBGFX979(const MeshType& mesh) :
            AbstractDrawableMesh(mesh), MeshType(mesh)
    {
        updateBuffers();
    }

    DrawableMeshBGFX979(MeshType&& mesh) :
            AbstractDrawableMesh(mesh), MeshType(std::move(mesh))
    {
        updateBuffers();
    }

    DrawableMeshBGFX979(const DrawableMeshBGFX979& drawableMesh) :
            AbstractDrawableMesh((const AbstractDrawableMesh&) drawableMesh),
            MeshType(drawableMesh), mBoundingBox(drawableMesh.mBoundingBox),
            mMeshRenderSettingsUniforms(
                drawableMesh.mMeshRenderSettingsUniforms)
    {
        if constexpr (HasName<MeshType>) {
            AbstractDrawableMesh::name() = drawableMesh.name();
        }
        mMRB.update(*this);
    }

    DrawableMeshBGFX979(DrawableMeshBGFX979&& drawableMesh) { swap(drawableMesh); }

    ~DrawableMeshBGFX979() = default;

    DrawableMeshBGFX979& operator=(DrawableMeshBGFX979 drawableMesh)
    {
        swap(drawableMesh);
        return *this;
    }

    void swap(DrawableMeshBGFX979& other)
    {
        using std::swap;
        AbstractDrawableMesh::swap(other);
        MeshType::swap(other);
        swap(mBoundingBox, other.mBoundingBox);
        swap(mMRB, other.mMRB);
        swap(mMeshRenderSettingsUniforms, other.mMeshRenderSettingsUniforms);
    }

    friend void swap(DrawableMeshBGFX979& a, DrawableMeshBGFX979& b) { a.swap(b); }

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

        bool bbToInitialize = !vcl::HasBoundingBox<MeshType>;
        if constexpr (vcl::HasBoundingBox<MeshType>) {
            if (this->MeshType::boundingBox().isNull()) {
                bbToInitialize = true;
            }
            else {
                mBoundingBox =
                    this->MeshType::boundingBox().template cast<double>();
            }
        }

        if (bbToInitialize) {
            mBoundingBox = vcl::boundingBox(*this);
        }

        if constexpr (HasTransformMatrix<MeshType>) {
            mBoundingBox.min() *=
                MeshType::transformMatrix().template cast<double>();
            mBoundingBox.max() *=
                MeshType::transformMatrix().template cast<double>();
        }

        mMRB.update(*this, buffersToUpdate);
        mMRS.setRenderCapabilityFrom(*this);
        mMeshRenderSettingsUniforms.updateSettings(mMRS);
    }

    void setRenderSettings(const MeshRenderSettings& rs) override
    {
        AbstractDrawableMesh::setRenderSettings(rs);
        mMeshRenderSettingsUniforms.updateSettings(rs);
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

    std::vector<std::string> textures() const override
    {
        std::vector<std::string> txs;
        if constexpr (HasTexturePaths<MeshType>) {
            txs.reserve(MeshType::textureNumber());
            for (const auto& tpath : MeshType::texturePaths()) {
                txs.push_back(tpath);
            }
        }
        return txs;
    }

    // DrawableObject implementation

    void init() override {}

    void draw(uint viewId) const override
    {
        using enum VertFragProgram;

        ProgramManager& pm = Context::instance().programManager();

        uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                         BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL;

        vcl::Matrix44f model = vcl::Matrix44f::Identity();

        if constexpr (HasTransformMatrix<MeshType>) {
            model = MeshType::transformMatrix().template cast<float>();
        }

        mMRB.calculateSelection(*this, viewId, mBoundingBox);

        if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
            mMRB.bindTextures(); // Bind textures before vertex buffers!!
            mMRB.bindVertexBuffers(mMRS);
            mMRB.bindIndexBuffers(mMRS);
            bindUniforms();

            bgfx::setState(state);
            bgfx::setTransform(model.data());

            bgfx::submit(viewId, surfaceProgramSelector());
        }

        if (mMRS.isWireframe(MRI::Wireframe::VISIBLE)) {
            mMRB.bindVertexBuffers(mMRS);
            mMRB.bindIndexBuffers(mMRS, MRI::Buffers::WIREFRAME);
            bindUniforms();

            bgfx::setState(state | BGFX_STATE_PT_LINES);
            bgfx::setTransform(model.data());

            bgfx::submit(viewId, pm.getProgram<DRAWABLE_MESH_WIREFRAME>());
        }

        if (mMRS.isEdges(MRI::Edges::VISIBLE)) {
            mMRB.bindVertexBuffers(mMRS);
            mMRB.bindIndexBuffers(mMRS, MRI::Buffers::EDGES);
            bindUniforms();

            bgfx::setState(state | BGFX_STATE_PT_LINES);
            bgfx::setTransform(model.data());

            bgfx::submit(viewId, pm.getProgram<DRAWABLE_MESH_EDGES>());
        }

        if (mMRS.isPoints(MRI::Points::VISIBLE)) {
            if (!Context::instance().supportsCompute()) {
                // 1 px vertices
                mMRB.bindVertexBuffers(mMRS);
                bindUniforms();

                bgfx::setState(state | BGFX_STATE_PT_POINTS);
                bgfx::setTransform(model.data());

                bgfx::submit(viewId, pm.getProgram<DRAWABLE_MESH_POINTS>());
            }
            else {
                // generate splats (quads) lazy
                mMRB.computeQuadVertexBuffers(*this, viewId);

                // render splats
                mMRB.bindVertexQuadBuffer();
                bindUniforms();

                bgfx::setState(state);
                bgfx::setTransform(model.data());

                bgfx::submit(
                    viewId, pm.getProgram<DRAWABLE_MESH_POINTS_INSTANCE>());
            }
        }
    }

    void drawId(uint viewId, uint id) const override
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
            Uniform::uintBitsToFloat(id), 0.0f, 0.0f, 0.0f};

        if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
            mMRB.bindTextures(); // Bind textures before vertex buffers!!
            mMRB.bindVertexBuffers(mMRS);
            mMRB.bindIndexBuffers(mMRS);
            mIdUniform.bind(&idFloat);

            bgfx::setState(state);
            bgfx::setTransform(model.data());

            bgfx::submit(viewId, pm.getProgram<DRAWABLE_MESH_SURFACE_ID>());
        }

        if (mMRS.isWireframe(MRI::Wireframe::VISIBLE)) {
            mMRB.bindVertexBuffers(mMRS);
            mMRB.bindIndexBuffers(mMRS, MRI::Buffers::WIREFRAME);
            mIdUniform.bind(&idFloat);

            bgfx::setState(state | BGFX_STATE_PT_LINES);
            bgfx::setTransform(model.data());

            bgfx::submit(viewId, pm.getProgram<DRAWABLE_MESH_WIREFRAME_ID>());
        }

        if (mMRS.isEdges(MRI::Edges::VISIBLE)) {
            mMRB.bindVertexBuffers(mMRS);
            mMRB.bindIndexBuffers(mMRS, MRI::Buffers::EDGES);
            mIdUniform.bind(&idFloat);

            bgfx::setState(state | BGFX_STATE_PT_LINES);
            bgfx::setTransform(model.data());

            bgfx::submit(viewId, pm.getProgram<DRAWABLE_MESH_EDGES_ID>());
        }

        if (mMRS.isPoints(MRI::Points::VISIBLE)) {
            if (!Context::instance().supportsCompute()) {
                // 1 px vertices
                mMRB.bindVertexBuffers(mMRS);
                mIdUniform.bind(&idFloat);

                bgfx::setState(state | BGFX_STATE_PT_POINTS);
                bgfx::setTransform(model.data());

                bgfx::submit(viewId, pm.getProgram<DRAWABLE_MESH_POINTS_ID>());
            }
            else {
                // generate splats (quads) lazy
                mMRB.computeQuadVertexBuffers(*this, viewId);

                // render splats
                mMRB.bindVertexQuadBuffer();
                bindUniforms();
                mIdUniform.bind(&idFloat);

                bgfx::setState(state);
                bgfx::setTransform(model.data());

                bgfx::submit(
                    viewId, pm.getProgram<DRAWABLE_MESH_POINTS_INSTANCE_ID>());
            }
        }
    }

    Box3d boundingBox() const override { return mBoundingBox; }

    std::shared_ptr<DrawableObject> clone() const& override
    {
        return std::make_shared<DrawableMeshBGFX979>(*this);
    }

    std::shared_ptr<DrawableObject> clone() && override
    {
        return std::make_shared<DrawableMeshBGFX979>(std::move(*this));
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
        mMeshRenderSettingsUniforms.bind();
        mMRB.bindUniforms();
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

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_MESH_H
