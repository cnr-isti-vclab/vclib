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

#ifndef VCL_SPACE_KD_TREE_H
#define VCL_SPACE_KD_TREE_H

#include <vclib/mesh/requirements.h>
#include <vector>

namespace vcl {

template<PointConcept PointType>
class KDTree
{
	using Scalar = PointType::ScalarType;
	struct Node;

public:
	KDTree();
	KDTree(
		const std::vector<PointType>& points,
		uint                          pointsPerCell = 16,
		uint                          maxDepth      = 64,
		bool                          balanced      = false);

	template<MeshConcept MeshType>
	KDTree(
		const MeshType& m,
		uint                          pointsPerCell = 16,
		uint                          maxDepth      = 64,
		bool                          balanced      = false)
		requires (std::is_same_v<typename MeshType::VertexType::CoordType, PointType>);

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

// deduction guides
template<MeshConcept MeshType>
KDTree(const MeshType& m) -> KDTree<typename MeshType::VertexType::CoordType>;

template<MeshConcept MeshType>
KDTree(const MeshType& m, uint pointsPerCell) -> KDTree<typename MeshType::VertexType::CoordType>;

template<MeshConcept MeshType>
KDTree(const MeshType& m, uint pointsPerCell, uint maxDepth)
	-> KDTree<typename MeshType::VertexType::CoordType>;

template<MeshConcept MeshType>
KDTree(const MeshType& m, uint pointsPerCell, uint maxDepth, bool balanced)
	-> KDTree<typename MeshType::VertexType::CoordType>;

} // namespace vcl

#include "kd_tree.cpp"

#endif // VCL_SPACE_KD_TREE_H
