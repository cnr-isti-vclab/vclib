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

/**
 * @brief Sets the vertex colors from its incident face colors, computing a plain average of the
 * face colors.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *   - Faces:
 *     - Color
 *
 * @param[in/out] m: mesh on which transfer the face color into the vertex color.
 */
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

/**
 * @brief Sets the face colors from its incident vertex colors, computing a plain average of the
 * vertex colors.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *   - Faces:
 *     - Color
 *
 * @param[in/out] m: mesh on which transfer the vertex color into the face color.
 */
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

/**
 * @brief Sets the vertex colors from the scalar values by computing a shading in the given color
 * map (default is Red to Blue color map), in the given interval [minScalar, maxScalar].
 *
 * If minScalar and maxScalar are not set (or if they are equal), the range is automatically
 * computed.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *     - Scalar
 *
 * @param[in/out] m: mesh on which compute the vertex color.
 * @param[in] colorMap: the colormap to use to color the vertices of the mesh (default: RedBlue).
 * @param[in] minScalar: the minimum value of the range to use for coloring (default: 0).
 * @param[in] maxScalar: the maximum value of the range to use for coloring (default: 0).
 */
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

/**
 * @brief Sets the face colors from the scalar values by computing a shading in the given color
 * map (default is Red to Blue color map), in the given interval [minScalar, maxScalar].
 *
 * If minScalar and maxScalar are not set (or if they are equal), the range is automatically
 * computed.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Color
 *     - Scalar
 *
 * @param[in/out] m: mesh on which compute the face color.
 * @param[in] colorMap: the colormap to use to color the faces of the mesh (default: RedBlue).
 * @param[in] minScalar: the minimum value of the range to use for coloring (default: 0).
 * @param[in] maxScalar: the maximum value of the range to use for coloring (default: 0).
 */
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

/**
 * @brief Color the vertices of the mesh that are on border, using the border flags of the faces.
 *
 * Before using this function, you should update the face border flags accordingly.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *   - Faces:
 *     - TriangleFlags/PolygonFlags
 *
 * @param[in/out] m: the mesh on which update the vertex color from the border face flags.
 * @param[in] borderColor: the color of the vertices that are part of edges that are all marked as
 * on border.
 * @param[in] internalColor: the color of the vertices that are part of edges that are all marked as
 * non on border.
 * @param[in] mixColor: the color of vertices that are part of edges that are both on border and non
 * on border.
 */
template<typename MeshType>
void setPerVertexColorFromFaceBorderFlag(
	MeshType& m,
	Color     borderColor,
	Color     internalColor,
	Color     mixColor)
{
	vcl::requirePerVertexColor(m);
	vcl::requireFaces<MeshType>();

	using FaceType = typename MeshType::FaceType;

	const vcl::Color baseColor = Color::Green;

	setPerVertexColor(m, baseColor);

	for (FaceType& f : m.faces()) {
		for (uint i = 0; i < f.vertexNumber(); ++i) {
			if (f.isEdgeOnBorder(i)) {
				if (f.vertex(i).color() == baseColor)
					f.vertex(i).color() = borderColor;
				if (f.vertex(i).color() == internalColor)
					f.vertex(i).color() = mixColor;
				if (f.vertexMod(i+1).color() == baseColor)
					f.vertexMod(i+1).color() = borderColor;
				if (f.vertexMod(i+1).color() == internalColor)
					f.vertexMod(i+1).color() = mixColor;
			}
			else {
				if (f.vertex(i).color() == baseColor)
					f.vertex(i).color() = internalColor;
				if (f.vertex(i).color() == borderColor)
					f.vertex(i).color() = mixColor;
				if (f.vertexMod(i+1).color() == baseColor)
					f.vertexMod(i+1).color() = internalColor;
				if (f.vertexMod(i+1).color() == borderColor)
					f.vertexMod(i+1).color() = mixColor;
			}
		}
	}
}

} // namespace vcl
