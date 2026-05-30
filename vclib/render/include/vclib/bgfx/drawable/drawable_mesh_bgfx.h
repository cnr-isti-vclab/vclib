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
#include <vclib/render/selection/selection_box.h>

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

    // TODO: to be removed after shader benchmarks
    SurfaceProgramsType mSurfaceProgramType = SurfaceProgramsType::UBER;

    mutable uint mBufToTexRemainingFrames = 255;

    mutable std::vector<uint8_t> mVertexSelectionBackup;
    mutable std::vector<uint8_t> mFaceSelectionBackup;
    SelectionMode                mLastReadbackMode;

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
        swap(mBufToTexRemainingFrames, other.mBufToTexRemainingFrames);
        swap(mLastReadbackMode, other.mLastReadbackMode);
        swap(mVertexSelectionBackup, other.mVertexSelectionBackup);
        swap(mFaceSelectionBackup, other.mFaceSelectionBackup);
        swap(mSurfaceProgramType, other.mSurfaceProgramType);
        swap(mMRB, other.mMRB);
    }

    friend void swap(DrawableMeshBGFX& a, DrawableMeshBGFX& b) { a.swap(b); }

    using AbstractDrawableMesh::boundingBox;

    void computeSelection(const SelectionParameters& params) override
    {
        if (!isVisible()) {
            return;
        }
        if (params.mode.isFaceSelection()) {
            if (!(params.mode.isVisibleSelection() ?
                      faceSelectionVisible(params) :
                      faceSelection(params))) {
                return;
            }
        }
        else if (params.mode.isVertexSelection()) {
            if (!vertexSelection(params)) {
                return;
            }
        }
        if (mBufToTexRemainingFrames != 255 || params.isTemporary) {
            return;
        }
        mLastReadbackMode      = params.mode;
        mBufToTexRemainingFrames =
            mMRB.requestCPUCopyOfSelectionBuffer(params.mode);
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

        // Upload initial selection state from CPU to GPU
        uploadCPUSelectionToGPU();
    }

    void setRenderSettings(const MeshRenderSettings& rs) override
    {
        AbstractDrawableMesh::setRenderSettings(rs);
        mMRB.updateEdgeSettings(rs);
        mMRB.updateWireframeSettings(rs);
    }

    uint vertexCount() const override { return MeshType::vertexCount(); }

    uint faceCount() const override
    {
        if constexpr (HasFaces<MeshType>)
            return MeshType::faceCount();
        else
            return 0;
    }

    uint edgeCount() const override
    {
        if constexpr (HasEdges<MeshType>)
            return MeshType::edgeCount();
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

    void draw(const DrawObjectSettings& settings) override
    {
        using enum VertFragProgram;

        ProgramManager& pm = Context::instance().programManager();

        uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                         BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL;

        vcl::Matrix44f model = vcl::Matrix44f::Identity();

        if constexpr (HasTransformMatrix<MeshType>) {
            model = MeshType::transformMatrix().template cast<float>();
        }

        DrawableMeshUniforms::setColor(*this);
        MeshRenderSettingsUniforms::set(mMRS);

        if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
            const PBRViewerSettings&   pbrSettings = settings.pbrSettings;
            const DrawableEnvironment* env         = settings.environment;

            bool iblEnabled = pbrSettings.imageBasedLighting &&
                              env != nullptr && env->canDraw();

            for (uint i = 0; i < mMRB.triangleChunksNumber(); ++i) {
                // Bind textures before vertex buffers!!

                /* TEXTURES */
                DrawableMeshUniforms::resetTextureStages();
                // tStage is the first stage from which we can bind new 2D
                // textures
                uint tStage = mMRB.bindTextures(mMRS, i, *this);
                if (pbrSettings.pbrMode && iblEnabled) {
                    using enum DrawableEnvironment::TextureType;
                    env->bindTexture(BRDF_LUT, tStage);

                    DrawableMeshUniforms::setTextureStage(
                        DrawableMeshUniforms::TextureType::BRDF_LUT, tStage);

                    env->bindTexture(IRRADIANCE, VCL_MRB_CUBEMAP0);
                    env->bindTexture(SPECULAR, VCL_MRB_CUBEMAP1);
                }

                /* BUFFERS */
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(mMRS, i);
                if (mMRB.hasFaceSelectionBuffer())
                    mMRB.bindSelectedFacesBuffer();

                /* UNIFORMS */
                DrawableMeshUniforms::setFirstChunkIndex(
                    mMRB.triangleChunk(i).startIndex);
                uint64_t materialState =
                    updateAndBindMaterialUniforms(i, iblEnabled);

                bindUniforms();

                bgfx::setTransform(model.data());

                /* STATE */
                uint64_t surfaceState = state;
                if (pbrSettings.pbrMode) {
                    surfaceState |= materialState;
                }

                bgfx::setState(surfaceState);

                /* SUBMIT */
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

            mMRB.drawWireframeLines(settings.additionalViewIds[0]);
        }

        if (mMRS.isEdges(MRI::Edges::VISIBLE)) {
            bgfx::setTransform(model.data());

            mMRB.drawEdgeLines(settings.additionalViewIds[0]);
        }

        if (mMRS.isPoints(MRI::Points::VISIBLE)) {
            if (!Context::instance().supportsCompute()) {
                // 1 px vertices
                mMRB.bindVertexBuffers(mMRS);
                if (mMRB.hasVertexSelectionBuffer())
                    mMRB.bindSelectedVerticesBuffer();
                bindUniforms();

                bgfx::setState(state | BGFX_STATE_PT_POINTS);
                bgfx::setTransform(model.data());

                bgfx::submit(
                    settings.additionalViewIds[1],
                    pm.getProgram<DRAWABLE_MESH_POINTS>());
            }
            else {
                // generate splats (quads) lazy
                mMRB.computeQuadVertexBuffers(*this, settings.viewId);

                // render splats
                mMRB.bindVertexQuadBuffer();
                mMRB.bindPointsVertexColorBuffer();
                if (mMRB.hasVertexSelectionBuffer())
                    mMRB.bindSelectedVerticesBuffer();
                bindUniforms();

                bgfx::setState(state);
                bgfx::setTransform(model.data());

                bgfx::submit(
                    settings.additionalViewIds[1],
                    pm.getProgram<DRAWABLE_MESH_POINTS_INSTANCE>());
            }
        }

        // selection readback
        {
            switch (mBufToTexRemainingFrames) {
            case 0:
                mBufToTexRemainingFrames = 255;
                {
                    auto vec = mMRB.getSelectionBufferCopy();
                    if (mLastReadbackMode.isVertexSelection()) {
                        mVertexSelectionBackup = std::move(vec);
                    }
                    else if (mLastReadbackMode.isFaceSelection()) {
                        mFaceSelectionBackup = std::move(vec);
                    }

                    // Update CPU-side selection flags from GPU readback
                    updateCPUSelectionFromGPU();
                }
                break;
            case 255: break;
            default: mBufToTexRemainingFrames--;
            }
        }
    }

    void drawId(const DrawObjectSettings& settings) override
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

        if (mMRS.isSurface(MRI::Surface::VISIBLE)) {
            mMRB.bindVertexBuffers(mMRS);
            mMRB.bindIndexBuffers(mMRS);
            DrawableMeshUniforms::setMeshId(settings.objectId);
            DrawableMeshUniforms::setFirstChunkIndex(0);
            bindUniforms();

            bgfx::setState(state);
            bgfx::setTransform(model.data());

            bgfx::submit(
                settings.viewId, pm.getProgram<DRAWABLE_MESH_SURFACE_ID>());
        }

        // TODO: manage ID for wireframe

        // TODO: manage ID for edges

        if (mMRS.isPoints(MRI::Points::VISIBLE)) {
            if (!Context::instance().supportsCompute()) {
                // 1 px vertices
                mMRB.bindVertexBuffers(mMRS);

                DrawableMeshUniforms::setMeshId(settings.objectId);
                bindUniforms();

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
                DrawableMeshUniforms::setMeshId(settings.objectId);
                bindUniforms();

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

    std::string& name() override { return MeshType::name(); }

    const std::string& name() const override { return MeshType::name(); }

protected:
    /**
     * @brief Uploads the CPU-side selection flags (from BitFlags component) to
     * the GPU selection buffers.
     *
     * This function reads the selected() flag from each vertex/face in the mesh
     * and uploads the corresponding bitfield to the GPU.
     *
     * @note For vertices, only the original mesh vertices are synchronized,
     * not any duplicated vertices created for rendering purposes (e.g., for
     * wedge texture coordinates). Duplicated vertices will inherit the
     * selection state of their original vertex.
     */
    void uploadCPUSelectionToGPU()
    {
        // Upload vertex selection
        const uint numVerts = MeshType::vertexCount();
        if (numVerts == 0)
            return;

        // Compute number of uint32 words needed to store vertex selection
        // We use mMRB.numVerts() which includes duplicated vertices
        const uint numRenderVerts = mMRB.numVerts();
        const uint wordCount      = (numRenderVerts + 31) / 32;
        std::vector<uint8_t> vertexBackup(wordCount * 4, 0);

        // Build bitfield from mesh vertex selection flags
        // Note: For duplicated vertices (indices >= numVerts), they will
        // remain unselected unless explicitly set elsewhere
        uint vidx = 0;
        for (const auto& v : MeshType::vertices()) {
            if (v.selected()) {
                const uint wordIdx = vidx / 32;
                const uint bitIdx  = vidx % 32;
                const uint byteIdx = 4 * wordIdx + (3 - bitIdx / 8);
                const uint bitPos  = bitIdx % 8;
                vertexBackup[byteIdx] |= (1 << bitPos);
            }
            ++vidx;
        }

        mVertexSelectionBackup = vertexBackup;
        mMRB.setVertexSelectionFromCPUBuffer(vertexBackup);

        // Upload face selection
        if constexpr (HasFaces<MeshType>) {
            const uint numFaces = MeshType::faceCount();
            if (numFaces == 0 || !mMRB.hasFaceSelectionBuffer())
                return;

            const auto& indexMap = mMRB.triPolyIndexMap();
            const uint  numTris  = indexMap.triangleCount();

            // Compute number of uint32 words needed to store triangle selection
            const uint wordCount = (numTris + 31) / 32;
            std::vector<uint8_t> faceBackup(wordCount * 4, 0);

            // For each face, set selection for all its triangles
            for (const auto& f : MeshType::faces()) {
                if (f.selected()) {
                    const uint faceIdx     = f.index();
                    const uint firstTriIdx = indexMap.triangleBegin(faceIdx);
                    const uint numFaceTris = indexMap.triangleCount(faceIdx);

                    // Set selection for all triangles of this face
                    for (uint t = 0; t < numFaceTris; ++t) {
                        const uint triIdx  = firstTriIdx + t;
                        const uint wordIdx = triIdx / 32;
                        const uint bitIdx  = triIdx % 32;
                        const uint byteIdx = 4 * wordIdx + (3 - bitIdx / 8);
                        const uint bitPos  = bitIdx % 8;
                        faceBackup[byteIdx] |= (1 << bitPos);
                    }
                }
            }

            mFaceSelectionBackup = faceBackup;
            mMRB.setFaceSelectionFromCPUBuffer(faceBackup);
        }
    }

    /**
     * @brief Updates the CPU-side selection flags (BitFlags component) from the
     * GPU selection buffers that have been read back.
     *
     * This function parses the selection data from mVertexSelectionBackup and
     * mFaceSelectionBackup and updates the selected() flag on each mesh element.
     *
     * @note For vertices, only the original mesh vertices are synchronized.
     * Selection state of duplicated vertices is ignored.
     */
    void updateCPUSelectionFromGPU()
    {
        // Update vertex selection
        if constexpr (HasVertices<MeshType>) {
            if (mLastReadbackMode.isVertexSelection() &&
                !mVertexSelectionBackup.empty()) {
                uint vidx = 0;
                for (auto& v : MeshType::vertices()) {
                    const uint wordIdx = vidx / 32;
                    const uint bitIdx  = vidx % 32;
                    const uint byteIdx = 4 * wordIdx + (3 - bitIdx / 8);
                    const uint bitPos  = bitIdx % 8;

                    if (byteIdx < mVertexSelectionBackup.size()) {
                        const bool isSelected =
                            (mVertexSelectionBackup[byteIdx] & (1 << bitPos)) !=
                            0;
                        v.selected() = isSelected;
                    }
                    ++vidx;
                }
            }
        }

        // Update face selection
        if constexpr (HasFaces<MeshType>) {
            if (mLastReadbackMode.isFaceSelection() &&
                !mFaceSelectionBackup.empty()) {
                const auto& indexMap = mMRB.triPolyIndexMap();

                // First, clear all face selections
                for (auto& f : MeshType::faces()) {
                    f.selected() = false;
                }

                // For each face, check if its first triangle is selected
                // (compute shaders ensure all triangles of a face have the same
                // selection state)
                for (auto& f : MeshType::faces()) {
                    const uint faceIdx     = f.index();
                    const uint firstTriIdx = indexMap.triangleBegin(faceIdx);
                    const uint wordIdx     = firstTriIdx / 32;
                    const uint bitIdx      = firstTriIdx % 32;
                    const uint byteIdx     = 4 * wordIdx + (3 - bitIdx / 8);
                    const uint bitPos      = bitIdx % 8;

                    if (byteIdx < mFaceSelectionBackup.size()) {
                        const bool isSelected =
                            (mFaceSelectionBackup[byteIdx] & (1 << bitPos)) != 0;
                        f.selected() = isSelected;
                    }
                }
            }
        }
    }

    bool vertexSelection(const SelectionParameters& params)
    {
        if constexpr (!HasVertices<MeshType>) {
            return false;
        }
        if (params.isTemporary &&
            (params.mode == SelectionMode::VERTEX_ADD ||
             params.mode == SelectionMode::VERTEX_SUBTRACT)) {
            mMRB.setVertexSelectionFromCPUBuffer(mVertexSelectionBackup);
        }
        return (
            params.mode.isAtomicMode() ? mMRB.vertexSelectionAtomic(params) :
                                         mMRB.vertexSelection(params));
    }

    bool faceSelection(const SelectionParameters& params)
    {
        if constexpr (!HasFaces<MeshType>) {
            return false;
        }
        if (params.isTemporary &&
            (params.mode == SelectionMode::FACE_ADD ||
             params.mode == SelectionMode::FACE_SUBTRACT)) {
            mMRB.setFaceSelectionFromCPUBuffer(mFaceSelectionBackup);
        }
        return (
            params.mode.isAtomicMode() ? mMRB.faceSelectionAtomic(params) :
                                         mMRB.faceSelection(params));
    }

    bool faceSelectionVisible(const SelectionParameters& params)
    {
        if constexpr (!HasFaces<MeshType>) {
            return false;
        }
        if (params.isTemporary &&
            (params.mode == SelectionMode::FACE_VISIBLE_ADD ||
             params.mode == SelectionMode::FACE_VISIBLE_SUBTRACT)) {
            mMRB.setFaceSelectionFromCPUBuffer(mFaceSelectionBackup);
        }
        Matrix44f model = Matrix44f::Identity();
        if constexpr (HasTransformMatrix<MeshType>) {
            model = MeshType::transformMatrix().template cast<float>();
        }
        return mMRB.faceSelectionVisible(params, model);
    }

    void bindUniforms() const
    {
        MeshRenderSettingsUniforms::bind();
        DrawableMeshUniforms::bind();
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

        if constexpr (!HasMaterials<MeshType>) {
            // fallback to default material
            MaterialUniforms::set(
                DEFAULT_MATERIAL,
                isPerVertexColorAvailable(*this),
                isPerVertexTangentAvailable(*this),
                imageBasedLighting);
        }
        else {
            using enum Material::AlphaMode;

            uint materialId = mMRB.materialIndex(mMRS, chunkNumber);

            if (materialId == UINT_NULL) {
                // fallback to default material
                MaterialUniforms::set(
                    DEFAULT_MATERIAL,
                    isPerVertexColorAvailable(*this),
                    isPerVertexTangentAvailable(*this),
                    imageBasedLighting);
            }
            else {
                MaterialUniforms::set(
                    MeshType::material(materialId),
                    isPerVertexColorAvailable(*this),
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

        MaterialUniforms::bind();
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
