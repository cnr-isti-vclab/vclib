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

#include <vclib/algorithms/polygon.h>
#include <vclib/mesh/mesh_algorithms.h>

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
	fillTextures(m);
}

template<MeshConcept MeshType>
uint MeshRenderBuffers<MeshType>::vertexNumber() const
{
	return nv;
}

template<MeshConcept MeshType>
uint MeshRenderBuffers<MeshType>::triangleNumber() const
{
	return nt;
}

template<MeshConcept MeshType>
uint MeshRenderBuffers<MeshType>::textureNumber() const
{
	return textures.size();
}

template<MeshConcept MeshType>
Point2i MeshRenderBuffers<MeshType>::textureSize(uint ti) const
{
	return vcl::Point2i(textures[ti].width(), textures[ti].height());
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
const float* MeshRenderBuffers<MeshType>::vertexBufferData() const
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
const float* MeshRenderBuffers<MeshType>::vertexNormalBufferData() const
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
const float* MeshRenderBuffers<MeshType>::triangleNormalBufferData() const
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
const unsigned char* MeshRenderBuffers<MeshType>::textureBufferData(uint ti) const
{
	return textures[ti].data();
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
			triPolyMap.reserve(m.faceNumber(), m.faceNumber());
			tris.reserve(m.faceNumber());

			nt = 0;
			for (const auto& f : m.faces()) {
				if (f.vertexNumber() == 3) {
					triPolyMap.insert(nt, m.faceIndexIfCompact(m.index(f)));
					tris.push_back(m.vertexIndexIfCompact(m.index(f.vertex(0))));
					tris.push_back(m.vertexIndexIfCompact(m.index(f.vertex(1))));
					tris.push_back(m.vertexIndexIfCompact(m.index(f.vertex(2))));
					nt += 1;
				}
				else {
					std::vector<uint> vind = vcl::mesh::earCut(f);
					for (uint vi = 0; vi < vind.size(); vi+=3) {
						triPolyMap.insert(nt + vi/3, m.faceIndexIfCompact(m.index(f)));
						tris.push_back(m.vertexIndexIfCompact(m.index(f.vertex(vind[vi + 0]))));
						tris.push_back(m.vertexIndexIfCompact(m.index(f.vertex(vind[vi + 1]))));
						tris.push_back(m.vertexIndexIfCompact(m.index(f.vertex(vind[vi + 2]))));
					}
					nt += vind.size() / 3;
				}
			}
		}


		tNormals.reserve(m.faceNumber() * 3);

		if constexpr(vcl::HasPerFaceColor<MeshType>) {
			if (vcl::isPerFaceColorEnabled(m)) {
				tColors.reserve(m.faceNumber() * 3);
			}
		}

		uint i = 0;
		for (const auto& f : m.faces()) {
			if constexpr(vcl::HasPerFaceNormal<MeshType>) {
				if (vcl::isPerFaceNormalEnabled(m)) {
					if constexpr (vcl::HasTriangles<MeshType>) {
						tNormals.push_back(f.normal().x());
						tNormals.push_back(f.normal().y());
						tNormals.push_back(f.normal().z());
					}
					else {
						const uint fi = m.faceIndexIfCompact(m.index(f));
						for (uint i = 0; i < triPolyMap.triangleNumber(fi); i++) {
							tNormals.push_back(f.normal().x());
							tNormals.push_back(f.normal().y());
							tNormals.push_back(f.normal().z());
						}
					}
				}
				else {
					fillFaceNromals(f, vcl::HasTriangles<MeshType>, m.faceIndexIfCompact(m.index(f)));
				}
			}
			else {
				fillFaceNromals(f, vcl::HasTriangles<MeshType>, m.faceIndexIfCompact(m.index(f)));
			}

			if constexpr(vcl::HasPerFaceColor<MeshType>) {
				if (vcl::isPerFaceColorEnabled(m)) {
					if constexpr (vcl::HasTriangles<MeshType>) {
						tColors.push_back(f.color().redF());
						tColors.push_back(f.color().greenF());
						tColors.push_back(f.color().blueF());
					}
					else {
						const uint fi = m.faceIndexIfCompact(m.index(f));
						for (uint i = 0; i < triPolyMap.triangleNumber(fi); i++) {
							tColors.push_back(f.color().redF());
							tColors.push_back(f.color().greenF());
							tColors.push_back(f.color().blueF());
						}
					}
				}
			}

			i += 3;
		}
	}
}

template<MeshConcept MeshType>
void MeshRenderBuffers<MeshType>::fillTextures(const MeshType &m)
{
	if constexpr(vcl::HasTexturePaths<MeshType>) {
		for (uint i = 0; i < m.textureNumber(); ++i) {
			textures.push_back(vcl::Image(m.meshBasePath() + m.texturePath(i)));
		}
	}
}

template<MeshConcept MeshType>
template<typename FaceType>
void MeshRenderBuffers<MeshType>::fillFaceNromals(const FaceType &f, bool triangle, uint fi)
{
	using NormalType = typename FaceType::VertexType::CoordType;
	if (triangle) {
		NormalType n = vcl::triangleNormal(f);
		tNormals.push_back(n.x());
		tNormals.push_back(n.y());
		tNormals.push_back(n.z());
	}
	else {
		NormalType n = vcl::polygonNormal(f);
		for (uint i = 0; i < triPolyMap.triangleNumber(fi); i++) {
			tNormals.push_back(n.x());
			tNormals.push_back(n.y());
			tNormals.push_back(n.z());
		}
	}

}

} // namespace vcl
