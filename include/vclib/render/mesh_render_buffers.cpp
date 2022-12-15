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

#include "mesh_render_buffers.h"

namespace vcl {

template<MeshConcept MeshType>
MeshRenderBuffers<MeshType>::MeshRenderBuffers()
{
}

template<MeshConcept MeshType>
MeshRenderBuffers<MeshType>::MeshRenderBuffers(const MeshType& m)
{
	fillVertices(m);
	fillTriangles(m);
}

template<MeshConcept MeshType>
unsigned int MeshRenderBuffers<MeshType>::vertexNumber() const
{
	return nv;
}

template<MeshConcept MeshType>
unsigned int MeshRenderBuffers<MeshType>::triangleNumber() const
{
	return nt;
}

template<MeshConcept MeshType>
const Point3d &MeshRenderBuffers<MeshType>::bbMin() const
{
	return bbmin;
}

template<MeshConcept MeshType>
const Point3d &MeshRenderBuffers<MeshType>::bbMax() const
{
	return bbmax;
}

template<MeshConcept MeshType>
const double* MeshRenderBuffers<MeshType>::vertexBufferData() const
{
	if (verts.empty()) return nullptr;
	return verts.data();
}

template<MeshConcept MeshType>
const int* MeshRenderBuffers<MeshType>::triangleBufferData() const
{
	if (tris.empty()) return nullptr;
	return tris.data();
}

template<MeshConcept MeshType>
const double* MeshRenderBuffers<MeshType>::vertexNormalBufferData() const
{
	if (vNormals.empty()) return nullptr;
	return vNormals.data();
}

template<MeshConcept MeshType>
const float* MeshRenderBuffers<MeshType>::vertexColorBufferData() const
{
	if (vColors.empty()) return nullptr;
	return vColors.data();
}

template<MeshConcept MeshType>
const double* MeshRenderBuffers<MeshType>::triangleNormalBufferData() const
{
	if (tNormals.empty()) return nullptr;
	return tNormals.data();
}

template<MeshConcept MeshType>
const float* MeshRenderBuffers<MeshType>::triangleColorBufferData() const
{
	if (tColors.empty()) return nullptr;
	return tColors.data();
}

template<MeshConcept MeshType>
void MeshRenderBuffers<MeshType>::fillVertices(const MeshType &m)
{
	constexpr bool bbToInitialize = !vcl::HasBoundingBox<MeshType>;
	if constexpr(vcl::HasBoundingBox<MeshType>) {
		bbmin = m.boundingBox().min;
		bbmax = m.boundingBox().max;
	}
	else {
		bbmin = Point3d(
			std::numeric_limits<double>::max(),
			std::numeric_limits<double>::max(),
			std::numeric_limits<double>::max());
		bbmax = Point3d(
			std::numeric_limits<double>::lowest(),
			std::numeric_limits<double>::lowest(),
			std::numeric_limits<double>::lowest());
	}

	nv = m.vertexNumber();
	verts.resize(nv * 3);

	if constexpr(vcl::HasPerVertexNormal<MeshType>) {
		if (vcl::isPerVertexNormalEnabled(m)) {
			vNormals.resize(m.vertexNumber() * 3);
		}
	}

	if constexpr(vcl::HasPerVertexColor<MeshType>) {
		if (vcl::isPerVertexColorEnabled(m)) {
			vColors.resize(m.vertexNumber() * 3);
		}
	}

	uint i = 0;
	for (const auto& v : m.vertices()) {

		verts[i + 0] = v.coord().x();
		verts[i + 1] = v.coord().y();
		verts[i + 2] = v.coord().z();

		if constexpr(bbToInitialize) {
			bbmin = vcl::min(bbmin, v.coord());
			bbmax = vcl::max(bbmax, v.coord());
		}

		if constexpr(vcl::HasPerVertexNormal<MeshType>) {
			if (vcl::isPerVertexNormalEnabled(m)) {
				vNormals[i + 0] = v.normal().x();
				vNormals[i + 1] = v.normal().y();
				vNormals[i + 2] = v.normal().z();
			}
		}

		if constexpr(vcl::HasPerVertexColor<MeshType>) {
			if (vcl::isPerVertexColorEnabled(m)) {
				vColors[i + 0] = v.color().redF();
				vColors[i + 1] = v.color().greenF();
				vColors[i + 2] = v.color().blueF();
			}
		}

		i += 3;
	}
}

template<MeshConcept MeshType>
void MeshRenderBuffers<MeshType>::fillTriangles(const MeshType &m)
{
	if constexpr (vcl::HasFaces<MeshType>) {
		if constexpr (vcl::HasTriangles<MeshType>) {
			nt = m.faceNumber();

			tris.resize(nt * 3);

			uint i = 0;
			for (const auto& f : m.faces()) {
				tris[i + 0] = m.vertexIndexIfCompact(m.index(f.vertex(0)));
				tris[i + 1] = m.vertexIndexIfCompact(m.index(f.vertex(1)));
				tris[i + 2] = m.vertexIndexIfCompact(m.index(f.vertex(2)));

				i += 3;
			}
		}
		else {
			//todo - make polygon triangulation
		}

		if constexpr(vcl::HasPerFaceNormal<MeshType>) {
			if (vcl::isPerFaceNormalEnabled(m)) {
				tNormals.resize(m.faceNumber() * 3);
			}
		}

		if constexpr(vcl::HasPerFaceColor<MeshType>) {
			if (vcl::isPerFaceColorEnabled(m)) {
				tColors.resize(m.faceNumber() * 3);
			}
		}

		uint i = 0;
		for (const auto& f : m.faces()) {

			if constexpr(vcl::HasPerFaceNormal<MeshType>) {
				if (vcl::isPerFaceNormalEnabled(m)) {
					tNormals[i + 0] = f.normal().x();
					tNormals[i + 1] = f.normal().y();
					tNormals[i + 2] = f.normal().z();
				}
			}

			if constexpr(vcl::HasPerFaceColor<MeshType>) {
				if (vcl::isPerFaceColorEnabled(m)) {
					tColors[i + 0] = f.color().redF();
					tColors[i + 1] = f.color().greenF();
					tColors[i + 2] = f.color().blueF();
				}
			}

			i += 3;
		}
	}
}

} // namespace vcl
