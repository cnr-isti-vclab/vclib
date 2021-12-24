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

#ifndef VCL_KD_TREE_H
#define VCL_KD_TREE_H

#include <vclib/misc/vcl_types.h>
#include <vector>

namespace vcl {

template<typename PointType>
class KDTree
{
	using Scalar = typename PointType::ScalarType;
	struct Node;

public:
	KDTree();
	KDTree(
		const std::vector<PointType>& points,
		uint                          pointsPerCell = 16,
		uint                          maxDepth      = 64,
		bool                          balanced      = false);
	template<typename MeshType>
	KDTree(
		const MeshType& m,
		uint                          pointsPerCell = 16,
		uint                          maxDepth      = 64,
		bool                          balanced      = false);

	uint      nearestNeighborIndex(const PointType& queryPoint, Scalar& dist = dummyScalar) const;
	PointType nearestNeighbor(const PointType& queryPoint, Scalar& dist = dummyScalar) const;

	std::vector<uint> kNearestNeighborsIndices(
		const PointType&     queryPoint,
		uint                 k,
		std::vector<Scalar>& distances = dummyScalars) const;
	std::vector<PointType> kNearestNeighbors(
		const PointType&     queryPoint,
		uint                 k,
		std::vector<Scalar>& distances = dummyScalars) const;

	std::vector<uint> neighborsIndicesInDistance(
		const PointType&     queryPoint,
		Scalar               dist,
		std::vector<Scalar>& distances = dummyScalars) const;
	std::vector<PointType> neighborsInDistance(
		const PointType&     queryPoint,
		Scalar               dist,
		std::vector<Scalar>& distances = dummyScalars) const;

private:
	std::vector<PointType> points;
	std::vector<uint>      indices;
	std::vector<Node>      nodes;
	uint                   pointsPerCell = 16; // min number of point in a leaf
	uint                   maxDepth      = 64; // max tree depth
	uint                   depth         = 0;  // actual tree depth

	// dummy values
	inline static Scalar              dummyScalar;
	inline static std::vector<Scalar> dummyScalars;

	uint createTree(uint nodeId, uint start, uint end, uint level, bool balanced);
	uint split(uint start, uint end, uint dim, Scalar splitValue);

	struct Node
	{
		union
		{
			// standard node
			struct
			{
				Scalar splitValue;
				uint   firstChildId : 24;
				uint   dim : 2;
				uint   leaf : 1;
			};
			// leaf
			struct
			{
				uint           start;
				unsigned short size;
			};
		};
	};
	struct QueryNode
	{
		QueryNode() {}
		QueryNode(uint id) : nodeId(id) {}
		uint   nodeId; // id of the next node
		Scalar sq;     // squared distance to the next node
	};
};

} // namespace vcl

#include "kd_tree.cpp"

#endif // VCL_KD_TREE_H
