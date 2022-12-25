/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include <vclib/image/image.h>
#include <vclib/mesh/requirements.h>
#include <vclib/mesh/tri_poly_index_bimap.h>

#include "mesh_render_settings.h"

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderBuffers
{
public:
	MeshRenderBuffers();
	MeshRenderBuffers(const MeshType& m);

	uint vertexNumber() const;
	uint triangleNumber() const;
	uint textureNumber() const;
	vcl::Point2i textureSize(uint ti) const;

	const vcl::Point3d& bbMin() const;
	const vcl::Point3d& bbMax() const;

	const float*         vertexBufferData() const;
	const int*           triangleBufferData() const;
	const float*         vertexNormalBufferData() const;
	const float*         vertexColorBufferData() const;
	const float*         triangleNormalBufferData() const;
	const float*         triangleColorBufferData() const;
	const float*         vertexTexCoordsBufferData() const;
	const float*         wedgeTexCoordsBufferData() const;
	const short*         wedgeTextureIDsBufferData() const;
	const float*         meshColorBufferData() const;
	const unsigned char* textureBufferData(uint ti) const;

private:
	uint nv = 0;
	uint nt = 0;

	std::vector<float>   verts;
	std::vector<int>     tris;
	std::vector<float>   vNormals;
	std::vector<float>   vColors;
	std::vector<float>   tNormals;
	std::vector<float>   tColors;
	std::vector<float>   vTexCoords;
	std::vector<float>   wTexCoords;
	std::vector<short>   wTexIds;
	std::array<float, 4> mColor;

	vcl::Point3d            bbmin, bbmax;
	mesh::TriPolyIndexBiMap triPolyMap;
	std::vector<vcl::Image> textures;

	void fillVertices(const MeshType& m);
	void fillTriangles(const MeshType& m);
	void fillTextures(const MeshType& m);
	void fillMeshAttribs(const MeshType& m);

	template<typename FaceType>
	void fillFaceNromals(const FaceType& f, bool triangle, uint fi);
};

} // namespace vcl

#include "mesh_render_buffers.cpp"

#endif // VCL_RENDER_MESH_RENDER_BUFFERS_H
