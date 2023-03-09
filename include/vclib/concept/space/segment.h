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

#ifndef VCL_CONCEPT_SPACE_SEGMENT_H
#define VCL_CONCEPT_SPACE_SEGMENT_H

#include <vclib/misc/types.h>

namespace vcl {

template<typename T>
concept SegmentConcept = requires(T o, const T& co)
{
	typename T::PointType;
	typename T::ScalarType;
	o.DIM;
	{ o.p0() } -> std::same_as<typename T::PointType&>;
	{ co.p0() } -> std::same_as<const typename T::PointType&>;
	{ o.p1() } -> std::same_as<typename T::PointType&>;
	{ co.p1() } -> std::same_as<const typename T::PointType&>;
};

template<typename T>
concept Segment2Concept = SegmentConcept<T> && T::DIM == 2;

template<typename T>
concept Segment3Concept = SegmentConcept<T> && T::DIM == 3;

} // namespace vcl

#endif // VCL_CONCEPT_SPACE_SEGMENT_H
