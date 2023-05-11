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

#include "color.h"

#include <vclib/math/perlin_noise.h>
#include <vclib/views/mesh.h>

#include "../clean.h"
#include "../stat.h"

namespace vcl {

namespace internal {

struct ColorAvgInfo
{
	vcl::Point4<uint> c   = vcl::Point4<uint>(0, 0, 0, 0);
	uint              cnt = 0;
};

} // namespace internal

/**
 * @brief Sets the color of the vertices. If the `onlySelected` flag is set to `true`, only the
 * color of the selected vertices will be set. Otherwise, all the vertices will have the same color.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *
 * @param[in,out] m: the mesh on which set the vertex color.
 * @param[in] c: the color to set to the vertices of the mesh.
 * @param[in] onlySelected: if `true`, the color will be set just on the selected vertices.
 */
template<MeshConcept MeshType>
void setPerVertexColor(MeshType& m, vcl::Color c, bool onlySelected)
{
	vcl::requirePerVertexColor(m);
	
#ifdef VCLIB_USES_RANGES
	if (onlySelected) {
		std::ranges::fill(m.vertices() | views::selected | views::colors, c);
	}
	else {
		std::ranges::fill(m.vertices() | views::colors, c);
	}
#else
	using VertexType = typename MeshType::VertexType;
	
	for (VertexType& v : m.vertices()) {
		if (!onlySelected || v.selected()) {
			v.color() = c;
		}
	}
#endif
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
 * @param[in,out] m: the mesh on which set the face color.
 * @param[in] c: the color to set to the faces of the mesh.
 * @param[in] onlySelected: if `true`, the color will be set just on the selected faces.
 */
template<FaceMeshConcept MeshType>
void setPerFaceColor(MeshType& m, vcl::Color c, bool onlySelected)
{
	vcl::requirePerFaceColor(m);
	
#ifdef VCLIB_USES_RANGES
	if (onlySelected) {
		std::ranges::fill(m.faces() | views::selected | views::colors, c);
	}
	else {
		std::ranges::fill(m.faces() | views::colors, c);
	}
#else
	using FaceType = typename MeshType::FaceType;
	
	for (FaceType& f : m.faces()) {
		if (!onlySelected || f.selected()) {
			f.color() = c;
		}
	}
#endif
}

/**
 * @brief Sets the color component of a mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Color
 *
 * @param[in,out] m: the mesh on which set the color.
 * @param[in] c: the color to set to the mesh.
 */
template<HasColor MeshType>
void setMeshColor(MeshType& m, vcl::Color c)
{
	m.color() = c;
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
 * @param[in,out] m: mesh on which transfer the face color into the vertex color.
 */
template<FaceMeshConcept MeshType>
void setPerVertexColorFromFaceColor(MeshType& m)
{
	vcl::requirePerVertexColor(m);
	vcl::requirePerFaceColor(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;

	std::vector<internal::ColorAvgInfo> csi(m.vertexContainerSize());

	for (const FaceType& f : m.faces()) {
		for (const VertexType* v : f.vertices()) {
			csi[m.index(v)].c += v->color();
			csi[m.index(v)].cnt++;
		}
	}

	for (VertexType& v : m.vertices()) {
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
 * @param[in,out] m: mesh on which transfer the vertex color into the face color.
 */
template<FaceMeshConcept MeshType>
void setPerFaceColorFromVertexColor(MeshType& m)
{
	vcl::requirePerVertexColor(m);
	vcl::requirePerFaceColor(m);

	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;

	for (FaceType& f : m.faces()) {
		vcl::Point4<uint> avg(0, 0, 0, 0);
		for (const VertexType* v : f.vertices()) {
			avg += v->color().template cast<uint>();
		}
		avg /= f.vertexNumber();
		f.color() = avg.cast<uint8_t>();
	}
}

/**
 * @brief Sets the vertex colors from the quality values by computing a shading in the given color
 * map (default is Red to Blue color map), in the given interval [minQuality, maxQuality].
 *
 * If minQuality and maxQuality are not set (or if they are equal), the range is automatically
 * computed.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *     - Quality
 *
 * @param[in,out] m: mesh on which compute the vertex color.
 * @param[in] colorMap: the colormap to use to color the vertices of the mesh (default: RedBlue).
 * @param[in] minQuality: the minimum value of the range to use for coloring (default: 0).
 * @param[in] maxQuality: the maximum value of the range to use for coloring (default: 0).
 */
template<MeshConcept MeshType>
void setPerVertexColorFromQuality(
	MeshType&                                 m,
	vcl::Color::ColorMap                      colorMap,
	typename MeshType::VertexType::QualityType minQuality,
	typename MeshType::VertexType::QualityType maxQuality)
{
	vcl::requirePerVertexColor(m);
	vcl::requirePerVertexQuality(m);

	using VertexType = typename MeshType::VertexType;
	using QualityType = typename VertexType::QualityType;

	if (minQuality == maxQuality) {
		std::pair<QualityType, QualityType> pair = vertexQualityMinMax(m);
		minQuality                              = pair.first;
		maxQuality                              = pair.second;
	}
	for (VertexType& v : m.vertices()) {
		v.color() = colorFromInterval(minQuality, maxQuality, v.quality(), colorMap);
	}
}

/**
 * @brief Sets the face colors from the quality values by computing a shading in the given color
 * map (default is Red to Blue color map), in the given interval [minQuality, maxQuality].
 *
 * If minQuality and maxQuality are not set (or if they are equal), the range is automatically
 * computed.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Color
 *     - Quality
 *
 * @param[in,out] m: mesh on which compute the face color.
 * @param[in] colorMap: the colormap to use to color the faces of the mesh (default: RedBlue).
 * @param[in] minQuality: the minimum value of the range to use for coloring (default: 0).
 * @param[in] maxQuality: the maximum value of the range to use for coloring (default: 0).
 */
template<FaceMeshConcept MeshType>
void setPerFaceColorFromQuality(
	MeshType&                               m,
	vcl::Color::ColorMap                    colorMap,
	typename MeshType::FaceType::QualityType minQuality,
	typename MeshType::FaceType::QualityType maxQuality)
{
	vcl::requirePerFaceColor(m);
	vcl::requirePerFaceQuality(m);

	using FaceType   = typename MeshType::FaceType;
	using QualityType = typename FaceType::QualityType;

	if (minQuality == maxQuality) {
		std::pair<QualityType, QualityType> pair = faceQualityMinMax(m);
		minQuality                              = pair.first;
		maxQuality                              = pair.second;
	}
	for (FaceType& f : m.faces()) {
		f.color() = colorFromInterval(minQuality, maxQuality, f.quality(), colorMap);
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
 * @param[in,out] m: the mesh on which update the vertex color from the border face flags.
 * @param[in] borderColor: the color of the vertices that are part of edges that are all marked as
 * on border.
 * @param[in] internalColor: the color of the vertices that are part of edges that are all marked as
 * non on border.
 * @param[in] mixColor: the color of vertices that are part of edges that are both on border and non
 * on border.
 */
template<FaceMeshConcept MeshType>
void setPerVertexColorFromFaceBorderFlag(
	MeshType& m,
	Color     borderColor,
	Color     internalColor,
	Color     mixColor)
{
	vcl::requirePerVertexColor(m);

	using FaceType = typename MeshType::FaceType;

	const vcl::Color baseColor = Color::Green;

	setPerVertexColor(m, baseColor);

	for (FaceType& f : m.faces()) {
		for (uint i = 0; i < f.vertexNumber(); ++i) {
			if (f.edgeOnBorder(i)) {
				if (f.vertex(i).color() == baseColor)
					f.vertex(i).color() = borderColor;
				if (f.vertex(i).color() == internalColor)
					f.vertex(i).color() = mixColor;
				if (f.vertexMod(i + 1).color() == baseColor)
					f.vertexMod(i + 1).color() = borderColor;
				if (f.vertexMod(i + 1).color() == internalColor)
					f.vertexMod(i + 1).color() = mixColor;
			}
			else {
				if (f.vertex(i).color() == baseColor)
					f.vertex(i).color() = internalColor;
				if (f.vertex(i).color() == borderColor)
					f.vertex(i).color() = mixColor;
				if (f.vertexMod(i + 1).color() == baseColor)
					f.vertexMod(i + 1).color() = internalColor;
				if (f.vertexMod(i + 1).color() == borderColor)
					f.vertexMod(i + 1).color() = mixColor;
			}
		}
	}
}

/**
 * @brief Given an already computed vector of sets of connected components (see
 * vcl::connectedComponents(m) in `vclib/algorithms/clean.h`), sets face colors according from
 * connected components of the mesh. Each connected component will have a different per face color.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Color
 *
 * @param[in,out] m: the mesh on which set the face colors according to its connected components.
 * @param[in] connectedComponents: a vector of sets, each one of them containing the face ids of a
 * connected component.
 */
template<FaceMeshConcept MeshType>
void setPerFaceColorFromConnectedComponents(
	MeshType&                          m,
	const std::vector<std::set<uint>>& connectedComponents)
{
	std::vector<Color> vc = colorScattering(connectedComponents.size());

	uint cid = 0;
	for (const std::set<uint>& connComp : connectedComponents) {
		for (const uint& fid : connComp) {
			m.face(fid).color() = vc[cid];
		}
		cid++;
	}
}

/**
 * @brief Sets face colors according from connected components of the mesh. Each connected component
 * will have a different per face color. Since this function will need to compute connected
 * components of the mesh, also per Face AdjacentFaces component is required.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - AdjacentFaces
 *     - Color
 *
 * @param[in,out] m: the mesh on which set the face colors according to its connected components.
 */
template<FaceMeshConcept MeshType>
void setPerFaceColorFromConnectedComponents(MeshType& m)
{
	vcl::requirePerFaceColor(m);

	std::vector<std::set<uint>> connComps = connectedComponents(m);

	setPerFaceColorFromConnectedComponents(m, connComps);
}

/**
 * @brief This function colors each face of the mesh using a given number of scattering colors
 * (default number is 50). By default, colors uniformely adjacent faces having faux edges in common.
 * This check is made only if the option `checkFauxEdges` is set to true, and if the per Face
 * AdjacentFaces component is available in the mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Color
 *
 * Optional:
 * - Mesh:
 *   - Faces:
 *     - AdjacentFaces
 *
 * @param[in,out] m: mesh on which set the face colors.
 * @param[in] nColors: number of colors used for the scattering (default 50).
 * @param[in] checkFauxEdges: if true, colors uniformely adjacent faces with faux edges (default
 * `true`).
 */
template<FaceMeshConcept MeshType>
void setPerFaceColorScattering(MeshType& m, uint nColors, bool checkFauxEdges)
{
	vcl::requirePerFaceColor(m);

	using FaceType = typename MeshType::FaceType;

	Color baseColor = Color::Black;
	setPerFaceColor(m, baseColor);

	std::vector<Color> vc = colorScattering(nColors);

	for (FaceType& f : m.faces()) {
		if (f.color() == baseColor) {
			f.color() = vc[m.index(f) % nColors];
		}
		if constexpr (HasPerFaceAdjacentFaces<MeshType>) {
			if (checkFauxEdges && isPerFaceAdjacentFacesEnabled(m)) {
				for (uint i = 0; i < f.vertexNumber(); ++i) {
					if (f.edgeFaux(i)) {
						assert(f.adjFace(i) != nullptr);
						f.adjFace(i)->color = f.color();
					}
				}
			}
		}
	}
}

/**
 * @brief Set the vertex color according to a perlin noise computed on the vertex coordinates.
 *
 * To make things weirder each color band can have its own offset and frequency.
 * Period is expressed in absolute terms.
 * So as period it is meaningful could be to use something in the range of 1/10 of the bbox diag.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *
 * @param m
 * @param period
 * @param offset
 * @param onSelected
 */
template<MeshConcept MeshType, PointConcept PointType>
void setPerVertexColorPerlinNoise(MeshType& m, PointType period, PointType offset, bool onSelected)
{
	vcl::requirePerVertexColor(m);

	using VertexType = typename MeshType::VertexType;

	PointType p[3];

	for (VertexType& v : m.vertices()) {
		if (!onSelected || v.selected()) {
			p[0]      = (v.coord() / period[0]) + offset;
			p[1]      = (v.coord() / period[1]) + offset;
			p[2]      = (v.coord() / period[2]) + offset;
			v.color() = Color(
				127 + 128.0 * perlinNoise(p[0][0], p[0][1], p[0][2]),
				127 + 128.0 * perlinNoise(p[1][0], p[1][1], p[1][2]),
				127 + 128.0 * perlinNoise(p[2][0], p[2][1], p[2][2]),
				255);
		}
	}
}

/**
 * @brief Simple Perlin color mixing. `color1` and `color2` are mixed according the perlin noise
 * function, with period and offset.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Color
 *
 * @param m
 * @param period
 * @param offset
 * @param color1
 * @param color2
 * @param onSelected
 */
template<MeshConcept MeshType, PointConcept PointType>
void setPerVertexPerlinColor(
	MeshType&      m,
	double         period,
	PointType      offset,
	Color          color1,
	Color          color2,
	bool           onSelected)
{
	vcl::requirePerVertexColor(m);

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		if (!onSelected || v.selected()) {
			PointType p = v.coord() / period + offset;

			double factor = (perlinNoise(p[0], p[1], p[2]) + 1.0) / 2.0;

			int rr = (color1[0] * factor) + (color2[0] * (1.0 - factor));
			int gg = (color1[1] * factor) + (color2[1] * (1.0 - factor));
			int bb = (color1[2] * factor) + (color2[2] * (1.0 - factor));
			int aa = (color1[3] * factor) + (color2[3] * (1.0 - factor));

			v.color() = Color(rr, gg, bb, aa);
		}
	}
}

} // namespace vcl
