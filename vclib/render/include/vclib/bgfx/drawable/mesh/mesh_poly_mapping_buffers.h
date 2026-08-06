// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_MESH_MESH_POLY_MAPPING_BUFFERS_H
#define VCL_BGFX_DRAWABLE_MESH_MESH_POLY_MAPPING_BUFFERS_H

#include "mesh_render_buffers_macros.h"

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>

#include <vclib/space/core.h>
#include <vclib/space/complex.h>

#include <bgfx/bgfx.h>

namespace vcl {

/**
 * @brief Manages polygon-to-triangle mapping buffers for a mesh.
 *
 * This class encapsulates the buffers required to map triangulated indices
 * back to their original polygon indices. It is used by both the selection
 * compute shaders and the surface ID pass.
 */
class MeshPolyMappingBuffers
{
    // Polygon-to-triangle mapping buffers for polygon-level face selection and face ID
    IndexBuffer mTriToPolyBuffer;
    IndexBuffer mPolyToTriBeginBuffer;
    IndexBuffer mPolyToTriCountBuffer;

    bool mIsTriMesh = false;

public:
    MeshPolyMappingBuffers() = default;

    void swap(MeshPolyMappingBuffers& other)
    {
        using std::swap;
        swap(mTriToPolyBuffer, other.mTriToPolyBuffer);
        swap(mPolyToTriBeginBuffer, other.mPolyToTriBeginBuffer);
        swap(mPolyToTriCountBuffer, other.mPolyToTriCountBuffer);
        swap(mIsTriMesh, other.mIsTriMesh);
    }

    friend void swap(MeshPolyMappingBuffers& a, MeshPolyMappingBuffers& b)
    {
        a.swap(b);
    }

    /**
     * @brief Uploads the polygon-to-triangle mapping buffers to the GPU.
     *
     * These three buffers allow shaders to convert from triangle
     * indices back to the original polygon indices.
     *
     * Call this after the triangle index buffer has been built.
     *
     * @param[in] indexMap: Bidirectional triangle ↔ polygon index map.
     * @param[in] numTris: Number of triangles (after triangulation).
     */
    void init(const TriPolyIndexBiMap& indexMap, uint numTris)
    {
        mIsTriMesh = indexMap.isTriMesh();

        if (numTris == 0)
            return;

        const uint numPolys = indexMap.polygonCount();

        // tri_to_poly[triIdx] = polygon index
        {
            auto [buf, rel] =
                Context::getAllocatedBufferAndReleaseFn<uint>(numTris);
            for (uint i = 0; i < numTris; i++) {
                buf[i] = indexMap.polygon(i);
            }
            mTriToPolyBuffer.create(
                buf, numTris, PrimitiveType::UINT, bgfx::Access::Read, rel);
        }

        // poly_to_tri_begin[polyIdx] and poly_to_tri_count[polyIdx]
        {
            auto [bufBegin, relBegin] =
                Context::getAllocatedBufferAndReleaseFn<uint>(numPolys);
            auto [bufCount, relCount] =
                Context::getAllocatedBufferAndReleaseFn<uint>(numPolys);

            for (uint i = 0; i < numPolys; i++) {
                uint begin = indexMap.triangleBegin(i);
                if (begin != UINT_NULL) {
                    bufBegin[i] = begin;
                    bufCount[i] = indexMap.triangleCount(i);
                }
                else {
                    // deleted polygon — will never be accessed by a valid
                    // triangle thread
                    bufBegin[i] = 0;
                    bufCount[i] = 0;
                }
            }

            mPolyToTriBeginBuffer.create(
                bufBegin,
                numPolys,
                PrimitiveType::UINT,
                bgfx::Access::Read,
                relBegin);
            mPolyToTriCountBuffer.create(
                bufCount,
                numPolys,
                PrimitiveType::UINT,
                bgfx::Access::Read,
                relCount);
        }
    }

    bool isTriMesh() const { return mIsTriMesh; }

    void bindTriToPolyBuffer(uint stage = VCL_MRB_TRI_TO_POLY_BUFFER) const
    {
        mTriToPolyBuffer.bind(stage, bgfx::Access::Read);
    }

    void bindPolyToTriBeginBuffer(uint stage = VCL_MRB_POLY_TO_TRI_BEGIN_BUFFER) const
    {
        mPolyToTriBeginBuffer.bind(stage, bgfx::Access::Read);
    }

    void bindPolyToTriCountBuffer(uint stage = VCL_MRB_POLY_TO_TRI_COUNT_BUFFER) const
    {
        mPolyToTriCountBuffer.bind(stage, bgfx::Access::Read);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_MESH_MESH_POLY_MAPPING_BUFFERS_H
