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

#ifndef VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_H
#define VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_H

#include "mesh_render_buffers_macros.h"

#include <vclib/algorithms/core/create.h>
#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/uniforms/drawable_mesh_uniforms.h>
#include <vclib/bgfx/drawable/uniforms/material_uniforms.h>
#include <vclib/bgfx/primitives/lines.h>
#include <vclib/bgfx/texture.h>
#include <vclib/io/image/load.h>
#include <vclib/render/drawable/mesh/mesh_render_data.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>
#include <vclib/space/core/image.h>

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

namespace vcl {

template<MeshConcept Mesh>
class MeshRenderBuffers : public MeshRenderData<MeshRenderBuffers<Mesh>>
{
    using MeshType = Mesh;
    using Base     = MeshRenderData<MeshRenderBuffers<MeshType>>;
    using MRI      = MeshRenderInfo;

    friend Base;

    inline static const uint N_TEXTURE_TYPES =
        toUnderlying(Material::TextureType::COUNT);

    VertexBuffer mVertexPositionsBuffer;
    VertexBuffer mVertexNormalsBuffer;
    VertexBuffer mVertexColorsBuffer;
    VertexBuffer mVertexUVBuffer;
    VertexBuffer mVertexWedgeUVBuffer;
    VertexBuffer mVertexTangentsBuffer;

    // point splatting
    IndexBuffer         mVertexQuadIndexBuffer;
    DynamicVertexBuffer mVertexQuadBuffer;
    mutable bool        mVertexQuadBufferGenerated = false;

    IndexBuffer mTriangleIndexBuffer;
    IndexBuffer mTriangleNormalBuffer;
    IndexBuffer mTriangleColorBuffer;

    Lines mEdgeLines;

    Lines mWireframeLines;
    Color mMeshColor; // todo: find better way to store mesh color

    // map of textures
    // for each texture path of each material, store its texture
    std::map<std::string, Texture> mMaterialTextures;

    mutable DrawableMeshUniforms mMeshUniforms;
    mutable MaterialUniforms mMaterialUniforms;
    std::array<Uniform, N_TEXTURE_TYPES> mTextureSamplerUniforms;

public:
    MeshRenderBuffers() = default;

    MeshRenderBuffers(
        const MeshType&    mesh,
        MRI::BuffersBitSet buffersToFill = MRI::BUFFERS_ALL) :
            Base(buffersToFill)
    {
        Base::update(mesh, buffersToFill);
    }

    MeshRenderBuffers(const MeshRenderBuffers& other) = delete;

    MeshRenderBuffers(MeshRenderBuffers&& other) { swap(other); }

    MeshRenderBuffers& operator=(const MeshRenderBuffers& other) = delete;

    MeshRenderBuffers& operator=(MeshRenderBuffers&& other)
    {
        swap(other);
        return *this;
    }

    void swap(MeshRenderBuffers& other)
    {
        using std::swap;
        Base::swap(other);
        swap(mVertexPositionsBuffer, other.mVertexPositionsBuffer);
        swap(mVertexNormalsBuffer, other.mVertexNormalsBuffer);
        swap(mVertexColorsBuffer, other.mVertexColorsBuffer);
        swap(mVertexUVBuffer, other.mVertexUVBuffer);
        swap(mVertexWedgeUVBuffer, other.mVertexWedgeUVBuffer);
        swap(mVertexTangentsBuffer, other.mVertexTangentsBuffer);
        swap(mVertexQuadIndexBuffer, other.mVertexQuadIndexBuffer);
        swap(mVertexQuadBuffer, other.mVertexQuadBuffer);
        swap(mVertexQuadBufferGenerated, other.mVertexQuadBufferGenerated);
        swap(mTriangleIndexBuffer, other.mTriangleIndexBuffer);
        swap(mTriangleNormalBuffer, other.mTriangleNormalBuffer);
        swap(mTriangleColorBuffer, other.mTriangleColorBuffer);
        swap(mEdgeLines, other.mEdgeLines);
        swap(mWireframeLines, other.mWireframeLines);
        swap(mMaterialTextures, other.mMaterialTextures);
        swap(mMeshUniforms, other.mMeshUniforms);
        swap(mMaterialUniforms, other.mMaterialUniforms);
        swap(mTextureSamplerUniforms, other.mTextureSamplerUniforms);
    }

    friend void swap(MeshRenderBuffers& a, MeshRenderBuffers& b) { a.swap(b); }

    uint triangleChunksNumber() const { return Base::mMaterialChunks.size(); }

    // to generate splats
    void computeQuadVertexBuffers(
        const MeshType&    mesh,
        const bgfx::ViewId viewId) const
    {
        if (!mVertexQuadBuffer.isValid() || mVertexQuadBufferGenerated) {
            return;
        }

        // fill the buffer using compute shader
        mVertexPositionsBuffer.bindCompute(
            VCL_MRB_VERTEX_POSITION_STREAM, bgfx::Access::Read);
        mVertexNormalsBuffer.bindCompute(
            VCL_MRB_VERTEX_NORMAL_STREAM, bgfx::Access::Read);
        mVertexColorsBuffer.bindCompute(
            VCL_MRB_VERTEX_COLOR_STREAM, bgfx::Access::Read);

        mVertexQuadBuffer.bindCompute(4, bgfx::Access::Write);

        auto& pm = Context::instance().programManager();
        bgfx::dispatch(
            viewId,
            pm.getComputeProgram<ComputeProgram::DRAWABLE_MESH_POINTS>(),
            mesh.vertexNumber(),
            1,
            1);

        mVertexQuadBufferGenerated = true;
    }

    void bindVertexBuffers(const MeshRenderSettings& mrs) const
    {
        uint stream = 0;

        // streams MUST be consecutive starting from 0
        mVertexPositionsBuffer.bindVertex(stream++);

        if (mVertexNormalsBuffer.isValid()) {
            mVertexNormalsBuffer.bindVertex(stream++);
        }

        if (mVertexColorsBuffer.isValid()) {
            mVertexColorsBuffer.bindVertex(stream++);
        }

        if (mVertexUVBuffer.isValid()) {
            mVertexUVBuffer.bind(stream++);
        }

        if (mVertexWedgeUVBuffer.isValid()) {
            mVertexWedgeUVBuffer.bind(stream++);
        }

        if (mVertexTangentsBuffer.isValid()) {
            mVertexTangentsBuffer.bind(stream++);
        }
    }

    // to draw splats
    void bindVertexQuadBuffer() const
    {
        mVertexQuadBuffer.bind(VCL_MRB_VERTEX_POSITION_STREAM);
        mVertexQuadIndexBuffer.bind();
    }

    void bindIndexBuffers(
        const MeshRenderSettings& mrs,
        uint                      chunkToBind = UINT_NULL) const
    {
        using enum MRI::Buffers;

        if (chunkToBind == UINT_NULL) {
            mTriangleIndexBuffer.bind();
            mMeshUniforms.updateFirstChunkIndex(0);
        }
        else {
            const auto& chunk = Base::mMaterialChunks[chunkToBind];
            mMeshUniforms.updateFirstChunkIndex(chunk.startIndex);
            mTriangleIndexBuffer.bind(
                chunk.startIndex * 3, chunk.indexCount * 3);
        }

        mTriangleNormalBuffer.bind(VCL_MRB_PRIMITIVE_NORMAL_BUFFER);

        mTriangleColorBuffer.bind(VCL_MRB_PRIMITIVE_COLOR_BUFFER);
    }

    void drawEdgeLines(uint viewId) const { mEdgeLines.draw(viewId); }

    void drawWireframeLines(uint viewId) const { mWireframeLines.draw(viewId); }

    void bindTextures(
        const MeshRenderSettings& mrs,
        uint                      chunkNumber,
        const MeshType&           m) const
    {
        uint materialId = Base::materialIndex(mrs, chunkNumber);

        if (materialId != UINT_NULL) {
            for (uint j = 0; j < N_TEXTURE_TYPES; ++j) {
                const auto& td = m.material(materialId).textureDescriptor(j);
                const std::string& path = td.path();
                if (!path.empty()) {
                    const Texture& tex = mMaterialTextures.at(path);
                    if (tex.isValid()) {
                        uint flags = Texture::samplerFlagsFromTexture(td);
                        tex.bind(
                            VCL_MRB_TEXTURE0 + j,
                            mTextureSamplerUniforms[j].handle(),
                            flags);
                    }
                }
            }
        }
    }

    /**
     * @brief Sets and binds the material uniforms for the given triangle chunk,
     * and returns the render state associated to the material that must be set
     * for the draw call.
     *
     * @param mrs
     * @param chunkNumber
     * @param m
     * @return the render state associated to the material
     */
    uint64_t bindMaterials(
        const MeshRenderSettings& mrs,
        uint                      chunkNumber,
        const MeshType&           m) const
    {
        static const Material DEFAULT_MATERIAL;

        uint64_t state = BGFX_STATE_NONE;

        std::array<bool, N_TEXTURE_TYPES> textureAvailable = {false};

        if constexpr (!HasMaterials<MeshType>) {
            // fallback to default material
            mMaterialUniforms.update(
                DEFAULT_MATERIAL,
                isPerVertexColorAvailable(m),
                textureAvailable,
                isPerVertexTangentAvailable(m)
            );
        }
        else {
            using enum Material::AlphaMode;

            uint materialId = Base::materialIndex(mrs, chunkNumber);

            if (materialId == UINT_NULL) {
                // fallback to default material
                mMaterialUniforms.update(
                    DEFAULT_MATERIAL,
                    isPerVertexColorAvailable(m),
                    textureAvailable,
                    isPerVertexTangentAvailable(m)
                );
            }
            else {
                assert(materialId < m.materialsNumber());
                const Material& mat = m.material(materialId);

                for (uint j = 0; j < N_TEXTURE_TYPES; ++j) {
                    const auto& td =
                        m.material(materialId).textureDescriptor(j);
                    const std::string& path = td.path();
                    if (!path.empty()) {
                        const Texture& tex = mMaterialTextures.at(path);
                        textureAvailable[j] = tex.isValid();
                    }
                }

                mMaterialUniforms.update(
                    m.material(materialId),
                    isPerVertexColorAvailable(m),
                    textureAvailable,
                    isPerVertexTangentAvailable(m)
                );

                // set the state according to the material
                if (!m.material(materialId).doubleSided()) {
                    // backface culling
                    state |= BGFX_STATE_CULL_CW;
                }
                if (m.material(materialId).alphaMode() == ALPHA_BLEND) {
                    state |= BGFX_STATE_BLEND_ALPHA;
                }
            }
        }

        mMaterialUniforms.bind();
        return state;
    }

    void updateEdgeSettings(const MeshRenderSettings& mrs)
    {
        using enum MeshRenderInfo::Edges;
        using enum Lines::ColorToUse;

        mEdgeLines.thickness() = mrs.edgesWidth();
        mEdgeLines.setShading(mrs.isEdges(SHADING_SMOOTH));

        if (mrs.isEdges(COLOR_USER)) {
            mEdgeLines.generalColor() = mrs.edgesUserColor();
            mEdgeLines.setColorToUse(GENERAL);
        }
        else if (mrs.isEdges(COLOR_MESH)) {
            mEdgeLines.generalColor() = mMeshColor;
            mEdgeLines.setColorToUse(GENERAL);
        }
        else if (mrs.isEdges(COLOR_VERTEX)) {
            mEdgeLines.setColorToUse(PER_VERTEX);
        }
        else if (mrs.isEdges(COLOR_EDGE)) {
            mEdgeLines.setColorToUse(PER_EDGE);
        }
    }

    void updateWireframeSettings(const MeshRenderSettings& mrs)
    {
        using enum MeshRenderInfo::Wireframe;
        using enum Lines::ColorToUse;

        mWireframeLines.thickness() = mrs.wireframeWidth();
        mWireframeLines.setShading(mrs.isWireframe(SHADING_VERT));

        if (mrs.isWireframe(COLOR_USER)) {
            mWireframeLines.generalColor() = mrs.wireframeUserColor();
            mWireframeLines.setColorToUse(GENERAL);
        }
        else if (mrs.isWireframe(COLOR_MESH)) {
            mWireframeLines.generalColor() = mMeshColor;
            mWireframeLines.setColorToUse(GENERAL);
        }
        else if (mrs.isWireframe(COLOR_VERTEX)) {
            mWireframeLines.setColorToUse(PER_VERTEX);
        }
    }

    void bindUniforms() const { mMeshUniforms.bind(); }

private:
    void setVertexPositionsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(nv * 3);

        Base::fillVertexPositions(mesh, buffer);

        mVertexPositionsBuffer.createForCompute(
            buffer,
            nv,
            bgfx::Attrib::Position,
            3,
            PrimitiveType::FLOAT,
            false,
            bgfx::Access::Read,
            releaseFn);

        // Creates the buffers to be used with compute for splatting
        if (Context::instance().supportsCompute()) {
            // create a layout <coordinates, colors, normals, float>
            // 2 X vec4
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 1, bgfx::AttribType::Float)
                .end();

            // create the dynamic vertex buffer for splatting
            mVertexQuadBuffer.create(
                mesh.vertexNumber() * 4, layout, BGFX_BUFFER_COMPUTE_WRITE);

            // create the index buffer for splatting
            setVertexQuadIndexBuffer(mesh);

            // record that the vertex quad buffer must be generated
            mVertexQuadBufferGenerated = false;
        }
    }

    /**
     * @brief The function allocates and fills a GPU index buffer to render
     * a quad for each vertex of the mesh.
     *
     * @param[in] mesh: the input mesh from which to get the data
     */
    void setVertexQuadIndexBuffer(const MeshType& mesh)
    {
        const uint totalIndices = mesh.vertexNumber() * 6;

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<uint>(totalIndices);

        Base::fillVertexQuadIndices(mesh, buffer);

        mVertexQuadIndexBuffer.create(buffer, totalIndices, true, releaseFn);

        // if number of vertices is not zero, the index buffer must be valid
        assert(mVertexQuadIndexBuffer.isValid() || totalIndices == 0);
    }

    void setVertexNormalsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(nv * 3);

        Base::fillVertexNormals(mesh, buffer);

        mVertexNormalsBuffer.createForCompute(
            buffer,
            nv,
            bgfx::Attrib::Normal,
            3,
            PrimitiveType::FLOAT,
            false,
            bgfx::Access::Read,
            releaseFn);
    }

    void setVertexColorsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint>(nv);

        Base::fillVertexColors(mesh, buffer, Color::Format::ABGR);

        mVertexColorsBuffer.createForCompute(
            buffer,
            nv,
            bgfx::Attrib::Color0,
            4,
            PrimitiveType::UCHAR,
            true,
            bgfx::Access::Read,
            releaseFn);
    }

    void setVertexTexCoordsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(nv * 2);

        Base::fillVertexTexCoords(mesh, buffer);

        mVertexUVBuffer.create(
            buffer,
            nv,
            bgfx::Attrib::TexCoord0,
            2,
            PrimitiveType::FLOAT,
            false,
            releaseFn);
    }

    void setVertexTangentsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(nv * 4);

        Base::fillVertexTangents(mesh, buffer);

        mVertexTangentsBuffer.create(
            buffer,
            nv,
            bgfx::Attrib::Tangent,
            4,
            PrimitiveType::FLOAT,
            false,
            releaseFn);
    }

    void setWedgeTexCoordsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(nv * 2);

        Base::fillWedgeTexCoords(mesh, buffer);

        mVertexWedgeUVBuffer.create(
            buffer,
            nv,
            bgfx::Attrib::TexCoord1,
            2,
            PrimitiveType::FLOAT,
            false,
            releaseFn);
    }

    void setTriangleIndicesBuffer(const MeshType& mesh) // override
    {
        uint nt = Base::numTris();

        auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint>(nt * 3);

        Base::fillTriangleIndices(mesh, buffer);

        mTriangleIndexBuffer.create(buffer, nt * 3, true, releaseFn);
    }

    void setTriangleNormalsBuffer(const MeshType& mesh) // override
    {
        uint nt = Base::numTris();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<float>(nt * 3);

        Base::fillTriangleNormals(mesh, buffer);

        mTriangleNormalBuffer.createForCompute(
            buffer,
            nt * 3,
            PrimitiveType::FLOAT,
            bgfx::Access::Read,
            releaseFn);
    }

    void setTriangleColorsBuffer(const MeshType& mesh) // override
    {
        uint nt = Base::numTris();

        auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint>(nt);

        Base::fillTriangleColors(mesh, buffer, Color::Format::ABGR);

        mTriangleColorBuffer.createForCompute(
            buffer, nt, PrimitiveType::UINT, bgfx::Access::Read, releaseFn);
    }

    void setEdgeIndicesBuffer(const MeshType& mesh) // override
    {
        computeEdgeLines(mesh);
    }

    void setWireframeIndicesBuffer(const MeshType& mesh) // override
    {
        computeWireframeLines(mesh);
    }

    void setTextures(const MeshType& mesh) // override
    {
        // lambda that sets a texture
        auto setTexture = [&](const Image&       img,
                              const std::string& path,
                              bool               generateMips) {
            const uint size = img.width() * img.height();
            assert(size > 0);

            uint sizeWithMips = bimg::imageGetSize(
                                    nullptr,
                                    img.width(),
                                    img.height(),
                                    1,
                                    false,
                                    generateMips,
                                    1,
                                    bimg::TextureFormat::RGBA8) /
                                4; // in uints
            uint numMips = 1;
            if (generateMips)
                numMips = bimg::imageGetNumMips(
                    bimg::TextureFormat::RGBA8, img.width(), img.height());

            auto [buffer, releaseFn] =
                getAllocatedBufferAndReleaseFn<uint>(sizeWithMips);

            const uint* tdata = reinterpret_cast<const uint*>(img.data());

            std::copy(tdata, tdata + size, buffer); // mip level 0

            if (numMips > 1) {
                uint* source = buffer;
                uint* dest;
                uint  offset = size;
                for (uint mip = 1; mip < numMips; mip++) {
                    dest         = source + offset;
                    uint mipSize = (img.width() >> mip) * (img.height() >> mip);
                    bimg::imageRgba8Downsample2x2(
                        dest,                      // output location
                        img.width() >> (mip - 1),  // input width
                        img.height() >> (mip - 1), // input height
                        1, // depth, always 1 for 2D textures
                        (img.width() >> (mip - 1)) * 4, // input pitch
                        (img.width() >> mip) * 4,       // output pitch
                        source                          // input location
                    );
                    source = dest;
                    offset = mipSize;
                }
            }

            uint64_t flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;

            if (img.colorSpace() == Image::ColorSpace::SRGB)
                flags |= BGFX_TEXTURE_SRGB;

            Texture tex;
            tex.set(
                buffer,
                vcl::Point2i(img.width(), img.height()),
                generateMips,
                flags,
                releaseFn);

            // at() does not insert if already present, thus safe in parallel
            mMaterialTextures.at(path) = std::move(tex);
        };

        auto loadImageAndSetTexture =
            [&](const std::pair<std::string, uint>& pathPair) {
                const std::string& path        = pathPair.first;

                uint materialId  = pathPair.second / N_TEXTURE_TYPES;
                uint textureType = pathPair.second % N_TEXTURE_TYPES;
                // copy the image because it could be not loaded,
                // and at the end it needs to be mirrored.
                vcl::Image txtImg = mesh.textureImage(path);
                if (txtImg.isNull()) { // try to load it just for rendering
                    try {
                        txtImg = vcl::loadImage(mesh.meshBasePath() + path);
                        txtImg.colorSpace() = Material::textureTypeToColorSpace(
                            static_cast<Material::TextureType>(textureType));
                    }
                    catch (...) {
                        // do nothing
                    }
                    if (txtImg.isNull()) {
                        // still null, use a dummy texture
                        txtImg = createCheckBoardImage(512);
                    }
                }

                // if loading succeeded (or dummy texture has been created)
                if (!txtImg.isNull()) {
                    const TextureDescriptor& tex =
                        mesh.material(materialId)
                            .textureDescriptor(textureType);
                    using enum TextureDescriptor::MinificationFilter;

                    TextureDescriptor::MinificationFilter minFilter = tex.minFilter();

                    bool hasMips =
                        minFilter >= NEAREST_MIPMAP_NEAREST ||
                        minFilter == NONE; // default LINEAR_MIPMAP_LINEAR

                    txtImg.mirror();
                    setTexture(txtImg, path, hasMips);
                }
            };

        mMaterialTextures.clear();

        if constexpr (vcl::HasMaterials<MeshType>) {
            // textures could be missing from the textureImages of the mesh
            // setting the texture paths in a map - key is the path and value is
            // an uint where materialIndex and textureType are encoded
            // map is used to avoid duplicates, then is moved to a vector for
            // parallel processing
            std::map<std::string, uint> texturePaths;
            for (uint i = 0; i < mesh.materialsNumber(); ++i) {
                for (uint j = 0; j < N_TEXTURE_TYPES; ++j) {
                    const vcl::TextureDescriptor& td =
                        mesh.material(i).textureDescriptor(j);
                    if (!td.path().empty()) {
                        texturePaths[td.path()] = i * N_TEXTURE_TYPES + j;

                        // create a null texture in the map
                        // this is crucial to avoid insertions during the
                        // actual creation, that is done in parallel
                        mMaterialTextures[td.path()] = Texture();
                    }
                }
            }

            // move to vector for parallel processing
            std::vector<std::pair<std::string, uint>> texturePathVec;
            texturePathVec.reserve(texturePaths.size());
            for (const auto& tp : texturePaths) {
                texturePathVec.push_back(tp);
            }

            parallelFor(texturePathVec, loadImageAndSetTexture);

            createTextureSamplerUniforms();
        }
    }

    void setMeshUniforms(const MeshType& mesh) // override
    {
        mMeshUniforms.update(mesh);
        if constexpr (HasColor<MeshType>) {
            mMeshColor = mesh.color();
        }
    }

    void computeEdgeLines(const MeshType& mesh)
    {
        // if cpu lines, do this...

        // positions
        const uint         nv = Base::numVerts();
        std::vector<float> positions(nv * 3);
        Base::fillVertexPositions(mesh, positions.data());

        // indices
        const uint        ne = Base::numEdges();
        std::vector<uint> indices(ne * 2);
        Base::fillEdgeIndices(mesh, indices.data());

        // v normals
        std::vector<float> normals;
        if (mVertexNormalsBuffer.isValid()) {
            normals.resize(nv * 3);
            Base::fillVertexNormals(mesh, normals.data());
        }

        // todo - edge normals

        // vcolors
        std::vector<uint> vcolors;
        if (mVertexColorsBuffer.isValid()) {
            vcolors.resize(nv);
            Base::fillVertexColors(mesh, vcolors.data(), Color::Format::ABGR);
        }

        std::vector<uint> ecolors;
        if constexpr (vcl::HasPerEdgeColor<MeshType>) {
            if (vcl::isPerEdgeColorAvailable(mesh)) {
                // if (btu[toUnderlying(EDGE_COLORS)]) {
                //  edge color buffer
                ecolors.resize(ne);
                Base::fillEdgeColors(mesh, ecolors.data(), Color::Format::ABGR);
                //}
            }
        }

        mEdgeLines.setPoints(positions, indices, normals, vcolors, ecolors);

        // otherwise, already computed buffers should do the job
    }

    // to generate wireframe lines
    void computeWireframeLines(const MeshType& mesh)
    {
        // if cpu lines, do this...

        // positions
        const uint         nv = Base::numVerts();
        std::vector<float> positions(nv * 3);
        Base::fillVertexPositions(mesh, positions.data());

        // indices
        const uint        nw = Base::numWireframeLines();
        std::vector<uint> indices(nw * 2);
        Base::fillWireframeIndices(mesh, indices.data());

        // v normals
        std::vector<float> normals;
        if (mVertexNormalsBuffer.isValid()) {
            normals.resize(nv * 3);
            Base::fillVertexNormals(mesh, normals.data());
        }

        // vcolors
        std::vector<uint> vcolors;
        if (mVertexColorsBuffer.isValid()) {
            vcolors.resize(nv);
            Base::fillVertexColors(mesh, vcolors.data(), Color::Format::ABGR);
        }

        mWireframeLines.setPoints(positions, indices, normals, vcolors, {});

        // otherwise, already computed buffers should do the job
    }

    void createTextureSamplerUniforms()
    {
        for (uint i = 0; i < mTextureSamplerUniforms.size(); ++i) {
            mTextureSamplerUniforms[i] = Uniform(
                Material::TEXTURE_TYPE_NAMES[i].c_str(),
                bgfx::UniformType::Sampler);
        }
    }

    template<typename T>
    std::pair<T*, bgfx::ReleaseFn> getAllocatedBufferAndReleaseFn(uint size)
    {
        T* buffer = new T[size];

        return std::make_pair(buffer, [](void* ptr, void*) {
            delete[] static_cast<T*>(ptr);
        });
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_H
