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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_MESH_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_MESH_H

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers.h>
#include <vclib/bgfx/drawable/uniforms/mesh_render_settings_uniforms.h>

#include <bgfx/bgfx.h>

namespace vcl {

template<MeshConcept MeshType>
class DrawableMeshBGFX : public AbstractDrawableMesh, public MeshType
{
    using MRI = MeshRenderInfo;

    Box3d mBoundingBox;

    MeshRenderBuffers<MeshType> mMRB;

    bgfx::ProgramHandle mProgramEdges =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::DRAWABLE_MESH_EDGES>();

    bgfx::ProgramHandle mProgramPoints =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::DRAWABLE_MESH_POINTS>();

    bgfx::ProgramHandle mProgramSurface =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::DRAWABLE_MESH_SURFACE>();

    bgfx::ProgramHandle mProgramWireframe =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::DRAWABLE_MESH_WIREFRAME>();

    bgfx::ProgramHandle mProgramEdgesID =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::DRAWABLE_MESH_EDGES_ID>();

    bgfx::ProgramHandle mProgramPointsID =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::DRAWABLE_MESH_POINTS_ID>();

    bgfx::ProgramHandle mProgramSurfaceID =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::DRAWABLE_MESH_SURFACE_ID>();

    bgfx::ProgramHandle mProgramWireframeID =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::DRAWABLE_MESH_WIREFRAME_ID>();

    mutable MeshRenderSettingsUniforms mMeshRenderSettingsUniforms;

    Uniform mIdUniform = Uniform("u_meshId", bgfx::UniformType::Vec4);

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
            MeshType(drawableMesh), mBoundingBox(drawableMesh.mBoundingBox),
            mMeshRenderSettingsUniforms(
                drawableMesh.mMeshRenderSettingsUniforms)
    {
        if constexpr (HasName<MeshType>) {
            AbstractDrawableMesh::name() = drawableMesh.name();
        }
        mMRB.update(*this);
    }

    DrawableMeshBGFX(DrawableMeshBGFX&& drawableMesh) { swap(drawableMesh); }

    ~DrawableMeshBGFX() = default;

    DrawableMeshBGFX& operator=(DrawableMeshBGFX drawableMesh)
    {
        swap(drawableMesh);
        return *this;
    }

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

        mMRB.update(*this, buffersToUpdate);
        mMRS.setRenderCapabilityFrom(*this);
        mMeshRenderSettingsUniforms.updateSettings(mMRS);
    }

    std::string& name() override { return MeshType::name(); }

    const std::string& name() const override { return MeshType::name(); }

    void swap(DrawableMeshBGFX& other)
    {
        using std::swap;
        AbstractDrawableMesh::swap(other);
        MeshType::swap(other);
        swap(mBoundingBox, other.mBoundingBox);
        swap(mMRB, other.mMRB);
        swap(mMeshRenderSettingsUniforms, other.mMeshRenderSettingsUniforms);
    }

    friend void swap(DrawableMeshBGFX& a, DrawableMeshBGFX& b) { a.swap(b); }

    // DrawableObject implementation

    void init() override {}

    void draw(uint viewId) const override
    {
        uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                         BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL;



        if (mMRS.isPoints(MRI::Points::VISIBLE)) {
            if (bgfx::isValid(mProgramPoints)) {
                mMRB.bindVertexBuffers(mMRS);
                bindUniforms();

                bgfx::setState(state | BGFX_STATE_PT_POINTS);

                bgfx::submit(viewId, mProgramPoints);
            }
        }

        if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
            if (bgfx::isValid(mProgramSurface)) {
                mMRB.bindTextures(); // Bind textures before vertex buffers!!
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(mMRS);
                bindUniforms();

                bgfx::setState(state);

                bgfx::submit(viewId, mProgramSurface);
            }
        }

        if (mMRS.isWireframe(MRI::Wireframe::VISIBLE)) {
            if (bgfx::isValid(mProgramWireframe)) {
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(mMRS, MRI::Buffers::WIREFRAME);
                bindUniforms();

                bgfx::setState(state | BGFX_STATE_PT_LINES);

                bgfx::submit(viewId, mProgramWireframe);
            }
        }

        if (mMRS.isEdges(MRI::Edges::VISIBLE)) {
            if (bgfx::isValid(mProgramEdges)) {
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(mMRS, MRI::Buffers::EDGES);
                bindUniforms();

                bgfx::setState(state | BGFX_STATE_PT_LINES);

                bgfx::submit(viewId, mProgramEdges);
            }
        }
    }

    void drawId(uint viewId, uint id) const override
    {
        uint64_t state =
            0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
            BGFX_STATE_DEPTH_TEST_LEQUAL |
            BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ZERO);
        // write alpha as is

        const std::array<float, 4> idFloat = {
            Uniform::uintBitsToFloat(id), 0.0f, 0.0f, 0.0f};

        if (mMRS.isPoints(MRI::Points::VISIBLE)) {
            if (bgfx::isValid(mProgramPointsID)) {
                mMRB.bindVertexBuffers(mMRS);
                mIdUniform.bind(&idFloat[0]);

                bgfx::setState(state | BGFX_STATE_PT_POINTS);

                bgfx::submit(viewId, mProgramPointsID);
            }
        }

        if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
            if (bgfx::isValid(mProgramSurfaceID)) {
                mMRB.bindTextures(); // Bind textures before vertex buffers!!
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(mMRS);
                mIdUniform.bind(&idFloat);

                bgfx::setState(state);

                bgfx::submit(viewId, mProgramSurfaceID);
            }
        }

        if (mMRS.isWireframe(MRI::Wireframe::VISIBLE)) {
            if (bgfx::isValid(mProgramWireframeID)) {
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(mMRS, MRI::Buffers::WIREFRAME);
                mIdUniform.bind(&idFloat);

                bgfx::setState(state | BGFX_STATE_PT_LINES);

                bgfx::submit(viewId, mProgramWireframeID);
            }
        }

        if (mMRS.isEdges(MRI::Edges::VISIBLE)) {
            if (bgfx::isValid(mProgramEdgesID)) {
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(mMRS, MRI::Buffers::EDGES);
                mIdUniform.bind(&idFloat);

                bgfx::setState(state | BGFX_STATE_PT_LINES);

                bgfx::submit(viewId, mProgramEdgesID);
            }
        }
    }

    Box3d boundingBox() const override { return mBoundingBox; }

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

    void setRenderSettings(const MeshRenderSettings& rs) override
    {
        AbstractDrawableMesh::setRenderSettings(rs);
        mMeshRenderSettingsUniforms.updateSettings(rs);
    }

private:
    void bindUniforms() const
    {
        mMeshRenderSettingsUniforms.bind();
        mMRB.bindUniforms();
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_MESH_H
