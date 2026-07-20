// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_H
#define VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_H

#include "mesh_render_buffers_macros.h"
#include "mesh_selection_buffers.h"

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/uniforms/drawable_mesh_uniforms.h>
#include <vclib/bgfx/drawable/uniforms/material_uniforms.h>
#include <vclib/bgfx/primitives/lines.h>
#include <vclib/bgfx/primitives/points.h>
#include <vclib/bgfx/texture.h>

#include <vclib/render/drawable/mesh/mesh_render_data.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>

#include <vclib/io.h>

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

    Points mPoints;

    MeshSelectionBuffers mSelection;

    IndexBuffer  mTriangleIndexBuffer;
    VertexBuffer mTriangleNormalBuffer;
    VertexBuffer mTriangleColorBuffer;

    Lines mEdgeLines;

    Lines mWireframeLines;
    Color mMeshColor; // todo: find better way to store mesh color

    // map of textures
    // for each texture path of each material, store its texture
    std::map<std::string, Texture> mMaterialTextures;

    static inline std::array<Uniform, N_TEXTURE_TYPES> sTextureSamplerUniforms;

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
        swap(mPoints, other.mPoints);
        swap(mTriangleIndexBuffer, other.mTriangleIndexBuffer);
        swap(mTriangleNormalBuffer, other.mTriangleNormalBuffer);
        swap(mTriangleColorBuffer, other.mTriangleColorBuffer);
        swap(mEdgeLines, other.mEdgeLines);
        swap(mWireframeLines, other.mWireframeLines);
        swap(mSelection, other.mSelection);
        swap(mMaterialTextures, other.mMaterialTextures);

        updatePointsVertexBuffers(*this, mPoints);
        updatePointsVertexBuffers(other, other.mPoints);
    }

    friend void swap(MeshRenderBuffers& a, MeshRenderBuffers& b) { a.swap(b); }

    uint selectedVertexCount() const
    {
        return mSelection.selectedVertexCount();
    }

    uint selectedFaceCount() const { return mSelection.selectedFaceCount(); }

    // called on computeSelection
    void computeSelection(
        const SelectionParameters& params,
        const Matrix44f&           model)
    {
        mSelection.computeSelection(
            params, model, mVertexPositionsBuffer, mTriangleIndexBuffer);
    }

    // called on draw
    template<MeshConcept MeshType>
    bool selectionReadback(MeshType& m)
    {
        return mSelection.selectionReadback(m, Base::triPolyIndexMap());
    }

    bool isSelectionReadbackPending() const
    {
        return mSelection.isSelectionReadbackPending();
    }

    void bindSelectedVerticesBuffer() const
    {
        mSelection.bindSelectedVerticesBuffer();
    }

    void bindSelectedFacesBuffer() const
    {
        mSelection.bindSelectedFacesBuffer();
    }

    void bindVertexBuffers(const MeshRenderSettings& mrs) const
    {
        // TODO: streams cannot be higher than
        // "bgfx::getCaps()->limits.maxVertexStreams". Buffers must be bound
        // only if necessary (using MeshRenderSettings) right now, this is
        // managed only for uvs (per vertex or per wedge, not both).
        // We MUST be sure that the limit is not exceeded.

        using enum MeshRenderInfo::Surface;

        uint stream = 0;

        // streams MUST be consecutive starting from 0
        // otherwise on metal it won't work
        mVertexPositionsBuffer.bind(stream++);

        if (mVertexNormalsBuffer.isValid()) {
            // bgfx limitation
            assert(stream < bgfx::getCaps()->limits.maxVertexStreams);
            mVertexNormalsBuffer.bind(stream++);
        }

        if (mVertexTangentsBuffer.isValid()) {
            // bgfx limitation
            assert(stream < bgfx::getCaps()->limits.maxVertexStreams);
            mVertexTangentsBuffer.bind(stream++);
        }

        if (mVertexColorsBuffer.isValid()) {
            // bgfx limitation
            assert(stream < bgfx::getCaps()->limits.maxVertexStreams);
            mVertexColorsBuffer.bind(stream++);
        }

        if (mVertexUVBuffer.isValid() && mrs.isSurface(COLOR_VERTEX_TEX)) {
            // bgfx limitation
            assert(stream < bgfx::getCaps()->limits.maxVertexStreams);
            mVertexUVBuffer.bind(stream++);
        }

        if (mVertexWedgeUVBuffer.isValid() && mrs.isSurface(COLOR_WEDGE_TEX)) {
            // bgfx limitation
            assert(stream < bgfx::getCaps()->limits.maxVertexStreams);
            mVertexWedgeUVBuffer.bind(stream++);
        }
    }

    void bindIndexBuffers(
        const MeshRenderSettings& mrs,
        uint                      chunkToBind = UINT_NULL) const
    {
        using enum MRI::Buffers;

        if (chunkToBind == UINT_NULL) {
            mTriangleIndexBuffer.bind();
        }
        else {
            const auto& chunk = Base::triangleChunk(chunkToBind);
            mTriangleIndexBuffer.bind(
                chunk.startIndex * 3, chunk.indexCount * 3);
        }

        mTriangleNormalBuffer.bindCompute(VCL_MRB_PRIMITIVE_NORMAL_BUFFER);

        mTriangleColorBuffer.bindCompute(VCL_MRB_PRIMITIVE_COLOR_BUFFER);
    }

    void drawEdgeLines(uint viewId) const { mEdgeLines.draw(viewId); }

    void drawWireframeLines(uint viewId) const { mWireframeLines.draw(viewId); }

    void drawPoints(uint viewId) const { mPoints.draw(viewId); }

    void drawPointsId(uint viewId, uint32_t id) const
    {
        mPoints.drawId(viewId, id);
    }

    /**
     * @brief Binds the textures associated to the material of the given
     * triangle chunk. Returns the number of bound textures.
     *
     * @param[in] mrs: the mesh render settings, needed to identify the material
     * index to use (per vertex or per face)
     * @param[in] chunkNumber: the triangle chunk number
     * @param[in] m: the mesh
     * @return the number of bound textures
     */
    uint bindTextures(
        const MeshRenderSettings& mrs,
        uint                      chunkNumber,
        const MeshType&           m) const
    {
        uint materialId = Base::materialIndex(mrs, chunkNumber);

        uint boundTextures = 0;

        DrawableMeshUniforms::TextureType tt =
            DrawableMeshUniforms::TextureType::BASE_COLOR;

        if (materialId != UINT_NULL) {
            for (uint j = 0; j < N_TEXTURE_TYPES; ++j) {
                const auto& td = m.material(materialId).textureDescriptor(j);
                const std::string& path = td.path();
                if (!path.empty()) {
                    const Texture& tex = mMaterialTextures.at(path);
                    if (tex.isValid()) {
                        uint flags = Texture::samplerFlagsFromTexture(td);
                        tex.bind(
                            boundTextures,
                            sTextureSamplerUniforms[j].handle(),
                            flags);

                        tt = static_cast<DrawableMeshUniforms::TextureType>(j);
                        DrawableMeshUniforms::setTextureStage(
                            tt, boundTextures);
                        boundTextures++;
                    }
                }
            }
        }
        return boundTextures;
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

    /**
     * @brief Updates the points primitive settings based on the mesh render
     * settings.
     *
     * @param[in] mrs: The mesh render settings to extract point settings from.
     */
    void updatePointsSettings(const MeshRenderSettings& mrs)
    {
        using enum MeshRenderInfo::Points;
        using enum Points::ColorSetting;

        mPoints.setSize(mrs.pointWidth());
        mPoints.setDepthOffset(0.00011f);

        if (mrs.isPoints(SHADING_VERT)) {
            mPoints.setShading(Points::Shading::PER_VERTEX);
        }
        else {
            mPoints.setShading(Points::Shading::NONE);
        }

        if (mrs.isPoints(SHAPE_CIRCLE) || mrs.isPoints(SHAPE_SPHERE)) {
            mPoints.setShape(Points::Shape::CIRCLE);
        }
        else {
            mPoints.setShape(Points::Shape::SQUARE);
        }

        if (mrs.isPoints(COLOR_USER)) {
            mPoints.setGeneralColor(mrs.pointUserColor());
            mPoints.setColorSetting(GENERAL);
        }
        else if (mrs.isPoints(COLOR_MESH)) {
            mPoints.setGeneralColor(mMeshColor);
            mPoints.setColorSetting(GENERAL);
        }
        else if (mrs.isPoints(COLOR_VERTEX)) {
            mPoints.setColorSetting(PER_VERTEX);
        }

        mPoints.setSelectionVisibility(mrs.isPoints(SELECTION));
        mPoints.setSelectionColor(mrs.pointSelectionColor());
    }

private:
    void setVertexPositionsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nv * 3);

        Base::fillVertexPositions(mesh, buffer);

        mVertexPositionsBuffer.create(
            buffer,
            nv,
            bgfx::Attrib::Position,
            3,
            PrimitiveType::FLOAT,
            releaseFn);

        mPoints.setVertices(nv, mVertexPositionsBuffer);

        // create the vertex selection buffer
        mSelection.initVertexSelectionBitfield(nv);
        mPoints.setSelection(nv, mSelection.vertexSelectionBuffer());

        // create the face selection buffer
        mSelection.initFaceSelectionBitfield(Base::numTris());

        mSelection.initReadbackHandler(
            uint(max(double(Base::numVerts()), double(Base::numTris()))));
    }

    void setVertexSelectionBuffer(const MeshType& mesh) // override
    {
        mSelection.setVertexSelectionFromMesh(mesh);
    }

    void setVertexNormalsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nv * 3);

        Base::fillVertexNormals(mesh, buffer);

        mVertexNormalsBuffer.create(
            buffer,
            nv,
            bgfx::Attrib::Normal,
            3,
            PrimitiveType::FLOAT,
            releaseFn);
        mPoints.setVertexNormals(nv, mVertexNormalsBuffer);
    }

    void setVertexColorsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(nv);

        Base::fillVertexColors(mesh, buffer, Color::Format::ABGR);

        mVertexColorsBuffer.create(
            buffer,
            nv,
            bgfx::Attrib::Color0,
            4,
            PrimitiveType::UCHAR,
            true,
            releaseFn);
        mPoints.setVertexColors(nv, mVertexColorsBuffer);
    }

    void setVertexTexCoordsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nv * 2);

        Base::fillVertexTexCoords(mesh, buffer);

        mVertexUVBuffer.create(
            buffer,
            nv,
            bgfx::Attrib::TexCoord0,
            2,
            PrimitiveType::FLOAT,
            releaseFn);
    }

    void setVertexTangentsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nv * 4);

        Base::fillVertexTangents(mesh, buffer);

        mVertexTangentsBuffer.create(
            buffer,
            nv,
            bgfx::Attrib::Tangent,
            4,
            PrimitiveType::FLOAT,
            releaseFn);
    }

    void setWedgeTexCoordsBuffer(const MeshType& mesh) // override
    {
        uint nv = Base::numVerts();

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nv * 2);

        Base::fillWedgeTexCoords(mesh, buffer);

        mVertexWedgeUVBuffer.create(
            buffer,
            nv,
            bgfx::Attrib::TexCoord1,
            2,
            PrimitiveType::FLOAT,
            releaseFn);
    }

    void setTriangleIndicesBuffer(const MeshType& mesh) // override
    {
        uint nt = Base::numTris();

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(nt * 3);

        Base::fillTriangleIndices(mesh, buffer);

        // Re-read the actual triangle count: fillTriangleIndices() updates
        // mNumTris to the real earCut result, which may be smaller than the
        // pre-triangulation estimate for meshes with degenerate faces.
        nt = Base::numTris();

        mTriangleIndexBuffer.create(buffer, nt * 3, releaseFn);

        // Build polygon mapping buffers for polygon-level face selection.
        // fillTriangleIndices() above has already populated
        // Base::triPolyIndexMap().
        if (Context::instance().supportsCompute() && nt > 0) {
            mSelection.initPolyMapping(Base::triPolyIndexMap(), nt);
        }
    }

    void setTriangleSelectionBuffer(const MeshType& mesh) // override
    {
        mSelection.setFaceSelectionFromMesh(mesh, Base::triPolyIndexMap());
    }

    void setTriangleNormalsBuffer(const MeshType& mesh) // override
    {
        uint nt = Base::numTris();

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<float>(nt * 3);

        Base::fillTriangleNormals(mesh, buffer);

        mTriangleNormalBuffer.create(
            buffer,
            nt,
            bgfx::Attrib::Normal,
            3,
            PrimitiveType::FLOAT,
            releaseFn);
    }

    void setTriangleColorsBuffer(const MeshType& mesh) // override
    {
        uint nt = Base::numTris();

        auto [buffer, releaseFn] =
            Context::getAllocatedBufferAndReleaseFn<uint>(nt);

        Base::fillTriangleColors(mesh, buffer, Color::Format::ABGR);

        mTriangleColorBuffer.create(
            buffer,
            nt,
            bgfx::Attrib::Color0,
            4,
            PrimitiveType::UCHAR,
            true,
            releaseFn);
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
        auto downsampleNormalMap2x2 = [](const uint8_t* src,
                                         uint8_t*       dst,
                                         uint           srcWidth,
                                         uint           srcHeight,
                                         uint           dstWidth,
                                         uint           dstHeight) {
            for (uint y = 0; y < dstHeight; ++y) {
                for (uint x = 0; x < dstWidth; ++x) {
                    uint sx = x * 2;
                    uint sy = y * 2;

                    uint sx1 = std::min(sx + 1, srcWidth - 1);
                    uint sy1 = std::min(sy + 1, srcHeight - 1);

                    // Fetch the 4 neighboring pixels from the higher resolution
                    // mip level
                    const uint8_t* p00 = src + (sy * srcWidth + sx) * 4;
                    const uint8_t* p10 = src + (sy * srcWidth + sx1) * 4;
                    const uint8_t* p01 = src + (sy1 * srcWidth + sx) * 4;
                    const uint8_t* p11 = src + (sy1 * srcWidth + sx1) * 4;

                    float nx = 0.0f, ny = 0.0f, nz = 0.0f, alpha = 0.0f;
                    const uint8_t* pixels[4] = {p00, p10, p01, p11};

                    for (int i = 0; i < 4; ++i) {
                        // Remap from [0, 255] to [-1.0, 1.0] to get the true
                        // normal vector direction
                        nx += (pixels[i][0] / 255.0f) * 2.0f - 1.0f;
                        ny += (pixels[i][1] / 255.0f) * 2.0f - 1.0f;
                        nz += (pixels[i][2] / 255.0f) * 2.0f - 1.0f;
                        // Accumulate alpha for standard bilinear interpolation
                        alpha += pixels[i][3];
                    }

                    // Normalize the averaged normal vector to preserve unit
                    // length
                    float len = std::sqrt(nx * nx + ny * ny + nz * nz);
                    if (len > 0.0001f) {
                        nx /= len;
                        ny /= len;
                        nz /= len;
                    }
                    else {
                        // Fallback to pointing upwards if the vector is
                        // degenerate
                        nx = 0.0f;
                        ny = 0.0f;
                        nz = 1.0f;
                    }

                    // Remap the normalized vector back to [0, 255] and write to
                    // destination
                    uint8_t* out = dst + (y * dstWidth + x) * 4;
                    out[0]       = static_cast<uint8_t>(std::max(
                        0.0f,
                        std::min(255.0f, (nx * 0.5f + 0.5f) * 255.0f + 0.5f)));
                    out[1]       = static_cast<uint8_t>(std::max(
                        0.0f,
                        std::min(255.0f, (ny * 0.5f + 0.5f) * 255.0f + 0.5f)));
                    out[2]       = static_cast<uint8_t>(std::max(
                        0.0f,
                        std::min(255.0f, (nz * 0.5f + 0.5f) * 255.0f + 0.5f)));
                    out[3]       = static_cast<uint8_t>(
                        std::max(0.0f, std::min(255.0f, alpha / 4.0f + 0.5f)));
                }
            }
        };

        // lambda that sets a texture
        auto setTexture = [&](const Image&       img,
                              const std::string& path,
                              bool               generateMips,
                              bool               isNormalMap) {
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
            uint numMips      = 1;
            if (generateMips)
                numMips = bimg::imageGetNumMips(
                    bimg::TextureFormat::RGBA8, img.width(), img.height());

            auto [buffer, releaseFn] =
                Context::getAllocatedBufferAndReleaseFn<uint>(sizeWithMips);

            const uint* tdata = reinterpret_cast<const uint*>(img.data());

            std::copy(tdata, tdata + size, buffer); // mip level 0

            if (numMips > 1) {
                uint* source = buffer;
                uint* dest;
                uint  offset = size;

                for (uint mip = 1; mip < numMips; mip++) {
                    dest = source + offset;

                    uint srcWidth  = std::max(1, img.width() >> (mip - 1));
                    uint srcHeight = std::max(1, img.height() >> (mip - 1));
                    uint dstWidth  = std::max(1, img.width() >> mip);
                    uint dstHeight = std::max(1, img.height() >> mip);
                    uint mipSize   = dstWidth * dstHeight;

                    if (isNormalMap) {
                        const uint8_t* src8 =
                            reinterpret_cast<const uint8_t*>(source);
                        uint8_t* dst8 = reinterpret_cast<uint8_t*>(dest);
                        downsampleNormalMap2x2(
                            src8,
                            dst8,
                            srcWidth,
                            srcHeight,
                            dstWidth,
                            dstHeight);
                    }
                    else {
                        bimg::imageRgba8Downsample2x2(
                            dest,         // output location
                            srcWidth,     // input width
                            srcHeight,    // input height
                            1,            // depth, always 1 for 2D textures
                            srcWidth * 4, // input pitch
                            dstWidth * 4, // output pitch
                            source        // input location
                        );
                    }

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
                bgfx::TextureFormat::RGBA8,
                false,
                releaseFn);

            // at() does not insert if already present, thus safe in parallel
            mMaterialTextures.at(path) = std::move(tex);
        };

        auto loadImageAndSetTexture = [&](const std::pair<std::string, uint>&
                                              pathPair) {
            const std::string& path = pathPair.first;

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
                    mesh.material(materialId).textureDescriptor(textureType);
                using enum TextureDescriptor::MinificationFilter;

                TextureDescriptor::MinificationFilter minFilter =
                    tex.minFilter();

                bool hasMips =
                    minFilter >= NEAREST_MIPMAP_NEAREST ||
                    minFilter == NONE; // default LINEAR_MIPMAP_LINEAR

                txtImg.mirror();
                setTexture(
                    txtImg,
                    path,
                    hasMips,
                    textureType == toUnderlying(Material::TextureType::NORMAL));
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
            for (uint i = 0; i < mesh.materialCount(); ++i) {
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

    void setMeshAdditionalData(const MeshType& mesh) // override
    {
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

        // to avoid z-fighting with filled triangles
        mEdgeLines.depthOffset() = 0.0001f;

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

        // to avoid z-fighting with filled triangles
        mWireframeLines.depthOffset() = 0.0001f;

        // otherwise, already computed buffers should do the job
    }

    /**
     * @brief Updates the internal buffer references in the given Points object.
     *
     * This must be called after a swap to update the internal buffer references
     * in Points. OwnedOrRefBuffer stores a pointer to the VertexBuffer object,
     * so after swapping MeshRenderBuffers, the pointers in Points would still
     * point to the other MeshRenderBuffers's members. We must unconditionally
     * update them, even if they are invalid, to ensure they point to the
     * correct memory addresses within this object.
     *
     * @param[in] mrb: The mesh render buffers object to read vertices from.
     * @param[in,out] points: The points primitive object to update.
     */
    static void updatePointsVertexBuffers(
        const MeshRenderBuffers<MeshType>& mrb,
        Points&                            points)
    {
        uint nv = mrb.numVerts();
        points.setVertices(nv, mrb.mVertexPositionsBuffer);
        points.setVertexNormals(nv, mrb.mVertexNormalsBuffer);
        points.setVertexColors(nv, mrb.mVertexColorsBuffer);
        points.setSelection(nv, mrb.mSelection.vertexSelectionBuffer());
    }

    static void createTextureSamplerUniforms()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sTextureSamplerUniforms[0].isValid()) {
            for (uint i = 0; i < sTextureSamplerUniforms.size(); ++i) {
                sTextureSamplerUniforms[i] = Uniform(
                    ("s_tex" + std::to_string(i)).c_str(),
                    bgfx::UniformType::Sampler);
            }
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_H
