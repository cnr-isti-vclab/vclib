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
#include <vclib/bgfx/drawable/uniforms/mesh_render_settings_uniforms.h>
#include "mesh/mesh_render_buffers_979.h"
#include <vclib/render/selection/selection_box.h>
#include <vclib/render/selection/selectable.h>

#include <bgfx/bgfx.h>

#include <bitset>

#include <fstream>

namespace vcl {

template<MeshConcept MeshType>
class DrawableMeshBGFX979 : public AbstractDrawableMesh, public MeshType, public Selectable
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

    mutable MeshRenderSettingsUniforms mMeshRenderSettingsUniforms;

    Uniform mIdUniform = Uniform("u_meshId", bgfx::UniformType::Vec4);

    // TODO: to be removed after shader benchmarks
    SurfaceProgramsType mSurfaceProgramType = SurfaceProgramsType::UBER;

    bgfx::ProgramHandle selDrawProg = vcl::loadProgram("shaders/vs_selection", "shaders/fs_selection");
    bgfx::ProgramHandle faceSelDrawProg = vcl::loadProgram("shaders_face/vs_selection", "shaders_face/fs_selection");

    mutable uint mBufToTexRemainingFrames = 255;

    // VISIBLE FACES SELECTION DEBUGGING
    //bgfx::TextureHandle mBlitTex = BGFX_INVALID_HANDLE;
    //std::vector<uint8_t> mTexReadBackVec;
    //std::array<uint, 2> mColAttSize;
    //mutable uint mVisSelTexRBFrames = 255;

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
            MeshType(drawableMesh)
    {
        if constexpr (HasName<MeshType>) {
            AbstractDrawableMesh::name() = drawableMesh.name();
        }
        mMeshRenderSettingsUniforms.updateSettings(mMRS);
        updateBuffers();
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
        swap(mMRB, other.mMRB);
        swap(mMeshRenderSettingsUniforms, other.mMeshRenderSettingsUniforms);
        swap(mBufToTexRemainingFrames, other.mBufToTexRemainingFrames);
    }

    friend void swap(DrawableMeshBGFX979& a, DrawableMeshBGFX979& b) { a.swap(b); }

    using AbstractDrawableMesh::boundingBox;

    void calculateSelection(const SelectionParameters& params) override {
        // VISIBLE FACES SELECTION DEBUGGING
        //if (!bgfx::isValid(mBlitTex)) {
        //    mBlitTex = bgfx::createTexture2D(params.texAttachmentsSize[0], params.texAttachmentsSize[1], false, 0, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_BLIT_DST | BGFX_TEXTURE_READ_BACK);
        //    mTexReadBackVec = std::vector<uint8_t>();
        //    mTexReadBackVec.resize(params.texAttachmentsSize[0] * params.texAttachmentsSize[1] * 4, 0);
        //    mColAttSize = params.texAttachmentsSize;
        //}
        if (params.mode.isFaceSelection()) {
            if (!(params.mode.isVisibleSelection() ? faceSelectionVisible(params) : faceSelection(params))) {
                return;
            }
        } else if (params.mode.isVertexSelection()) {
            if (!vertexSelection(params)) {
                return;
            }
        }
        if (mBufToTexRemainingFrames != 255 || params.isTemporary) {
            return;
        }
        mBufToTexRemainingFrames = mMRB.requestCPUCopyOfSelectionBuffer(params.mode);
    }

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

        if constexpr (HasFaces<MeshType>) {
            std::vector<uint8_t> vec;
            uint count = 0;
            switch (mBufToTexRemainingFrames) {
                case 0:
                    mBufToTexRemainingFrames = 255;
                    vec = mMRB.getSelectionBufferCopy();
                    for (size_t index = 0; index < vec.size(); index++) {
                        count += uint(std::bitset<8>(vec[index]).count());
                    }
                    std::cout << "Selected count: " << count << std::endl;
                    break;
                case 255:
                    break;
                default:
                    mBufToTexRemainingFrames--;
            }
        }

        // VISIBLE FACES SELECTION DEBUGGING
        //{
        //    if (mVisSelTexRBFrames == 0) {
        //        mVisSelTexRBFrames = 255;
        //        std::fstream file;
        //        file.open("output.ppm", std::ios::binary | std::ios::out);
        //        file << "P6\n" << mColAttSize[0] << " " << mColAttSize[1] <<"\n255\n";
        //        size_t index = 0;
        //        for (const uint8_t& val: mTexReadBackVec) {
        //            ++index;
        //            if (index%4 == 1) {
        //                continue;
        //            }
        //            uint8_t newval = val * 128;
        //            file.write(reinterpret_cast<const char*>(&newval), 1);
        //        }
        //        file.close();
        //    } else if (mVisSelTexRBFrames != 255) {
        //        mVisSelTexRBFrames--;
        //    }
        //}

        if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
            for (uint i = 0; i < mMRB.triangleChunksNumber(); ++i) {
                uint64_t surfaceState  = state;
                uint64_t materialState = mMRB.bindMaterials(mMRS, i, *this);
                // Bind textures before vertex buffers!!
                mMRB.bindTextures(mMRS, i, *this);
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(mMRS, i);

                bindUniforms();

                if (settings.pbrMode) {
                    surfaceState |= materialState;
                }

                bgfx::setState(surfaceState);
                bgfx::setTransform(model.data());

                if (settings.pbrMode) {
                    ProgramManager& pm = Context::instance().programManager();

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

                bgfx::submit(settings.viewId, pm.getProgram<DRAWABLE_MESH_POINTS>());
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
                    settings.viewId, pm.getProgram<DRAWABLE_MESH_POINTS_INSTANCE>());
            }
        }

        mMRB.bindVertexBuffers(mMRS);
        mMRB.bindIndexBuffers(mMRS);
        bindUniforms();
        mMRB.bindSelectedVerticesBuffer();

        bgfx::setState(state | BGFX_STATE_BLEND_NORMAL | BGFX_STATE_PT_POINTS);
        bgfx::setTransform(model.data());

        bgfx::submit(settings.viewId, selDrawProg);

        mMRB.bindVertexBuffers(mMRS);
        mMRB.bindIndexBuffers(mMRS);
        bindUniforms();
        mMRB.bindSelectedFacesBuffer();

        bgfx::setState(state | BGFX_STATE_BLEND_NORMAL);
        bgfx::setTransform(model.data());

        bgfx::submit(settings.viewId, faceSelDrawProg);
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
            Uniform::uintBitsToFloat(settings.objectId), 0.0f, 0.0f, 0.0f};

        if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
            mMRB.bindVertexBuffers(mMRS);
            mMRB.bindIndexBuffers(mMRS);
            mIdUniform.bind(&idFloat);

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

    bool vertexSelection(const SelectionParameters& params)
    {
        if constexpr (!HasVertices<MeshType>) {
            return false;
        }
        return (params.mode.isAtomicMode() ? 
            mMRB.vertexSelectionAtomic(params) 
            : 
            mMRB.vertexSelection(params)
        );
        return true;
    }

    bool faceSelection(const SelectionParameters& params)
    {
        if constexpr (!HasFaces<MeshType>) {
            return false;
        }
        return (params.mode.isAtomicMode() ? 
            mMRB.faceSelectionAtomic(params) 
            : 
            mMRB.faceSelection(params)
        );
    }

    bool faceSelectionVisible(const SelectionParameters& params)
    {
        if constexpr (!HasFaces<MeshType>) {
            return false;
        }
        Matrix44f model = Matrix44f::Identity();
        if constexpr (HasTransformMatrix<MeshType>) {
            model = MeshType::transformMatrix().template cast<float>();
        }
        bool ret = mMRB.faceSelectionVisible(params, model);
        // VISIBLE FACE SELECTION ATTACHMENT DEBUGGING (BLIT)
        //if (!params.isTemporary) {
        //    bgfx::blit(202, mBlitTex, 0, 0, params.meshIdTex, 0, 0, params.texAttachmentsSize[0], params.texAttachmentsSize[1]);
        //    bgfx::readTexture(mBlitTex, mTexReadBackVec.data());
        //    mVisSelTexRBFrames = 3;
        //}
        return ret;
    }

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

#endif // VCL_BGFX_DRAWABLE_MESH_979_H
