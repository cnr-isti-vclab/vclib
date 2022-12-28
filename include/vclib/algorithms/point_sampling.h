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

#ifndef VCL_ALGORITHMS_POINT_SAMPLING_H
#define VCL_ALGORITHMS_POINT_SAMPLING_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/sampler.h>

namespace vcl {

template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType allVerticesSampling(const MeshType& m, bool onlySelected = false);

template<SamplerConcept SamplerType, MeshConcept MeshType>
SamplerType vertexScalarWeightedSampling(const MeshType& m, uint nSamples);

} // namespace vcl

#include "point_sampling.cpp"

#endif // VCL_ALGORITHMS_POINT_SAMPLING_H
