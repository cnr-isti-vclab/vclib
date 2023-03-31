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

#ifndef VCL_ALGORITHM_DISTANCE_MESH_H
#define VCL_ALGORITHM_DISTANCE_MESH_H

#include <vclib/math/histogram.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/logger.h>

namespace vcl {

struct HausdorffDistResult {
	double minDist  = std::numeric_limits<double>::max();
	double maxDist  = std::numeric_limits<double>::lowest();
	double meanDist = 0;
	double RMSDist  = 0;
	Histogram<double> histogram;
};

enum HausdorffSamplingMethod {
	HAUSDORFF_VERTEX_UNIFORM = 0,
	HAUSDORFF_EDGE_UNIFORM,
	HAUSDORFF_MONTECARLO
};

template<MeshConcept MeshType1, MeshConcept MeshType2, LoggerConcept LogType = NullLogger>
HausdorffDistResult hausdorffDistance(
	const MeshType1& m1,
	const MeshType2& m2,
	LogType& log = nullLogger,
	HausdorffSamplingMethod sampMethod = HAUSDORFF_VERTEX_UNIFORM,
	uint nSamples = 0,
	bool deterministic = false);

} // namespace vcl

#include "mesh.cpp"

#endif // VCL_ALGORITHM_DISTANCE_MESH_H
