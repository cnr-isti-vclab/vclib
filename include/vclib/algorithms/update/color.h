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

#ifndef VCL_ALGORITHMS_UPDATE_COLOR_H
#define VCL_ALGORITHMS_UPDATE_COLOR_H

#include <set>

#include <vclib/mesh/requirements.h>
#include <vclib/space/color.h>

namespace vcl {

template<MeshConcept MeshType>
void setPerVertexColor(MeshType& m, vcl::Color c = vcl::Color::White, bool onlySelected = false);

template<FaceMeshConcept MeshType>
void setPerFaceColor(MeshType& m, vcl::Color c = vcl::Color::White, bool onlySelected = false);

template<HasColor MeshType>
void setMeshColor(MeshType& m, vcl::Color c = vcl::Color::White);

template<FaceMeshConcept MeshType>
void setPerVertexColorFromFaceColor(MeshType& m);

template<FaceMeshConcept MeshType>
void setPerFaceColorFromVertexColor(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexColorFromQuality(
	MeshType&                                  m,
	vcl::Color::ColorMap                       colorMap  = vcl::Color::RedBlue,
	typename MeshType::VertexType::QualityType minQuality = 0,
	typename MeshType::VertexType::QualityType maxQuality = 0);

template<FaceMeshConcept MeshType>
void setPerFaceColorFromQuality(
	MeshType&                                m,
	vcl::Color::ColorMap                     colorMap  = vcl::Color::RedBlue,
	typename MeshType::FaceType::QualityType minQuality = 0,
	typename MeshType::FaceType::QualityType maxQuality = 0);

template<FaceMeshConcept MeshType>
void setPerVertexColorFromFaceBorderFlag(
	MeshType&  m,
	vcl::Color borderColor   = vcl::Color::Blue,
	vcl::Color internalColor = vcl::Color::White,
	vcl::Color mixColor      = vcl::Color::Cyan);

template<FaceMeshConcept MeshType>
void setPerFaceColorFromConnectedComponents(
	MeshType& m,
	const std::vector<std::set<uint>>& connectedComponents);

template<FaceMeshConcept MeshType>
void setPerFaceColorFromConnectedComponents(MeshType& m);

template<FaceMeshConcept MeshType>
void setPerFaceColorScattering(MeshType& m, uint nColors = 50, bool checkFauxEdges = true);

template<MeshConcept MeshType, PointConcept PointType>
void setPerVertexColorPerlinNoise(
	MeshType& m,
	PointType period,
	PointType offset     = PointType(0, 0, 0),
	bool      onSelected = false);

template<MeshConcept MeshType, PointConcept PointType>
void setPerVertexPerlinColor(
	MeshType& m,
	double period,
	PointType offset  = PointType(0, 0, 0),
	Color color1 = Color::Black,
	Color color2 = Color::White,
	bool onSelected = false);

} // namespace vcl

#include "color.cpp"

#endif // VCL_ALGORITHMS_UPDATE_COLOR_H
