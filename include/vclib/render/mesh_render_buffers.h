/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_RENDER_MESH_RENDER_BUFFERS_H
#define VCL_RENDER_MESH_RENDER_BUFFERS_H

#include <vclib/algorithms/polygon.h>
#include <vclib/math/min_max.h>
#include <vclib/mesh/requirements.h>
#include <vclib/mesh/utils/tri_poly_index_bimap.h>
#include <vclib/space/image.h>

#include "mesh_render_settings.h"

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderBuffers
{
    std::vector<float>    verts;
    std::vector<uint32_t> tris;
    std::vector<float>    vNormals;
    std::vector<float>    vColors;
    std::vector<float>    tNormals;
    std::vector<float>    tColors;
    std::vector<float>    vTexCoords;
    std::vector<float>    wTexCoords;
    std::vector<short>    wTexIds;
    std::array<float, 4>  mColor;

    vcl::Point3d            bbmin, bbmax;
    vcl::TriPolyIndexBiMap  triPolyMap;
    std::vector<vcl::Image> textures;

public:
    MeshRenderBuffers() = default;

    MeshRenderBuffers(const MeshType& m)
    {
        fillVertices(m);
        fillTriangles(m);
        fillTextures(m);
        fillMeshAttribs(m);
    }

    uint vertexNumber() const { return verts.size() / 3; }

    uint triangleNumber() const { return tris.size() / 3; }

    uint textureNumber() const { return textures.size(); }

    vcl::Point2i textureSize(uint ti) const
    {
        return vcl::Point2i(textures[ti].width(), textures[ti].height());
    }

    const vcl::Point3d& bbMin() const { return bbmin; }

    const vcl::Point3d& bbMax() const { return bbmax; }

    const float* vertexBufferData() const
    {
        if (verts.empty())
            return nullptr;
        return verts.data();
    }

    const uint vertexBufferSize() const
    {
        return verts.size();
    }

    const uint32_t* triangleBufferData() const
    {
        if (tris.empty())
            return nullptr;
        return tris.data();
    }

    const uint triangleBufferSize() const
    {
        return tris.size();
    }

    const float* vertexNormalBufferData() const
    {
        if (vNormals.empty())
            return nullptr;
        return vNormals.data();
    }

    const float* vertexColorBufferData() const
    {
        if (vColors.empty())
            return nullptr;
        return vColors.data();
    }

    const float* triangleNormalBufferData() const
    {
        if (tNormals.empty())
            return nullptr;
        return tNormals.data();
    }

    const float* triangleColorBufferData() const
    {
        if (tColors.empty())
            return nullptr;
        return tColors.data();
    }

    const float* vertexTexCoordsBufferData() const
    {
        if (vTexCoords.empty())
            return nullptr;
        return vTexCoords.data();
    }

    const float* wedgeTexCoordsBufferData() const
    {
        if (wTexCoords.empty())
            return nullptr;
        return wTexCoords.data();
    }

    const short* wedgeTextureIDsBufferData() const
    {
        if (wTexIds.empty())
            return nullptr;
        return wTexIds.data();
    }

    const float* meshColorBufferData() const { return mColor.data(); }

    const unsigned char* textureBufferData(uint ti) const
    {
        return textures[ti].data();
    }

private:
    void fillVertices(const MeshType& m)
    {
        // not using Mesh's bounding box if:
        // - it has not bounding box, or
        // - it has bounding box, but it is null (not valid)
        bool bbToInitialize = !vcl::HasBoundingBox<MeshType>;
        if constexpr (vcl::HasBoundingBox<MeshType>) {
            if (m.boundingBox().isNull()) {
                bbToInitialize = true;
            }
        }

        // if mesh has bounding box, I set it anyway from its bb
        if constexpr (vcl::HasBoundingBox<MeshType>) {
            bbmin = m.boundingBox().min().template cast<double>();
            bbmax = m.boundingBox().max().template cast<double>();
        }
        // if I need to compute bb, I initialize to invalid numbers
        if (bbToInitialize) {
            bbmin = Point3d(
                std::numeric_limits<double>::max(),
                std::numeric_limits<double>::max(),
                std::numeric_limits<double>::max());
            bbmax = Point3d(
                std::numeric_limits<double>::lowest(),
                std::numeric_limits<double>::lowest(),
                std::numeric_limits<double>::lowest());
        }

        verts.resize(m.vertexNumber() * 3);

        if constexpr (vcl::HasPerVertexNormal<MeshType>) {
            if (vcl::isPerVertexNormalAvailable(m)) {
                vNormals.resize(m.vertexNumber() * 3);
            }
        }

        if constexpr (vcl::HasPerVertexColor<MeshType>) {
            if (vcl::isPerVertexColorAvailable(m)) {
                vColors.resize(m.vertexNumber() * 3);
            }
        }

        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
            if (vcl::isPerVertexTexCoordAvailable(m)) {
                vTexCoords.resize(m.vertexNumber() * 2);
            }
        }

        uint vi = 0;
        for (const auto& v : m.vertices()) {
            uint i = vi * 2; // use i for size 2, like texcoords
            uint j = vi * 3; // use j for size 3, like coords and normals

            verts[j + 0] = v.coord().x();
            verts[j + 1] = v.coord().y();
            verts[j + 2] = v.coord().z();

            if (bbToInitialize) {
                bbmin = vcl::min(bbmin, v.coord().template cast<double>());
                bbmax = vcl::max(bbmax, v.coord().template cast<double>());
            }

            if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                if (vcl::isPerVertexNormalAvailable(m)) {
                    vNormals[j + 0] = v.normal().x();
                    vNormals[j + 1] = v.normal().y();
                    vNormals[j + 2] = v.normal().z();
                }
            }

            if constexpr (vcl::HasPerVertexColor<MeshType>) {
                if (vcl::isPerVertexColorAvailable(m)) {
                    vColors[j + 0] = v.color().redF();
                    vColors[j + 1] = v.color().greenF();
                    vColors[j + 2] = v.color().blueF();
                }
            }

            if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
                if (vcl::isPerVertexTexCoordAvailable(m)) {
                    vTexCoords[i + 0] = v.texCoord().u();
                    vTexCoords[i + 1] = v.texCoord().v();
                }
            }

            vi++;
        }
    }

    void fillTriangles(const MeshType& m)
    {
        if constexpr (vcl::HasFaces<MeshType>) {
            std::vector<std::vector<uint>> vinds; // necessary for wedge attribs

            if constexpr (vcl::HasTriangles<MeshType>) {
                tris.resize(m.faceNumber() * 3);

                uint i = 0;
                for (const auto& f : m.faces()) {
                    tris[i + 0] = m.vertexIndexIfCompact(m.index(f.vertex(0)));
                    tris[i + 1] = m.vertexIndexIfCompact(m.index(f.vertex(1)));
                    tris[i + 2] = m.vertexIndexIfCompact(m.index(f.vertex(2)));
                    i += 3;
                }
            }
            else {
                vinds.reserve(m.faceNumber());
                triPolyMap.reserve(m.faceNumber(), m.faceNumber());
                tris.reserve(m.faceNumber());

                uint nt = 0;
                for (const auto& f : m.faces()) {
                    if (f.vertexNumber() == 3) {
                        triPolyMap.insert(nt, m.faceIndexIfCompact(m.index(f)));
                        tris.push_back(
                            m.vertexIndexIfCompact(m.index(f.vertex(0))));
                        tris.push_back(
                            m.vertexIndexIfCompact(m.index(f.vertex(1))));
                        tris.push_back(
                            m.vertexIndexIfCompact(m.index(f.vertex(2))));
                        nt += 1;
                        vinds.push_back({0, 1, 2});
                    }
                    else {
                        std::vector<uint> vind = vcl::earCut(f);
                        for (uint vi = 0; vi < vind.size(); vi += 3) {
                            triPolyMap.insert(
                                nt + vi / 3, m.faceIndexIfCompact(m.index(f)));
                            tris.push_back(m.vertexIndexIfCompact(
                                m.index(f.vertex(vind[vi + 0]))));
                            tris.push_back(m.vertexIndexIfCompact(
                                m.index(f.vertex(vind[vi + 1]))));
                            tris.push_back(m.vertexIndexIfCompact(
                                m.index(f.vertex(vind[vi + 2]))));
                        }
                        nt += vind.size() / 3;
                        vinds.push_back(vind);
                    }
                }
            }

            tNormals.reserve(m.faceNumber() * 3);

            if constexpr (vcl::HasPerFaceColor<MeshType>) {
                if (vcl::isPerFaceColorAvailable(m)) {
                    tColors.reserve(m.faceNumber() * 3);
                }
            }

            if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                if (vcl::isPerFaceWedgeTexCoordsAvailable(m)) {
                    wTexCoords.reserve(m.faceNumber() * 3 * 2);
                    wTexIds.reserve(m.faceNumber());
                }
            }

            for (const auto& f : m.faces()) {
                if constexpr (vcl::HasPerFaceNormal<MeshType>) {
                    if (vcl::isPerFaceNormalAvailable(m)) {
                        if constexpr (vcl::HasTriangles<MeshType>) {
                            tNormals.push_back(f.normal().x());
                            tNormals.push_back(f.normal().y());
                            tNormals.push_back(f.normal().z());
                        }
                        else {
                            const uint fi = m.faceIndexIfCompact(m.index(f));
                            for (uint i = 0; i < triPolyMap.triangleNumber(fi);
                                 i++)
                            {
                                tNormals.push_back(f.normal().x());
                                tNormals.push_back(f.normal().y());
                                tNormals.push_back(f.normal().z());
                            }
                        }
                    }
                    else {
                        fillFaceNormals(
                            f,
                            vcl::HasTriangles<MeshType>,
                            m.faceIndexIfCompact(m.index(f)));
                    }
                }
                else {
                    fillFaceNormals(
                        f,
                        vcl::HasTriangles<MeshType>,
                        m.faceIndexIfCompact(m.index(f)));
                }

                if constexpr (vcl::HasPerFaceColor<MeshType>) {
                    if (vcl::isPerFaceColorAvailable(m)) {
                        if constexpr (vcl::HasTriangles<MeshType>) {
                            tColors.push_back(f.color().redF());
                            tColors.push_back(f.color().greenF());
                            tColors.push_back(f.color().blueF());
                        }
                        else {
                            const uint fi = m.faceIndexIfCompact(m.index(f));
                            for (uint i = 0; i < triPolyMap.triangleNumber(fi);
                                 i++)
                            {
                                tColors.push_back(f.color().redF());
                                tColors.push_back(f.color().greenF());
                                tColors.push_back(f.color().blueF());
                            }
                        }
                    }
                }

                if constexpr (vcl::HasPerFaceWedgeTexCoords<MeshType>) {
                    if (vcl::isPerFaceWedgeTexCoordsAvailable(m)) {
                        if constexpr (vcl::HasTriangles<MeshType>) {
                            wTexCoords.push_back(f.wedgeTexCoord(0).u());
                            wTexCoords.push_back(f.wedgeTexCoord(0).v());
                            wTexCoords.push_back(f.wedgeTexCoord(1).u());
                            wTexCoords.push_back(f.wedgeTexCoord(1).v());
                            wTexCoords.push_back(f.wedgeTexCoord(2).u());
                            wTexCoords.push_back(f.wedgeTexCoord(2).v());
                            wTexIds.push_back(f.textureIndex());
                        }
                        else {
                            const uint fi = m.faceIndexIfCompact(m.index(f));
                            // triangulation of f
                            const std::vector<uint>& vind = vinds[fi];
                            // for each triangle of f
                            for (uint vi = 0; vi < vind.size(); vi += 3) {
                                wTexCoords.push_back(
                                    f.wedgeTexCoord(vind[vi + 0]).u());
                                wTexCoords.push_back(
                                    f.wedgeTexCoord(vind[vi + 0]).v());
                                wTexCoords.push_back(
                                    f.wedgeTexCoord(vind[vi + 1]).u());
                                wTexCoords.push_back(
                                    f.wedgeTexCoord(vind[vi + 1]).v());
                                wTexCoords.push_back(
                                    f.wedgeTexCoord(vind[vi + 2]).u());
                                wTexCoords.push_back(
                                    f.wedgeTexCoord(vind[vi + 2]).v());
                                wTexIds.push_back(f.textureIndex());
                            }
                        }
                    }
                }
            }
        }
    }

    void fillTextures(const MeshType& m)
    {
        if constexpr (vcl::HasTexturePaths<MeshType>) {
            for (uint i = 0; i < m.textureNumber(); ++i) {
                vcl::Image txt(m.meshBasePath() + m.texturePath(i));
                txt.mirror();
                textures.push_back(txt);
            }
        }
    }

    void fillMeshAttribs(const MeshType& m)
    {
        if constexpr (vcl::HasColor<MeshType>) {
            mColor[0] = m.color().redF();
            mColor[1] = m.color().greenF();
            mColor[2] = m.color().blueF();
            mColor[3] = m.color().alphaF();
        }
    }

    template<typename FaceType>
    void fillFaceNormals(const FaceType& f, bool triangle, uint fi)
    {
        using NormalType = FaceType::VertexType::CoordType;
        NormalType n     = vcl::faceNormal(f);
        if (triangle) {
            tNormals.push_back(n.x());
            tNormals.push_back(n.y());
            tNormals.push_back(n.z());
        }
        else {
            for (uint i = 0; i < triPolyMap.triangleNumber(fi); i++) {
                tNormals.push_back(n.x());
                tNormals.push_back(n.y());
                tNormals.push_back(n.z());
            }
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_MESH_RENDER_BUFFERS_H
