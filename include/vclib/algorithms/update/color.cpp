/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "color.h"

#include <vclib/mesh/requirements.h>

#include "../stat.h"

namespace vcl {

namespace internal {

struct ColorAvgInfo
{
	vcl::Point4<uint> c =  vcl::Point4<uint>(0,0,0,0);
	uint cnt = 0;
};

}

/**
 * @brief Sets the color of the vertices. If the `onlySelected` flag is set to `true`, only the
 * color of the selected vertices will be set. Otherwise, all the vertices will have the same color.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *
 * @param[in/out] m: the mesh on which set the vertex color.
 * @param[in] c: the color to set to the vertices of the mesh.
 * @param[in] onlySelected: if `true`, the color will be set just on the selected vertices.
 */
template<typename MeshType>
void setPerVertexColor(MeshType& m, vcl::Color c, bool onlySelected)
{
	vcl::requirePerVertexColor(m);

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		if (!onlySelected || v.isSelected()) {
			v.color() = c;
		}
	}
}

/**
 * @brief Sets the color of the faces. If the `onlySelected` flag is set to `true`, only the
 * color of the selected faces will be set. Otherwise, all the faces will have the same color.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Color
 *
 * @param[in/out] m: the mesh on which set the face color.
 * @param[in] c: the color to set to the faces of the mesh.
 * @param[in] onlySelected: if `true`, the color will be set just on the selected faces.
 */
template<typename MeshType>
void setPerFaceColor(MeshType& m, vcl::Color c, bool onlySelected)
{
	vcl::requirePerFaceColor(m);

	using FaceType = typename MeshType::FaceType;

	for (FaceType& f : m.faces()) {
		if (!onlySelected || f.isSelected()) {
			f.color() = c;
		}
	}
}

template<typename MeshType>
void setPerVertexColorFromFaceColor(MeshType& m)
{
	vcl::requirePerVertexColor(m);
	vcl::requirePerFaceColor(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType = typename MeshType::FaceType;

	std::vector<internal::ColorAvgInfo> csi(m.vertexContainerSize());

	for (const FaceType& f : m.faces()){
		for (const VertexType* v : f.vertices()){
			csi[m.index(v)].c += v->color();
			csi[m.index(v)].cnt++;
		}
	}

	for (VertexType& v : m.vertices()){
		v.color() = csi[m.index(v)].c / csi[m.index(v)].cnt;
	}
}

template<typename MeshType>
void setPerFaceColorFromVertexColor(MeshType& m)
{
	vcl::requirePerVertexColor(m);
	vcl::requirePerFaceColor(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType = typename MeshType::FaceType;

	for (FaceType& f : m.faces()){
		vcl::Point4<uint> avg(0,0,0,0);
		for (const VertexType* v : f.vertices()){
			avg += v->color();
		}
		f->color() = avg / f.vertexNumber();
	}
}

template<typename MeshType>
void setPerVertexColorFromScalar(
	MeshType&                                 m,
	vcl::Color::ColorMap                      colorMap,
	typename MeshType::VertexType::ScalarType minScalar,
	typename MeshType::VertexType::ScalarType maxScalar)
{
	vcl::requirePerVertexColor(m);
	vcl::requirePerVertexScalar(m);

	using VertexType = typename MeshType::VertexType;
	using ScalarType = typename VertexType::ScalarType;

	if (minScalar == maxScalar){
		std::pair<ScalarType, ScalarType> pair = perVertexScalarMinMax(m);
		minScalar = pair.first;
		maxScalar = pair.second;
	}
	for (VertexType& v : m.vertices()){
		v.color() = colorFromInterval(minScalar, maxScalar, v.scalar(), colorMap);
	}
}

template<typename MeshType>
void setPerFaceColorFromScalar(
	MeshType&                                 m,
	vcl::Color::ColorMap                      colorMap,
	typename MeshType::FaceType::ScalarType minScalar,
	typename MeshType::FaceType::ScalarType maxScalar)
{
	vcl::requirePerFaceColor(m);
	vcl::requirePerFaceScalar(m);

	using FaceType = typename MeshType::FaceType;
	using ScalarType = typename FaceType::ScalarType;

	if (minScalar == maxScalar){
		std::pair<ScalarType, ScalarType> pair = perFaceScalarMinMax(m);
		minScalar = pair.first;
		maxScalar = pair.second;
	}
	for (FaceType& f : m.faces()){
		f.color() = colorFromInterval(minScalar, maxScalar, f.scalar(), colorMap);
	}
}

} // namespace vcl
