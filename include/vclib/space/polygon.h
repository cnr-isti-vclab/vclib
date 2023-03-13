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

#ifndef VCL_SPACE_POLYGON_H
#define VCL_SPACE_POLYGON_H

#include <vector>

#include <vclib/concept/space/point.h>

namespace vcl {

template<PointConcept PointT>
class Polygon
{
public:
	using ScalarType = typename PointT::ScalarType;
	using PointType = PointT;

	static const int DIM = PointT::DIM;

	Polygon();

	template<typename Iterator>
	Polygon(Iterator begin, Iterator end)
		requires (std::is_same_v<typename Iterator::value_type, PointT>);

	PointT& point(uint i);
	const PointT& point(uint i) const;

	ScalarType sideLength(uint i) const;

	PointT normal() const requires (PointT::DIM == 3);

	PointT barycenter() const;

	template<typename WIterator>
	PointT weightedBarycenter(WIterator wbegin) const;

	ScalarType perimenter() const;
	ScalarType area() const;

	std::vector<uint> earCut() const requires (PointT::DIM == 2 || PointT::DIM == 3);

	// static member functions

	template<typename Iterator>
	static PointT normal(Iterator begin, Iterator end)
		requires (std::is_same_v<typename Iterator::value_type, PointT> && PointT::DIM == 3);

	template<typename Iterator>
	static PointT barycenter(Iterator begin, Iterator end)
		requires (std::is_same_v<typename Iterator::value_type, PointT>);

	template<typename Iterator, typename WIterator>
	static PointT weightedBarycenter(Iterator begin, Iterator end, WIterator wbegin)
		requires (std::is_same_v<typename Iterator::value_type, PointT>);

	template<typename Iterator>
	static ScalarType perimeter(Iterator begin, Iterator end)
		requires (std::is_same_v<typename Iterator::value_type, PointT>);

	template<typename Iterator>
	static ScalarType area(Iterator begin, Iterator end)
		requires (std::is_same_v<typename Iterator::value_type, PointT>);

	template<typename Iterator>
	static std::vector<uint> earCut(Iterator begin, Iterator end)
		requires (std::is_same_v<typename Iterator::value_type, PointT> && PointT::DIM == 2);

	template<typename Iterator>
	static std::vector<uint> earCut(Iterator begin, Iterator end)
		requires (std::is_same_v<typename Iterator::value_type, PointT> && PointT::DIM == 3);

private:
	std::vector<PointT> p;
};

} // namespace vcl

#include "polygon.cpp"

#endif // VCL_SPACE_POLYGON_H
